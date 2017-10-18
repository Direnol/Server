//
// Created by direnol on 18.10.17.
//

#ifndef SERVERFORESP_SERVER_H
#define SERVERFORESP_SERVER_H

#include <thread>
#include <netinet/in.h>

class Server {
private:
    int _listner;
    struct sockaddr_in _addr;
    std::thread _open_conet;
    std::thread _recv;
    char *buf_msg;
    void Log();
    void RecvMessage();
    void OpenConnection();
public:
    Server(char *ip, uint16_t port);
};

#endif //SERVERFORESP_SERVER_H
