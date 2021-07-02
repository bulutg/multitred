#include <unistd.h>
#include <csignal>
#include <string>
#include "src/headers/servers/JsonThreadedTCPServer.h"
#include "src/headers/clients/ThreadedTCPClient.h"

//JsonThreadedTCPServer* server;
ThreadedTCPClient* tcp_client;
//ThreadedModule* client1;
//ThreadedModule* client2;

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

//    server = new JsonThreadedTCPServer(0, 54001);
//    client1 = new ThreadedModule(1);
//    client2 = new ThreadedModule(2);

    tcp_client = new ThreadedTCPClient(3, 54111);

//    server->start();
//    client1->start();
//    client2->start();
//
//    server->setModules(client1, client2);

    tcp_client->start();

    while (g_running) pause();
    printf(YELLOW "exiting safely from while loop\n" RESET);

    tcp_client->stop();

//    server->stop();
//    printf("--s1 stopped\n");
//    client1->stop();
//    printf("--c1 stopped\n");
//    client2->stop();
//    printf("--c2 stopped\n");
//
//    printf("--will del pointers\n");
//    delete server;
//    delete client1;
//    delete client2;
    delete tcp_client;
    printf("--deleted pointers\n");

//    pthread_exit(NULL);
    return 0;
}
