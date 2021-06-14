//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
#define SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H


#include "ThreadedTCPServer.h"

class ThreadedTCPServerKeepPartnersAlive : public ThreadedTCPServer {
public:
    ThreadedTCPServerKeepPartnersAlive(int id, int port, std::string path, int partnerCount);

    bool start() override;

    bool stop() override;

    int handleReceivedString(std::string strRecv, int bytesRecv) override;

    pid_t startPartner();

private:
    pthread_t _timer_thread;

    pthread_mutex_t _timer_mutex;

    static void runPartnerChecker(void *obj_param);

    std::string partner_executable_path;

    int timer;

    int number_of_partners;

    std::map<pid_t, int> pidTimerMap;

    std::map<pid_t, int> pidPortMap;

};


#endif //SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
