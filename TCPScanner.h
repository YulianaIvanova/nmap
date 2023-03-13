//
// Created by User on 13.03.2023.
//

#ifndef N_NMAP_TCPSCANNER_H
#define N_NMAP_TCPSCANNER_H

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
const int PORT_LIMIT = 65535;

class TCPScanner{

    int _socket;
    int _close_port;

public:

    void start(char* ip)
    {
        for(int i=0;i < PORT_LIMIT;i++)
        {
            tcp_connect(ip, i);
        }
    }

    void tcp_connect(const char *ip, int port)
    {
        _socket=socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in addr{};
        addr.sin_family=AF_INET;
        addr.sin_port=htons(port);
        addr.sin_addr.s_addr=inet_addr(ip);

        int res = connect(_socket, (sockaddr*)& addr, sizeof(addr));
        if(res == -1)
        {
            _close_port++;
            close(_socket);
        }
        else
        {
            std::cout<<"Port "<< port << " is open"<<std::endl;
            close(_socket);
        }
    }
};

#endif //N_NMAP_TCPSCANNER_H
