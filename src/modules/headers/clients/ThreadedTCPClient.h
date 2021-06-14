//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_THREADEDTCPCLIENT_H
#define SIYAH1_THREADEDTCPCLIENT_H

#include "../servers/ThreadedModule.h"

class ThreadedTCPClient : public ThreadedModule {
public:
    ThreadedTCPClient(int id, int clientPort);

    static void runClient(void *obj_param);

    bool start() override;

    bool stop() override;

protected:

    int client_port;

};


#endif //SIYAH1_THREADEDTCPCLIENT_H
