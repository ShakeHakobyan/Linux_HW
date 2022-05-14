#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include "Server.h"
#include "Request.h"
#include "Response.h"
#include "Handler.h"

Server::Server(int p) : port(p)
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFd < 0)
    {
        std::cerr << "Error while creating a socket" << std::endl;
        exit(errno);
    }

     address.sin_family = AF_INET;
     address.sin_port = htons(port);
     address.sin_addr.s_addr = htonl(INADDR_ANY);

     int bound = bind(serverFd, (const struct sockaddr*) &address, sizeof(address));

     if(bound < 0)
     {
        std::cerr << "Could not bind to given port" << std::endl;
        exit(errno);
     }
}

void Server::run()
{
    int listening = listen(serverFd, 1024);
    if(listening < 0)
    {
        std::cerr << "Could not start listening" << std::endl;
        exit(errno);
    }

     while(true)
     {
         sockaddr_in clientAddress;
         unsigned int clientAddressLength;
         int clientFd = accept(serverFd,(struct sockaddr*)  &clientAddress, &clientAddressLength);
         struct in_addr clientAddr = clientAddress.sin_addr;
         int clientPort = ntohs(clientAddress.sin_port);
         char* clientIp = inet_ntoa(clientAddr);
         std::cout << "Addr: " << clientAddr.s_addr << std::endl;
         std::cout << "Got connection from " << clientIp << ":" << clientPort << std::endl;

         /*


         */

         ssize_t sentBytes = send(clientFd, (const void*) &responce, sizeof(pow), 0);

         if(sentBytes < 0)
         {
            std::cerr << "Could not write to client. Error: " << errno << std::endl;
            close(clientFd);
            continue;
         }

         close(clientFd);
     }
}


void Server::add_handler(const Handler& h)
{
    handlers.push_back(h);
}