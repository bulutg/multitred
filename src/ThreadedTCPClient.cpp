//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "headers/clients/ThreadedTCPClient.h"

ThreadedTCPClient::ThreadedTCPClient(int id, int clientPort) : ThreadedModule(id), client_port(clientPort) {}

void ThreadedTCPClient::runClient(void *obj_param) {
    ThreadedTCPClient *tcpClient = ((ThreadedTCPClient *) obj_param);
    pid_t process_id = getpid();

    tcpClient->master_socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpClient->master_socketFD != -1) {
        std::string alert = "Client:" + std::to_string(process_id) + " SOCKET CREATED!\n";
        printf(YELLOW);
        printf("%s", alert.c_str());
        printf(RESET);
        sockaddr_in hint{};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(tcpClient->client_port);
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

        //variable to store calling function's process id

        std::string client_pid = std::to_string(process_id);


        std::string userInput = "heartbeat";

        // Connect to server

        do {
            tcpClient->connected = connect(tcpClient->master_socketFD, (sockaddr *) &hint, sizeof(hint));
            if (tcpClient->connected != -1) {
                printf(GREEN "Client: Connected!\n" RESET);

                do {
                    if (send(tcpClient->master_socketFD, userInput.c_str(), userInput.size() + 1, 0) != -1) {

                        // wait for response
                        memset(tcpClient->recv_buffer, 0, RECV_BUFFER_SIZE);

                        int bytesRecv = recv(tcpClient->master_socketFD, tcpClient->recv_buffer, RECV_BUFFER_SIZE, 0);
                        if (bytesRecv != -1) {

                            std::string strRecv = std::string(tcpClient->recv_buffer, bytesRecv);

                            printf(GREEN "Client%sReceived> %s\n" RESET, client_pid.c_str(), strRecv.c_str());
                        } else printf(RED "Client%s there was a connection issue\n" RESET, client_pid.c_str());
                    } else printf(RED "Client%s: Cannot send to server!\n" RESET, client_pid.c_str());
                    sleep(5);
                } while (tcpClient->loop);
            } else {
                printf(RED "Client: Cannot connect to server! Trying again..., Loop: %d\n" RESET, tcpClient->loop);
                sleep(2);
                tcpClient->master_socketFD = socket(AF_INET, SOCK_STREAM, 0);
            }
        } while (tcpClient->loop);
    } else printf(RED "Client: Create socket failed!\n" RESET);
}

bool ThreadedTCPClient::start() {
    ThreadedModule::start();
    return pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(runClient), this);
}

bool ThreadedTCPClient::stop() {
    printf("TCP Client Close Called.\n");
    ThreadedModule::stop();

    if (this->master_socketFD != -1) {
        std::string userInput = "dead";

        if (this->connected != -1) {
            printf(GREEN "Client: Connected!\n" RESET);
            send(this->master_socketFD, userInput.c_str(), userInput.size() + 1, 0);
        }
    }
    return close(this->master_socketFD);
}
