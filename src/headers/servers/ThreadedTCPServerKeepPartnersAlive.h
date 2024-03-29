//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
#define SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H

#include "ThreadedTCPServer.h"


class ThreadedTCPServerKeepPartnersAlive : public ThreadedTCPServer {
public:
    ThreadedTCPServerKeepPartnersAlive(int id, int port, std::vector<struct Partner> partner_vec);

    bool start() override;

    bool stop() override;

    int handleReceivedString(std::string strRecv, int bytesRecv, int port) override;

    pid_t startPartner(const struct Partner& partner);

private:
    pthread_t _timer_thread;

    pthread_mutex_t _timer_mutex;

    static void runPartnerChecker(void *obj_param);

    int timer;

    std::map<pid_t, int> pidTimerMap;

    std::map<int, pid_t> portPidMap;

    std::vector<struct Partner> partners;

};


#endif //SIYAH1_THREADEDTCPSERVERKEEPPARTNERSALIVE_H
