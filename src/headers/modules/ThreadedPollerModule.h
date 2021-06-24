//
// Created by blt on 6/21/21.
//

#ifndef SIYAH1_THREADEDPOLLERMODULE_H
#define SIYAH1_THREADEDPOLLERMODULE_H

#include "ThreadedModule.h"
#include "../components/PollerStruct.h"

class ThreadedPollerModule : public ThreadedModule{

public:
    ThreadedPollerModule(int id);

    ~ThreadedPollerModule() override;

    bool start() override;

    bool stop() override;

    int register_handler(const struct PollerStruct& poller_str, const std::function<void()>& function_name);

    int unregister_handler(int fd_r);

protected:

    static void runModule(void *obj_param);

private:
    std::map<struct PollerStruct, std::function<void()>> pollMap;
    std::vector<struct pollfd> poll_fds;
    pthread_mutex_t _poller_mutex;
};


#endif //SIYAH1_THREADEDPOLLERMODULE_H
