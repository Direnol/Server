#include <signal.h>
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
    this->_addr.sin_family = AF_INET;
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
    work = true;
    this->threads.emplace_back(std::thread(&Server::log, this));
}

Server::~Server()
{
    printf("Server is finished.\n"
                   "ip [%s]\n"
                   "port [%d]\n", inet_ntoa(_addr.sin_addr), ntohs(this->_addr.sin_port));
}

void Server::log()
{
    std::cout << "Logger is on" << std::endl;
    while (work) {
        if (!qmsg.empty()) {
            qmutex.lock();
            while (!qmsg.empty()) {
                log_file << qmsg.front() << std::endl;
                qmsg.pop();
            }
            qmutex.unlock();
        }
    }
    std::cout << "Logger if off" << std::endl;
}

void Server::openConnect()
{
    int newconnect = 0;
    socklen_t socklen = sizeof(this->_addr);
    sockaddr_in addr_client {};
    std::string nameConnect;
    while (work) {
        newconnect = accept(this->_listner, (sockaddr *) &addr_client, &socklen);
        if (newconnect <= 0) {
            if (errno == EBADF) break;
            perror(nullptr);
            continue;
        }
        nameConnect = std::string(inet_ntoa(addr_client.sin_addr))
                      + std::string(":") + std::to_string(ntohs(addr_client.sin_port));
        printf("New connection: %s\n", nameConnect.c_str());
        ID[newconnect] = nameConnect;

        threads.emplace_back(&Server::RecvMsg, this, newconnect);
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

void Server::RecvMsg(int sock)
{
    int fd = sock;
    std::cout << "Client with " << this->ID[fd] << " has been conected" << std::endl;
    std::vector<char> buf(BUF_SIZE);
    std::string msg;
    ssize_t res;
    while (work) {
        res = recv(fd, buf.data(), buf.size(), 0);
        if (res <= 0) {
            perror(nullptr);
            break;
        }
        qmutex.lock();
        qmsg.push(ID[fd] + ":" + (msg.assign(buf.data(), static_cast<unsigned long>(res))));
        std::cout << qmsg.back() << std::endl;
        qmutex.unlock();

    }
    std::cout << "Client with " << ID[fd] << " has been disconected" << std::endl;
    close(fd);
    ID.erase(fd);
}

void Server::CloseServer()
{
    std::cout << "Server start finish" << std::endl;
    close(this->_listner);
    work = false;
}

