// HTTP_Server.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <unordered_map>
#include <sstream>

// TODO: Figure it out with Handler logic and boost thread pool

class Request
{
public:
    std::string method;
    std::string url;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Request(std::string _request);
private:
    std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);
};

class Responce
{
public:
    std::string version;
    std::string status_code;
    std::string status_text;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    Responce(std::string _version, std::string _status_code, std::string _status_text, std::unordered_map<std::string, std::string> _headers, std::string _body) :
        version(_version), status_code(_status_code), status_text(_status_text), headers(_headers), body(_body) {}

    std::string make_responce();
};


class Handler
{

};


class Server
{
public:
    int port;
    int server_fd;
    sockaddr_in address;
    boost::asio::thread_pool pool;
    std::vector<Handler> handlers;

    Server(int p);
    void serve(int client_fd);
    void run();
    void add_handler(const Handler& h);
};
