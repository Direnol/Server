#ifndef SERVERFORESP_SERVER_H
#define SERVERFORESP_SERVER_H

#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <fstream>
#include <map>
#include <mutex>

#define BUF_SIZE 512

class Server {
private:
    volatile bool work;
    std::ofstream log_file;
    int _listner;
    struct sockaddr_in _addr;
    std::queue<std::string> qmsg;
    std::mutex qmutex;
    std::map<int, std::string> ID;
    std::vector<std::thread> threads;
    void log();

    void RecvMsg(int sock);
protected:
public:
    void CloseServer();
    Server(char *ip, uint16_t port, int max_clients);
    void openConnect();
    ~Server();
};

#endif //SERVERFORESP_SERVER_H
