//
// Created by Bulut Gözübüyük on 31.05.2021.
//

#include "headers/modules/ThreadedModule.h"

ThreadedModule::ThreadedModule(int id) : id(id), c_buffer(RING_BUFFER_SIZE) {
    this->loop = 1;
    this->timeout = 50000;
}

bool ThreadedModule::start() {
    loop = 1;
    return (pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(&ThreadedModule::run), this) == 0);
}

void ThreadedModule::run(void *obj_param) {
    ThreadedModule *module = ((ThreadedModule *) obj_param);

    std::string msg;
    while (module->loop) {
        msg = module->moduleQueue.get(module->timeout);
        if (msg.empty()) module->handle_timeout();
        else module->handle_message(msg);
    }
}

bool ThreadedModule::stop() {
    printf("Thread %d stop called\n", this->id);
    this->trigger_exit();
    printf("Thread %d triggered exit\n", this->id);
    (void) pthread_join(_thread, nullptr);
    return false;
}

std::string ThreadedModule::handle_message(std::string message) {
    printf("Thread %d received msg: %s, current loop is %d\n", this->id, message.c_str(), this->loop);
    if (message == "exit") {
        printf(RED "Thread %d exiting\n" RESET, this->id);
        this->loop = 0;
    }
    return message;
}

void ThreadedModule::send_message(const std::string &message) {
    this->moduleQueue.enqueue(message);
}

void ThreadedModule::trigger_exit() {
    if (this->loop == 1) {
        this->send_message("exit");
        printf(YELLOW "exit message sent thread: %d\n" RESET, this->id);
    } else printf(RED "Thread: %d is not running, unable to trigger exit\n" RESET, this->id);
}

int ThreadedModule::is_running() const {
    return this->loop;
}

// if queue empty
void ThreadedModule::handle_timeout() {
//    return this->trigger_exit();
}

ThreadedModule::~ThreadedModule() = default;

