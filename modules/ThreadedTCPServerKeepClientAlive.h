//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVERKEEPCLIENTALIVE_H
#define SIYAH1_THREADEDTCPSERVERKEEPCLIENTALIVE_H


#include "ThreadedTCPServer.h"

class ThreadedTCPServerKeepClientAlive : public ThreadedTCPServer {
public:
    ThreadedTCPServerKeepClientAlive(int id, int port, std::string path);

    bool start() override;

    bool stop() override;

    int handleReceivedString(std::string strRecv, int bytesRecv) override;

    bool startClient();

private:
    pthread_t _timer_thread;

    pthread_mutex_t _timer_mutex;

    static void runClientChecker(void *obj_param);

    std::string partner_executable_path;

    pid_t child_pid;

    int timer;
};


#endif //SIYAH1_THREADEDTCPSERVERKEEPCLIENTALIVE_H
