//
// Created by direnol on 18.10.17.
//

#include <arpa/inet.h>
#include <iostream>
#include "Server.h"

Server::Server(char *ip, uint16_t port)
{
    this->_listner = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_listner < 0) {
        exit(15);
    }
    int on = 1;
    setsockopt(this->_listner, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    this->_addr.sin_family = AF_INET; // space addr
    this->_addr.sin_port = htons(port); // port
    this->_addr.sin_addr.s_addr = inet_addr(ip);
    if (bind(this->_listner, (sockaddr *) &this->_addr, sizeof(this->_addr)) < 0) {
        exit(15);
    }
    listen(this->_listner, 1);
    this->_recv = std::thread(&Server::RecvMessage, this);
    buf_msg = new char[256];
}

void Server::RecvMessage()
{

}

void Server::OpenConnection()
{
    int fd;
    while (true) {
        bool connect = false;
        fd = accept(this->_listner, nullptr, nullptr);
        if (fd < 0) {
            std::cout << "Error connect" << std::endl;
        } else {
            recv(fd, buf_msg, 256, 0);
            //next
        }
    }
}

void Server::Log()
{

}
