//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
#define SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H


#include "ThreadedTCPServer.h"

class ThreadedTCPServerKeepPartnersAlive : public ThreadedTCPServer {
public:
    ThreadedTCPServerKeepPartnersAlive(int id, int port, std::string path);

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

    std::vector<pid_t> partner_pids;
    std::vector<int> partner_timers;

};


#endif //SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
