//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "JsonThreadedTCPServer.h"

JsonThreadedTCPServer::JsonThreadedTCPServer(int id, int port) : ThreadedTCPServer(id, port) {
    this->modules[0] = nullptr;
    this->modules[1] = nullptr;
    this->curlyCount = 0;
}

void JsonThreadedTCPServer::setModules(ThreadedModule *module1, ThreadedModule *module2) {
    this->modules[0] = module1;
    this->modules[1] = module2;
}

int JsonThreadedTCPServer::triggerParseJsonDistribute(const std::string &jsonStr) {
    try {
//        std::ifstream ifs("./out/" + this->currentFileName + ".txt");
        json jsonRecv = json::parse(jsonStr);
        std::cout << std::setw(4) << jsonRecv << "\n\n";

        int moduleNo = jsonRecv["id"].get<int>();

        if (moduleNo == 1) this->modules[0]->send_message(jsonRecv["text"].get<std::string>());
        else if (moduleNo == 2) this->modules[1]->send_message(jsonRecv["text"].get<std::string>());
    }
    catch (json::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

int JsonThreadedTCPServer::filterJsonToRingBuffer(std::string strRecv, int bytesRecv) {
    for (int i = 0; i < bytesRecv - 1; ++i) {
        if (strRecv[i] == '{') {
//            if (curlyCount == 0) {
//                this->currentFileName = "Thread" + std::to_string(this->id) + "_" + time_stamp();
//                this->currentFileStream.open(("./out/" + currentFileName + ".txt").c_str());
//            }
            curlyCount++;
//            this->currentFileStream << strRecv[i];
            // cyclic buffer
            this->c_buffer.push(strRecv[i]);
        } else if (strRecv[i] == '}' && curlyCount != 0) {
            curlyCount--;
            if (curlyCount == 0) {
//                this->currentFileStream << strRecv[i];
                this->c_buffer.push(strRecv[i]);

                int count = this->c_buffer.getCount();

                std::string jsonStr;
                jsonStr.reserve(RING_BUFFER_SIZE);

                printf("size: %d", this->c_buffer.gsize());

                for (int j = 0; j < count; ++j) {
                    jsonStr.append(1, (this->c_buffer.pop()));
                }

//                printf(GREEN "End of JSON, Closing File Stream!\n" RESET);
//                this->currentFileStream.close();
                this->triggerParseJsonDistribute(jsonStr);
            }
        } else if (curlyCount >= 1) {
//            this->currentFileStream << strRecv[i];
            this->c_buffer.push(strRecv[i]);
        }
    }
    return 0;
}

int JsonThreadedTCPServer::handleReceivedString(std::string strRecv, int bytesRecv) {
    //clean \n
    strRecv.erase(std::remove(strRecv.begin(), strRecv.end(), '\n'), strRecv.end());
    printf(GREEN "Server Received: %s Loop %d\n" RESET, strRecv.c_str(), this->loop);

    this->filterJsonToRingBuffer(strRecv, bytesRecv);
    return 0;
}

