#include <unistd.h>
#include <csignal>
#include <string>
#include "modules/ThreadedTCPServerKeepClientAlive.h"

ThreadedTCPServerKeepClientAlive* server;

sig_atomic_t volatile g_running = 1;

void handle(int signum){
    g_running = 0;
}

int main() {

    //variable to store calling function's process id
    pid_t process_id;
    //variable to store parent function's process id
    pid_t p_process_id;

    //getpid() - will return process id of calling function
    process_id = getpid();
    //getppid() - will return process id of parent function
    p_process_id = getppid();

    //printing the process ids
    printf("Veli process id: %d\n",process_id);
    printf("The process id of parent function: %d\n",p_process_id);

    struct sigaction sa{};
    sa.sa_handler = handle;
    sigaction(SIGINT, &sa, nullptr);

    server = new ThreadedTCPServerKeepClientAlive(0, 54002);

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
