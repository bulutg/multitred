//
// Created by Bulut Gözübüyük on 1.06.2021.
//

#ifndef SIYAH1_THREADEDTCPSERVER_H
#define SIYAH1_THREADEDTCPSERVER_H

#include "../modules/ThreadedModule.h"


class ThreadedTCPServer : public ThreadedModule {
public:
    explicit ThreadedTCPServer(int id, int port);

    bool start() override;

    bool stop() override;

    static void runServer(void *obj_param);

    ~ThreadedTCPServer() override;

    virtual int handleReceivedString(std::string strRecv, int bytesRecv, int port);

private:

    int handleServerListen();

    int acceptTimeout;

    std::string currentFileName;

    std::list<int> client_socket_fds;
protected:
    int server_port;
};


#endif //SIYAH1_THREADEDTCPSERVER_H
