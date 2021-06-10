//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVERKEEPPARTNERALIVE_H
#define SIYAH1_THREADEDTCPSERVERKEEPPARTNERALIVE_H


#include "ThreadedTCPServer.h"

class ThreadedTCPServerKeepPartnerAlive : public ThreadedTCPServer {
public:
    ThreadedTCPServerKeepPartnerAlive(int id, int port, std::string path);

    bool start() override;

    bool stop() override;

    int handleReceivedString(std::string strRecv, int bytesRecv) override;

    bool startPartner();

private:
    pthread_t _timer_thread;

    pthread_mutex_t _timer_mutex;

    static void runPartnerChecker(void *obj_param);

    std::string partner_executable_path;

    pid_t child_pid;

    int timer;
};


#endif //SIYAH1_THREADEDTCPSERVERKEEPPARTNERALIVE_H
