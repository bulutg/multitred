//
// Created by Bulut Gözübüyük on 31.05.2021.
//

#include <pthread.h>
#include <queue>
#include <string>
#include <iostream>
#include <utility>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <sys/wait.h>
#include <list>
#include <csignal>
#include <sys/poll.h>


#include "../components/SafeQueue.h"
#include "../components/CircularBuffer.h"
#include "../../../lib/json.hpp"

#ifndef SIYAH1_THREADEDMODULE_H
#define SIYAH1_THREADEDMODULE_H

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RED     "\033[31m"      /* Red */

#define RING_BUFFER_SIZE 10000
#define RECV_BUFFER_SIZE 4096

using json = nlohmann::json;

class ThreadedModule {
public:
    int id;

    virtual bool start();

    virtual bool stop();

    int is_running() const;

    void send_message(const std::string &message);

    SafeQueue<std::string> moduleQueue;

    explicit ThreadedModule(int id);

    virtual ~ThreadedModule();

private:
    void trigger_exit();

    static void run(void *obj_param);

    int timeout;

    void handle_timeout();

    std::string handle_message(std::string message);

protected:
    int loop;

    int master_socketFD;

    pthread_t _thread;

    CircularBuffer c_buffer;

    char recv_buffer[4096];

};

#endif //SIYAH1_THREADEDMODULE_H
