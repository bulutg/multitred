//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "ThreadedTCPServerKeepPartnersAlive.h"


void ThreadedTCPServerKeepPartnersAlive::runPartnerChecker(void *obj_param) {
    ThreadedTCPServerKeepPartnersAlive *tcpServer = ((ThreadedTCPServerKeepPartnersAlive *) obj_param);
    while(tcpServer->loop){
        if (tcpServer->timer >= 11){
            printf(RED "no response from partner, deploying partner again..\n" RESET);

            tcpServer->startPartner();

            pthread_mutex_lock(&(tcpServer->_timer_mutex));
            (tcpServer->timer) = 6;
            pthread_mutex_unlock(&(tcpServer->_timer_mutex));
        }
        sleep(1);
        pthread_mutex_lock(&(tcpServer->_timer_mutex));
        (tcpServer->timer)++;
        pthread_mutex_unlock(&(tcpServer->_timer_mutex));
    }
}

ThreadedTCPServerKeepPartnersAlive::ThreadedTCPServerKeepPartnersAlive(int id, int port, std::string path) : ThreadedTCPServer(id, port) {
    pthread_mutex_init(&(this->_timer_mutex), NULL);
    this->timer = 0;
    this->partner_executable_path = std::move(path);
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
    waitpid(this->child_pid, NULL, 0);
    return false;
}

bool ThreadedTCPServerKeepPartnersAlive::startPartner() {

    this->child_pid = fork();

    char* appName = const_cast<char *>((this->partner_executable_path).c_str());

    char* argv[] = {appName, NULL};

    if (child_pid == 0) {
        // add pid to list
        partner_pids.push_back(child_pid);

        // deploy partner here
        execvp(appName, argv);
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    }

    return false;
}