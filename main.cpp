#include "Server.h"

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Use \"%s ip_addres port max_count_clients log_file\"", argv[0]);
        return EXIT_FAILURE;
    }
    Server server(argv[1], static_cast<uint16_t>(atoi(argv[2])), atoi(argv[3]));
    sleep(3);
}
