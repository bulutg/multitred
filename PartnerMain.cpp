#include <unistd.h>
#include <csignal>
#include <string>
#include "src/headers/clients/ThreadedTCPClient.h"

ThreadedTCPClient* tcp_client;

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
    printf(GREEN "Partner process id: %d\n" RESET,process_id);
//    printf("Partner: The process id of parent function: %d\n",p_process_id);

    struct sigaction sa{};
    sa.sa_handler = handle;
    sigaction(SIGINT, &sa, nullptr);

    tcp_client = new ThreadedTCPClient(3, 54111);

    tcp_client->start();

    while (g_running) pause();

    printf(YELLOW "exiting safely from while loop\n" RESET);

    tcp_client->stop();

    delete tcp_client;
    printf("--deleted pointers\n");

//    pthread_exit(NULL);
    return 0;
}
