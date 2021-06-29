//
// Created by blt on 6/21/21.
//
#include "headers/modules/ThreadedPollerModule.h"

#define TIMEOUT 5

ThreadedPollerModule::ThreadedPollerModule(int id) : ThreadedModule(id) {
    pthread_mutex_init(&(this->_poller_mutex), NULL);
}

int ThreadedPollerModule::register_handler(const struct PollerStruct &poller_str, std::function<int(struct PollerStruct)> function_name) {
    struct pollfd new_pollfd{};
    new_pollfd.fd = poller_str.poll_fd;
    new_pollfd.events = poller_str.poll_events;

    pthread_mutex_lock(&(this->_poller_mutex));
    this->poll_fds.push_back(new_pollfd);
    (this->pollMap).insert({poller_str, function_name} );
    pthread_mutex_unlock(&(this->_poller_mutex));
    return 0;
}

int ThreadedPollerModule::unregister_handler(int fd_r) {
    std::vector<struct pollfd>::iterator it;

    pthread_mutex_lock(&(this->_poller_mutex));

    for(it = this->poll_fds.begin(); it != this->poll_fds.end();){
        if(it->fd == fd_r) {
            this->poll_fds.erase(it);
            break;
        }
        else it++;
    }

    auto mit = this->pollMap.cbegin();

    while (mit != this->pollMap.end()) {
        if (mit->first.poll_fd == fd_r) {
            this->pollMap.erase(mit);
            break;
        } else ++mit;
    }

    pthread_mutex_unlock(&(this->_poller_mutex));
    return 0;
}

bool ThreadedPollerModule::start() {
    ThreadedModule::start();
    return pthread_create(&_thread, NULL, reinterpret_cast<void *(*)(void *)>(runModule), this);
}

bool ThreadedPollerModule::stop() {
    return ThreadedModule::stop();
}

void ThreadedPollerModule::runModule(void *obj_param) {

    ThreadedPollerModule *module = ((ThreadedPollerModule *) obj_param);
    int ret;

    while (module->loop) {
       // printf(RED "BEFORE POLL\n" RESET);

    //    for (int i = 0; i < (module->poll_fds).size(); i++) printf(RED "fd%d event%d revent%d\n" RESET, (module->poll_fds)[i].fd, (module->poll_fds)[i].events, (module->poll_fds)[i].revents);
        pthread_mutex_lock(&(module->_poller_mutex));

        pollfd* poll_arr = module->poll_fds.data();
        ret = poll(poll_arr, (module->poll_fds).size(), TIMEOUT * 1000);
        pthread_mutex_unlock(&(module->_poller_mutex));
        if (ret == -1) {
            printf(RED "poll err\n" RESET);
        }
        if (!ret) {
            printf(RESET"%d seconds elapsed.\n" RESET, TIMEOUT);
        }
        for (auto outer_iterator = module->pollMap.cbegin(); outer_iterator != module->pollMap.cend();){
            auto inner_iterator = find_if(begin(module->poll_fds), end(module->poll_fds), [=] (struct pollfd const& f) {
                return (f.fd == outer_iterator->first.poll_fd);
            });
            printf(GREEN "%d\n" RESET,(inner_iterator->revents & outer_iterator->first.poll_events));
            if (inner_iterator != end(module->poll_fds)) if (inner_iterator->revents & outer_iterator->first.poll_events){
                outer_iterator->second(outer_iterator->first);
                //module->pollMap.erase(outer_iterator++);
                //continue;
            }
            ++outer_iterator;
        }
       // printf(GREEN "After POLL\n" RESET);

       // for (int i = 0; i < (module->poll_fds).size(); i++)printf(RED "fd%d event%d revent%d\n" RESET, (module->poll_fds)[i].fd, (module->poll_fds)[i].events, (module->poll_fds)[i].revents);
        sleep(1);
    }

}


void ThreadedPollerModule::handleReceivedString(std::string basicString, int i, uint16_t i1) {

}

ThreadedPollerModule::~ThreadedPollerModule() = default;
