//
// Created by blt on 6/24/21.
//

#include <unistd.h>
#include <csignal>
#include <string>
#include "src/headers/servers/ThreadedPollerTCPServer.h"

ThreadedPollerTCPServer* server;

sig_atomic_t volatile g_running = 1;

void handle(int signum){
    g_running = 0;
}

int main(int argc,char* argv[]) {
    pid_t process_id;
    pid_t p_process_id;
    process_id = getpid();
    p_process_id = getppid();

    //printing the process ids
    printf("Supervisor: process id: %d\n",process_id);
    printf("Supervisor: process id of parent function: %d\n",p_process_id);

    struct sigaction sa{};
    sa.sa_handler = handle;
    sigaction(SIGINT, &sa, nullptr);

    server = new ThreadedPollerTCPServer(0, 54111);

    server->start();

    while (g_running) pause();
    printf("exiting safely from while loop\n");

    server->stop();
    printf("--s1 stopped\n");

    printf("--will del pointers\n");
    delete server;
    printf("--deleted pointers\n");
    return 0;
}
