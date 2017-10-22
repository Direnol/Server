#include <csignal>
#include "Server.h"

void offserv(int sig);

Server *server = nullptr;


int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Use \"%s ip_addres port max_count_clients \"\n", argv[0]);
        return EXIT_FAILURE;
    }
    signal(SIGINT, offserv);
    server = new Server(argv[1], static_cast<uint16_t>(atoi(argv[2])), atoi(argv[3]));
    server->openConnect();
    delete (server);
}

void offserv(int sig)
{
    server->CloseServer();
}