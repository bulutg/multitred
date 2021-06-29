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

    static void runServer(void *obj_param);

    virtual int handleReceivedString(std::string strRecv, int bytesRecv, int port);

    int receiveFromSocketFunction(struct PollerStruct ps);

    int newSocketAcceptFunction(struct PollerStruct ps);

private:

    int handleServerListen();

    int acceptTimeout;

    std::string currentFileName;

    std::list<int> client_socket_fds;

protected:
    int server_port;
};


#endif //SIYAH1_THREADEDPOLLERTCPSERVER_H
