//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#ifndef SIYAH1_JSONTHREADEDTCPSERVER_H
#define SIYAH1_JSONTHREADEDTCPSERVER_H

#include "ThreadedTCPServer.h"

class JsonThreadedTCPServer : public ThreadedTCPServer {

public:
    JsonThreadedTCPServer(int id, int port);

    void setModules(ThreadedModule *module1, ThreadedModule *module2);

private:

    ThreadedModule *modules[2]{};

    int curlyCount;

    int triggerParseJsonDistribute(const std::string& jsonStr);

    int filterJsonToRingBuffer(std::string strRecv, int bytesRecv);


public:
    int handleReceivedString(std::string strRecv, int bytesRecv, int port) override;

};


#endif //SIYAH1_JSONTHREADEDTCPSERVER_H
