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
                printf(GREEN "Listen!\n" RESET);
                struct PollerStruct poll_struct = {.poll_fd = (tcpServer->master_socketFD), .poll_events=POLLIN};

                std::function<int(struct PollerStruct)> acceptSocketFunc = std::bind(
                        &ThreadedPollerTCPServer::newSocketAcceptFunction, tcpServer, std::placeholders::_1);

                tcpServer->register_handler(poll_struct, acceptSocketFunc);
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


int ThreadedPollerTCPServer::receiveFromSocketFunction(struct PollerStruct ps) {
    int fd = ps.poll_fd;
    sockaddr_in addr = ps.poll_addr;
    socklen_t addrSize = sizeof(addr);

    // Clear buffer
    memset(this->recv_buffer, 0, 4096);

    int bytesRecv = recv(fd, this->recv_buffer, RECV_BUFFER_SIZE, 0);

    printf("recv ret: %d\n", bytesRecv);

    getpeername(fd, (struct sockaddr *) &addr, (socklen_t *) &addrSize);

    if (bytesRecv == 0) {
        //getpeername(fd, (struct sockaddr *) &addr, (socklen_t *) &addrSize);
        //Somebody disconnected , get his details and print
        printf(RED "Host disconnected , ip %s , port %d \n" RESET,
               inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close(fd);
        //TODO
        //it2 = (this->client_socket_fds).erase(it2);
    } else {
        std::string strRecv = std::string(this->recv_buffer, 0, bytesRecv);

        this->handleReceivedString(strRecv, bytesRecv, ntohs(addr.sin_port));
        //resend msg (echo)
        send(fd, this->recv_buffer, bytesRecv + 1, 0);
    }
    return 0;
}

int ThreadedPollerTCPServer::newSocketAcceptFunction(struct PollerStruct ps) {
    int fd = ps.poll_fd;
    sockaddr_in addr = ps.poll_addr;
    socklen_t addrSize = sizeof(addr);

    printf(GREEN "IN SAMPLE FUNC MMM\n" RESET);

    int new_socket = accept(fd, (sockaddr *) &addr, &addrSize);

    if (new_socket != -1) {
        printf(GREEN "New connection , socket fd is %d , ip is : %s , port : %d\n" RESET, new_socket,
               inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

        struct PollerStruct poll_struct = {.poll_fd = new_socket, .poll_events=POLLIN, .poll_addr = addr};

        std::function<int(struct PollerStruct)> recvFunc = std::bind(&ThreadedPollerTCPServer::receiveFromSocketFunction, this, std::placeholders::_1);

        this->register_handler(poll_struct, recvFunc);
    }
    return 0;
}
