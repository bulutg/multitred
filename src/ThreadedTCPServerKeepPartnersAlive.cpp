//
// Created by Bulut Gözübüyük on 8.06.2021.
//

#include "headers/servers/ThreadedTCPServerKeepPartnersAlive.h"

void ThreadedTCPServerKeepPartnersAlive::runPartnerChecker(void *obj_param) {
    ThreadedTCPServerKeepPartnersAlive *tcpServer = ((ThreadedTCPServerKeepPartnersAlive *) obj_param);
    bool restart;
    while (tcpServer->loop) {
        restart = false;
        for (auto &it : tcpServer->pidTimerMap) {
            if (it.second > 12) {
                printf(RED "no response from partner pid: %d\n" RESET, it.first);
                restart = true;
                break;
            }
        }
        if (restart || tcpServer->pidTimerMap.empty()) {
            printf(RED "no response from partner, deploying partner again..\n" RESET);

            pthread_mutex_lock(&(tcpServer->_timer_mutex));

            for (auto &it : tcpServer->pidTimerMap) {
                pid_t currentPid = it.first;
                printf(RED "Killing process %d\n" RESET, currentPid);
                kill(currentPid, SIGTERM);
            }

            // clean pid timer and port map
            (tcpServer->pidTimerMap).clear();
            (tcpServer->portPidMap).clear();

            for (auto &p : tcpServer->partners) {
                pid_t currentPid = tcpServer->startPartner(p);
                (tcpServer->pidTimerMap).insert({currentPid, 0});
            }

            sleep(1);

            std::vector<std::string> vect;

            vect.push_back("sh");
            vect.push_back("parsed_ss.sh");

            std::string result = execGetOutput(vect);

            std::string colon = ":";
            std::string space = " ";
            std::string pid = "pid=";
            std::string comma = ",";
            printf(GREEN "PARSED PORT AND PID\n" RESET);

            std::string s;
            std::istringstream f(result);

            while (getline(f, s)) {
                std::string prt = s.substr(s.find(colon) + 1, s.find(space) - s.find(colon) - 1);
                s.erase(0, s.find(pid) + pid.length());
                std::string pidno = s.substr(0, s.find(comma));
                printf(RED "Port> %s matches Pid> %s \n" RESET, prt.c_str(), pidno.c_str());
                (tcpServer->portPidMap).insert({std::stoi(prt), std::stoi(pidno)});
            }

            pthread_mutex_unlock(&(tcpServer->_timer_mutex));
        }

        sleep(1);
        pthread_mutex_lock(&(tcpServer->_timer_mutex));
        for (auto &it : tcpServer->pidTimerMap) {
            it.second++;
        }
        pthread_mutex_unlock(&(tcpServer->_timer_mutex));
    }
}

ThreadedTCPServerKeepPartnersAlive::ThreadedTCPServerKeepPartnersAlive(int id, int port,
                                                                       std::vector<struct Partner> partner_vec)
        : ThreadedTCPServer(id, port) {
    pthread_mutex_init(&(this->_timer_mutex), NULL);
    this->partners = std::move(partner_vec);
}

int ThreadedTCPServerKeepPartnersAlive::handleReceivedString(std::string strRecv, int bytesRecv, int port) {
    strRecv.erase(std::remove(strRecv.begin(), strRecv.end(), '\n'), strRecv.end());
    printf(GREEN "KCA: Server Received: %s Loop %d\n" RESET, strRecv.c_str(), this->loop);

    if (strRecv == "heartbeat") {
        printf(GREEN "Partner is alive, received heartbeat from port %d!\n" RESET, port);
        pthread_mutex_lock(&(this->_timer_mutex));

        std::_Rb_tree_iterator<std::pair<const int, int>> foundPid = this->portPidMap.find(port);

        if (foundPid != this->portPidMap.end()) {
            //reset timer
            std::_Rb_tree_iterator<std::pair<const int, int>> foundTimer = this->pidTimerMap.find(foundPid->second);
            if (foundTimer != this->pidTimerMap.end()) {
                //reset timer
                foundTimer->second = 0;
            }
        }

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
    while (wait(NULL) > 0) {}
    return false;
}

pid_t ThreadedTCPServerKeepPartnersAlive::startPartner(const struct Partner &partner) {
    pid_t child_pid = fork();

    char *appName = const_cast<char *>((partner.exec_str).c_str());
    char *argv[] = {appName, NULL};

    if (child_pid == 0) {
        // deploy partner here
        execvp(appName, argv);
        fprintf(stderr, "an error occurred in execvp\n");
        abort();
    } else return child_pid;
}
