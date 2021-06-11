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
    tcpServer->master_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpServer->master_socketFD != -1) {
        sockaddr_in hint{};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(tcpServer->server_port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        int enable = 1;
        if (setsockopt(tcpServer->master_socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            printf("setsockopt(SO_REUSEADDR) failed");

        if (bind(tcpServer->master_socketFD, (sockaddr *) &hint, sizeof(hint)) != -1) {
            printf(GREEN "Bind!\n" RESET);
            if (listen(tcpServer->master_socketFD, SOMAXCONN) != -1) {
                tcpServer->handleServerListen();
            } else printf(RED "Cannot listen!\n" RESET);
        } else printf(RED "Cannot bind IP/Port!\n" RESET);
    } else printf(RED "Create socket failed!\n" RESET);
}

int ThreadedTCPServer::handleServerListen() {
    printf(GREEN "Listen!\n" RESET);
    sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);

    int max_fd, iResult;

    struct timeval tv;
    fd_set read_fds;

    printf(YELLOW "Try to accept! loop: %d\n" RESET, this->loop);
    while (this->loop) {
        tv.tv_sec = (long) this->acceptTimeout;
        tv.tv_usec = 0;

        FD_ZERO(&read_fds);
        FD_SET(this->master_socketFD, &read_fds);

        max_fd = this->master_socketFD;

        std::list<int>::iterator its;
        for (its = client_socket_fds.begin(); its != client_socket_fds.end(); ++its){
            int fd = *its;
            printf("Client fd: %d\n", fd);
            if (fd > 0) FD_SET(fd, &read_fds);
            if (fd > max_fd) max_fd = fd;
        }
        iResult = select(max_fd + 1, &read_fds, (fd_set *) 0, (fd_set *) 0, &tv);

        if (iResult > 0) {
            if (FD_ISSET(this->master_socketFD, &read_fds)) {
                int new_socket = accept(this->master_socketFD, (sockaddr *) &addr, &addrSize);

                if (new_socket != -1) {
                    printf("Accepted clientSocket!\n");
                    printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket,
                           inet_ntoa(addr.sin_addr), ntohs
                           (addr.sin_port));

                    (this->client_socket_fds).push_back(new_socket);
                }
            }
            char buffer[1025];


            std::list<int>::iterator it2 = (this->client_socket_fds).begin();
            while (it2 != (this->client_socket_fds).end()){
                int fd = *it2;
                printf("fd: %d\n", fd);


                if (FD_ISSET( fd , &read_fds))
                {
                    //Check if it was for closing , and also read the incoming message
                    int valread = read( fd , buffer, 1024);
                    printf("valread %d\n", valread);

                    if (valread == 0){
                        //Somebody disconnected , get his details and print
                        getpeername(fd , (struct sockaddr*)&addr , \
                        (socklen_t*)&addrSize);
                        printf("Host disconnected , ip %s , port %d \n" ,
                               inet_ntoa(addr.sin_addr) , ntohs(addr.sin_port));

                        //Close the socket and mark as 0 in list for reuse
                        close( fd );
                        it2 = (this->client_socket_fds).erase(it2);
                    }

                        //Echo back the message that came in
                    else{
                        //set the string terminating NULL byte on the end
                        //of the data read
                        printf("recv: %s\n", buffer);
                        buffer[valread] = '\0';
                        send(fd , buffer , strlen(buffer) , 0 );
                    }
                }
                it2++;
            }

        } else printf(YELLOW "Timeout: retry to accept! loop: %d\n" RESET, this->loop);
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
