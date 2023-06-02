//
// Created by User on 13.03.2023.
//

#ifndef N_NMAP_TCP_SCANNER_H
#define N_NMAP_TCP_SCANNER_H

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cmath>
#include <string>
#include <vector>
#include <fcntl.h>
#include <cstring>

using namespace std;
const int PORT_LIMIT = 65535;
#define THREAD_COUNT 15
//int CLOSEPORT;


void tcp_connect(const char *ip, int port)
{
    long arg;
    struct timeval tv;
    fd_set myset;
    socklen_t lon;
    int valopt;

    int _socket=socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);


    // Set non-blocking
    if( (arg = fcntl(_socket, F_GETFL, NULL)) < 0) {
        //fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        return;
    }

    arg |= O_NONBLOCK;
    if( fcntl(_socket, F_SETFL, arg) < 0) {
        //fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        return;
    }

    // Trying to connect with timeout
    int res = connect(_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (res < 0) {
        if (errno == EINPROGRESS) {
            //fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
            do {
                tv.tv_sec = 15;
                tv.tv_usec = 0;
                FD_ZERO(&myset);
                FD_SET(_socket, &myset);
                res = select(_socket+1, NULL, &myset, NULL, &tv);
                if (res < 0 && errno != EINTR) {
                    //fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
                    return;
                }
                else if (res > 0) {
                    // Socket selected for write
                    lon = sizeof(int);
                    if (getsockopt(_socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {
                        //fprintf(stderr, "Error in getsockopt() %d - %s\n", errno, strerror(errno));
                        return;
                    }
                    // Check the value returned...
                    if (valopt) {
                        //fprintf(stderr, "Error in delayed connection() %d - %s\n", valopt, strerror(valopt));
                        return;
                    }
                    break;
                }
                else {
                    //fprintf(stderr, "Timeout in select() - Cancelling!\n");
                    return;
                }
            } while (1);
        }
        else {
            //fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
            return;
        }
    }

    // Set to blocking mode again...
    if( (arg = fcntl(_socket, F_GETFL, NULL)) < 0) {
        fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
        return;
    }
    arg &= (~O_NONBLOCK);
    if( fcntl(_socket, F_SETFL, arg) < 0) {
        fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
        return;
    }

    std::cout<<"Port "<< port << " is open"<<std::endl;
    close(_socket);
   // if(res == -1)
  //  {
   //     CLOSEPORT++;
   //     close(_socket);
   // }
   // else
   // {
   //     std::cout<<"Port "<< port << " is open"<<std::endl;
   //     close(_socket);
   // }
}


struct ArgMultiply
{
    int start_port;
    int end_port;
    string ip;
};

void *TaskMultiply(void* args)
{
    ArgMultiply *argMultiply = (ArgMultiply*) args;
    string ip = argMultiply->ip;
    for (int port = argMultiply->start_port; port<argMultiply->end_port; port++)
    {
        tcp_connect(ip.c_str(), port);
    }
}

void MultiplyParallel(string ip)
{
    pthread_t threads[THREAD_COUNT];
    int chunk_size = PORT_LIMIT/THREAD_COUNT;
    for (int i=0; i<THREAD_COUNT; i++)
    {
        ArgMultiply *argMultiply = new ArgMultiply();
        argMultiply->start_port = i*chunk_size;
        argMultiply->end_port = (i+1)*chunk_size;
        argMultiply->ip = ip;
        pthread_create(&threads[i], 0, TaskMultiply, argMultiply);
    }

    for (int i=0; i<THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);
}

void start(vector<string> ip_list)
{
    for(const string& ip_element: ip_list)
    {
        cout << "|---- " << ip_element << endl;
        MultiplyParallel(ip_element);
        //cout<< "Close port: " << CLOSEPORT<<endl;
    }
}




#endif //N_NMAP_TCP_SCANNER_H
