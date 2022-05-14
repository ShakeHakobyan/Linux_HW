#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <Handler>
#include <vector>


class Server
{
public:
    int port;
    int serverFd;
    sockaddr_in address;
    boost::asio::thread_pool pool;
    std::vector<Handler> handlers;

    Server(int p);
    void serve(int clientFd);
    void run();
    void add_handler(const Handler& h);
}