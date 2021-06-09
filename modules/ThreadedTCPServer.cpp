//
// Created by Bulut Gözübüyük on 1.06.2021.
//

#include "ThreadedTCPServer.h"

ThreadedTCPServer::ThreadedTCPServer(int id, int port) : ThreadedModule(id) , server_port(port) {
    this->acceptTimeout = 5;
}

bool ThreadedTCPServer::start() {
    ThreadedModule::start();
    return pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(runServer), this);
}

bool ThreadedTCPServer::stop() {
    printf("TCP Server Close Called.\n");
    close(this->clientSocket);
    printf("Client socket closed.\n");
    return ThreadedModule::stop();
}

void ThreadedTCPServer::runServer(void *obj_param) {
    ThreadedTCPServer *tcpServer = ((ThreadedTCPServer *) obj_param);
    // Create Socket
    tcpServer->socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpServer->socketFD != -1) {
        sockaddr_in hint{};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(tcpServer->server_port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        int enable = 1;
        if (setsockopt(tcpServer->socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            printf("setsockopt(SO_REUSEADDR) failed");

        if (bind(tcpServer->socketFD, (sockaddr *) &hint, sizeof(hint)) != -1) {
            printf(GREEN "Bind!\n" RESET);
            if (listen(tcpServer->socketFD, SOMAXCONN) != -1) {
                tcpServer->handleServerListen();
            } else printf(RED "Cannot listen!\n" RESET);
        } else printf(RED "Cannot bind IP/Port!\n" RESET);
    } else printf(RED "Create socket failed!\n" RESET);
}

int ThreadedTCPServer::handleServerListen() {
    printf(GREEN "Listen!\n" RESET);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    // manage timeout start
    int iResult;
    struct timeval tv;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(this->socketFD, &rfds);

    tv.tv_sec = (long) this->acceptTimeout;
    tv.tv_usec = 0;

    printf(YELLOW "Try to accept! loop: %d\n" RESET, this->loop);
    iResult = select(this->socketFD + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
    // manage timeout end

    while (this->loop) {
        if (iResult > 0) {
            this->clientSocket = accept(this->socketFD, (sockaddr *) &client, &clientSize);

            if (this->clientSocket != -1) {
                printf("Accepted clientSocket!\n");
                close(this->socketFD);
                memset(host, 0, NI_MAXHOST);
                memset(svc, 0, NI_MAXSERV);

                int result = getnameinfo((sockaddr *) &client, sizeof(client),
                                         host, NI_MAXHOST,
                                         svc, NI_MAXSERV, 0);

                if (result) printf("%s connected on %s\n", host, svc);
                else {
                    inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                    printf(GREEN "%s connected on %hu\n" RESET, host, ntohs(client.sin_port));
                    this->handleReceiveLoop();
                }
            } else {
                printf(YELLOW "No client connected, running server again!\n" RESET);
                runServer(this);
            }
        } else {
            printf(YELLOW "Timeout: retry to accept! loop: %d\n" RESET, this->loop);
            FD_ZERO(&rfds);
            FD_SET(this->socketFD, &rfds);
            iResult = select(this->socketFD + 1, &rfds, (fd_set *) 0, (fd_set *) 0, &tv);
        }
    }
    return 0;
}

int ThreadedTCPServer::handleReceiveLoop() {
    while (this->loop) {
        // Clear buffer
        memset(this->recv_buffer, 0, 4096);

        int bytesRecv = recv(this->clientSocket, this->recv_buffer, RECV_BUFFER_SIZE, 0);
        if (bytesRecv == -1) {
            std::cerr << "There was a connecetion issue." << std::endl;
            break;
        } else if (bytesRecv == 0) {
            std::cerr << "Client disconnected." << std::endl;
            break;
        }

        std::string strRecv = std::string(this->recv_buffer, 0, bytesRecv);

        this->handleReceivedString(strRecv, bytesRecv);
        //resend msg (echo)
        send(clientSocket, this->recv_buffer, bytesRecv + 1,0);
    }
    return 0;
}

int ThreadedTCPServer::handleReceivedString(std::string strRecv, int bytesRecv) {
    //clean \n
    strRecv.erase(std::remove(strRecv.begin(), strRecv.end(), '\n'), strRecv.end());
    printf(GREEN "Server Received: %s Loop %d\n" RESET, strRecv.c_str(), this->loop);

    return 0;
}


ThreadedTCPServer::~ThreadedTCPServer() = default;
