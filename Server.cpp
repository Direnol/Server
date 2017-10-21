#include "Server.h"

Server::Server(char *ip, uint16_t port, int max_clients)
{
    this->_listner = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listner < 0) {
        perror(nullptr);
        exit(errno);
    }
    int on = 1;
    setsockopt(this->_listner, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    this->_addr.sin_family = AF_INET; // space addr
    this->_addr.sin_port = htons(port); // port
    this->_addr.sin_addr.s_addr = inet_addr(ip);
    if (bind(this->_listner, (sockaddr *) &this->_addr, sizeof(this->_addr)) < 0) {
        perror(nullptr);
        exit(errno);
    }
    listen(this->_listner, max_clients);
    char log[28];
    sprintf(log, "%s:%d.log", ip, port);
    this->log_file.open(log);
    printf("Server is started, you have:\n"
                   "ip [%s]\n"
                   "port [%d]\n"
                   "max clients [%d]\n"
                   "log_file [%s]\n", ip, port, max_clients, log);
    pthread_mutex_init(&this->qmutex, nullptr);
    work = true;
    this->threads.emplace_back(std::thread(&Server::log, this));
    threads.back().detach();
}

Server::~Server()
{
    close(this->_listner);
    for (int i = 0; i < threads.size(); ++i) {
        threads[i].~thread();
    }
    printf("Server is finished.\n"
                   "ip [%s]\n"
                   "port [%d]", inet_ntoa(_addr.sin_addr), ntohs(this->_addr.sin_port));
}

void Server::log()
{
    std::cout << "Logger is on" << std::endl;
    while (work) {
        if (!qmsg.empty()) {
            pthread_mutex_lock(&qmutex);
            while (!qmsg.empty()) {
                log_file << qmsg.front() << std::endl;
                qmsg.pop();
            }
            pthread_mutex_unlock(&qmutex);
        }
    }
}

void Server::openConnect()
{
    int newconnect;
    socklen_t socklen = sizeof(this->_addr);
    sockaddr_in addr_client {};
    std::string nameConnect;
    while (work) {
        newconnect = accept(this->_listner, (sockaddr *) &addr_client, &socklen);
        if (newconnect < 0) {
            perror(nullptr);
            continue;
        }
        nameConnect = std::string(inet_ntoa(addr_client.sin_addr))
                      + std::string(":") + std::to_string(ntohs(addr_client.sin_port));
        printf("New connection: %s\n", nameConnect.c_str());
        ID[newconnect] = nameConnect;

        threads.emplace_back(&Server::recv, this, newconnect);
        threads.back().detach();
    }
}

void Server::recv(int sock)
{

}

