//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "headers/servers/ThreadedTCPServerKeepPartnersAlive.h"


void ThreadedTCPServerKeepPartnersAlive::runPartnerChecker(void *obj_param) {
    ThreadedTCPServerKeepPartnersAlive *tcpServer = ((ThreadedTCPServerKeepPartnersAlive *) obj_param);
    bool restart = false;
    while(tcpServer->loop){
        for (auto & it : tcpServer->pidTimerMap){
            if (it.second > 500) {
                restart = true;
                break;
            }
        }
        if (restart || tcpServer->pidTimerMap.empty()){
            printf(RED "no response from partner, deploying partner again..\n" RESET);

            pthread_mutex_lock(&(tcpServer->_timer_mutex));

            for (auto & it : tcpServer->pidTimerMap){
                pid_t currentPid = it.first;
                printf(RED "Killing process %d\n" RESET, currentPid);
                kill(currentPid, SIGTERM);
            }

            // clean pid timer map
            (tcpServer->pidTimerMap).clear();

            for (int i = 0; i < tcpServer->number_of_partners; ++i) {
                pid_t currentPid = tcpServer->startPartner();
                (tcpServer->pidTimerMap).insert({currentPid, 0});
            }

            pthread_mutex_unlock(&(tcpServer->_timer_mutex));
        }

        sleep(1);
        pthread_mutex_lock(&(tcpServer->_timer_mutex));
        for (auto & it : tcpServer->pidTimerMap){
            it.second++;
        }
        pthread_mutex_unlock(&(tcpServer->_timer_mutex));
    }
}

ThreadedTCPServerKeepPartnersAlive::ThreadedTCPServerKeepPartnersAlive(int id, int port, std::string path, int partnerCount) : ThreadedTCPServer(id, port) {
    pthread_mutex_init(&(this->_timer_mutex), NULL);
    this->partner_executable_path = std::move(path);
    this->number_of_partners = partnerCount;
}

int ThreadedTCPServerKeepPartnersAlive::handleReceivedString(std::string strRecv, int bytesRecv) {
    strRecv.erase(std::remove(strRecv.begin(), strRecv.end(), '\n'), strRecv.end());
    printf(GREEN "KCA: Server Received: %s Loop %d\n" RESET, strRecv.c_str(), this->loop);

    if (strRecv == "heartbeat") {
        printf(GREEN "Partner is alive, received heartbeat!\n" RESET);
        pthread_mutex_lock(&(this->_timer_mutex));
        timer = 0;
        pthread_mutex_unlock(&(this->_timer_mutex));
    }

    return 0;
}

bool ThreadedTCPServerKeepPartnersAlive::start() {
    ThreadedTCPServer::start();
    return pthread_create(&_timer_thread, NULL, reinterpret_cast<void *(*)(void *)>(runPartnerChecker), this);
}

bool ThreadedTCPServerKeepPartnersAlive::stop() {
    ThreadedTCPServer::stop();
    (void) pthread_join(_timer_thread, nullptr);
    // TODO wait for child processes
    while(wait(NULL) > 0) {}
    return false;
}

pid_t ThreadedTCPServerKeepPartnersAlive::startPartner() {

    pid_t child_pid = fork();

    char* appName = const_cast<char *>((this->partner_executable_path).c_str());

    char* argv[] = {appName, NULL};

    if (child_pid == 0) {
        // deploy partner here
        execvp(appName, argv);
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    }
    else return child_pid;
}
