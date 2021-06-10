#include <unistd.h>
#include <csignal>
#include <string>
#include "modules/ThreadedTCPServerKeepPartnerAlive.h"

ThreadedTCPServerKeepPartnerAlive* server;

sig_atomic_t volatile g_running = 1;

void handle(int signum){
    g_running = 0;
}

int main(int argc,char* argv[]) {
    if (argc != 3) {
        printf("Supervisor <N> <pX>\n");
        printf("<N> is the number of Partners\n");
        printf("<pX> path of Partner executable\n");
        return 0;
    }

    int N = atoi(argv[1]);

    std::string partner_executable_path = argv[2];

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

    server = new ThreadedTCPServerKeepPartnerAlive(0, 54002, partner_executable_path);

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
