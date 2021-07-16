#include <unistd.h>
#include <csignal>
#include <string>
#include "src/headers/servers/ThreadedTCPServerKeepPartnersAlive.h"

ThreadedTCPServerKeepPartnersAlive* server;

sig_atomic_t volatile g_running = 1;

void handle(int signum){
    g_running = 0;
}

int main(int argc,char* argv[]) {
    //variable to store calling function's process id
    pid_t process_id;
    //variable to store parent function's process id
    pid_t p_process_id;
    //getpid() - will return process id of calling function
    process_id = getpid();
    //getppid() - will return process id of parent function
    p_process_id = getppid();

    //printing the process ids
    printf("Supervisor: process id: %d\n",process_id);
    printf("Supervisor: process id of parent function: %d\n",p_process_id);

    struct sigaction sa{};
    sa.sa_handler = handle;
    sigaction(SIGINT, &sa, nullptr);

    std::vector<struct Partner> partner_vec;

    struct Partner p1, p2, p3;

    p1 = {.exec_str = "/home/blt/CLionProjects/tredmred/Partner", .param = ""};
    p2 = {.exec_str = "/home/blt/CLionProjects/tredmred/Partner", .param = ""};
    p3 = {.exec_str = "/home/blt/CLionProjects/tredmred/Partner", .param = ""};

    partner_vec.push_back(p1);
    partner_vec.push_back(p2);
    partner_vec.push_back(p3);

    server = new ThreadedTCPServerKeepPartnersAlive(0, 54112, partner_vec);

    server->start();

    while (g_running) pause();
    printf("exiting safely from while loop\n");

    server->stop();
    printf("--s1 stopped\n");

    printf("--will del pointers\n");
    delete server;
    printf("--deleted pointers\n");

//    pthread_exit(NULL);
    return 0;
}
