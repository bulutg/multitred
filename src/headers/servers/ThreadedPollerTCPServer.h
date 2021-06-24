//
// Created by blt on 6/24/21.
//

#ifndef SIYAH1_THREADEDPOLLERTCPSERVER_H
#define SIYAH1_THREADEDPOLLERTCPSERVER_H


#include "../modules/ThreadedPollerModule.h"

class ThreadedPollerTCPServer : public ThreadedPollerModule{
public:
    ThreadedPollerTCPServer(int id, int port);

    ~ThreadedPollerTCPServer() override;

    bool start() override;

    bool stop() override;

private:
    int port;
};


#endif //SIYAH1_THREADEDPOLLERTCPSERVER_H
