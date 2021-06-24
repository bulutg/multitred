//
// Created by blt on 6/24/21.
//

#include "headers/servers/ThreadedPollerTCPServer.h"

ThreadedPollerTCPServer::~ThreadedPollerTCPServer() = default;

ThreadedPollerTCPServer::ThreadedPollerTCPServer(int id, int port) : ThreadedPollerModule(id) {
    this->port = port;
}

bool ThreadedPollerTCPServer::start() {
    return ThreadedPollerModule::start();
}

bool ThreadedPollerTCPServer::stop() {
    return ThreadedPollerModule::stop();
}
