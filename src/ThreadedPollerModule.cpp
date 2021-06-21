//
// Created by blt on 6/21/21.
//

#include "headers/modules/ThreadedPollerModule.h"

ThreadedPollerModule::ThreadedPollerModule(int id) : ThreadedModule(id) {}

int ThreadedPollerModule::register_handler(const struct PollerStruct& poller_str, const std::string& function_name) {
    return 0;
}

int ThreadedPollerModule::unregister_handler(int fd) {
    return 0;
}

ThreadedPollerModule::~ThreadedPollerModule() = default;
