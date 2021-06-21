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

    int register_handler(const struct PollerStruct& poller_str, const std::string& function_name);

    int unregister_handler(int fd);


};


#endif //SIYAH1_THREADEDPOLLERMODULE_H
