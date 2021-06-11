//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "ThreadedTCPClient.h"

ThreadedTCPClient::ThreadedTCPClient(int id, int clientPort) : ThreadedModule(id), client_port(clientPort) {}

void ThreadedTCPClient::runClient(void *obj_param) {
    ThreadedTCPClient *tcpClient = ((ThreadedTCPClient *) obj_param);

//    struct timeval tv;
//    tv.tv_sec = (long) 5;
//    tv.tv_usec = 0;

    tcpClient->socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpClient->socketFD != -1) {
        printf(YELLOW "SOCKET CREATED!\n" RESET);
        sockaddr_in hint{};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(tcpClient->client_port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        std::string userInput = "heartbeat";

        // Connect to server

        do {

            if (connect(tcpClient->socketFD, (sockaddr *) &hint, sizeof(hint)) != -1) {
                printf(GREEN "Connected!\n" RESET);

                do {
//                std::cout << "> ";
//                getline(std::cin, userInput);

                    if (send(tcpClient->socketFD, userInput.c_str(), userInput.size() + 1, 0) != -1) {
                        // wait for response
                        memset(tcpClient->recv_buffer, 0, RECV_BUFFER_SIZE);

                        int bytesRecv = recv(tcpClient->socketFD, tcpClient->recv_buffer, RECV_BUFFER_SIZE, 0);
                        if (bytesRecv != -1) {

                            std::string strRecv = std::string(tcpClient->recv_buffer, bytesRecv);

                            printf("SERVER> %s\n", strRecv.c_str());
                        } else std::cerr << "There was a connecetion issue." << std::endl;
                    } else printf(RED "Cannot send to server!\n" RESET);
                    sleep(5);
                } while (tcpClient->loop);
            } else {
                printf(RED "Cannot connect to server! Trying again..., Loop: %d\n" RESET, tcpClient->loop);
                sleep(2);
                tcpClient->socketFD = socket(AF_INET, SOCK_STREAM, 0);
            }
        } while (tcpClient->loop);
    } else printf(RED "Create socket failed!\n" RESET);
}

bool ThreadedTCPClient::start() {
    ThreadedModule::start();
    return pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(runClient), this);
}

bool ThreadedTCPClient::stop() {
    printf("TCP Client Close Called.\n");
    close(this->socketFD);
    printf("Client socket closed.\n");
    return ThreadedModule::stop();
}