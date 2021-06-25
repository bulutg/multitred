//
// Created by blt on 6/24/21.
//

#include "headers/servers/ThreadedPollerTCPServer.h"

ThreadedPollerTCPServer::~ThreadedPollerTCPServer() = default;

ThreadedPollerTCPServer::ThreadedPollerTCPServer(int id, int port) : ThreadedPollerModule(id) {
    this->server_port = port;
}

bool ThreadedPollerTCPServer::start() {
    ThreadedPollerModule::start();
    return pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(runServer), this);
}

bool ThreadedPollerTCPServer::stop() {
    printf("TCP Server Close Called.\n");
    //TODO CHANGE FD
    std::list<int>::iterator close_it;
    for (close_it = client_socket_fds.begin(); close_it != client_socket_fds.end(); ++close_it) {
        int fd = *close_it;
        close(fd);
    }
    printf("Client sockets closed.\n");
    return ThreadedPollerModule::stop();
}

void ThreadedPollerTCPServer::runServer(void *obj_param) {
    ThreadedPollerTCPServer *tcpServer = ((ThreadedPollerTCPServer *) obj_param);
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

int ThreadedPollerTCPServer::handleReceivedString(std::string strRecv, int bytesRecv, int port) {
    //clean \n
    strRecv.erase(std::remove(strRecv.begin(), strRecv.end(), '\n'), strRecv.end());
    printf(GREEN "Server Received: %s Loop %d\n" RESET, strRecv.c_str(), this->loop);

    return 0;
}

int ThreadedPollerTCPServer::handleServerListen() {
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
        for (its = client_socket_fds.begin(); its != client_socket_fds.end(); ++its) {
            int fd = *its;
            printf(GREEN " %d " RESET, fd);
            if (fd > 0) FD_SET(fd, &read_fds);
            if (fd > max_fd) max_fd = fd;
        }
        printf(YELLOW"<<< Client fds \n" RESET);
        iResult = select(max_fd + 1, &read_fds, nullptr, nullptr, &tv);

        if (iResult > 0) {
            if (FD_ISSET(this->master_socketFD, &read_fds)) {
                int new_socket = accept(this->master_socketFD, (sockaddr *) &addr, &addrSize);

                if (new_socket != -1) {
                    printf(GREEN "New connection , socket fd is %d , ip is : %s , port : %d\n" RESET, new_socket,
                           inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

                    //(this->client_socket_fds).push_back(new_socket);

                    struct PollerStruct poll_struct = {.poll_fd = new_socket, .poll_events=POLLIN, .poll_addr = addr};

                    std::function<int(struct PollerStruct)> sampleFunc(std::bind(&ThreadedPollerTCPServer::sampleFunction, this, poll_struct));

                    this->register_handler(poll_struct, sampleFunc);
                }
            }
            int fd = 0;

            if (FD_ISSET(fd, &read_fds)) {

            }
        } else printf(YELLOW "Timeout: retry to accept! loop: %d\n" RESET, this->loop);
    }
    return 0;
}
