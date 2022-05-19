#include "HTTP_Server.h"
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

Request::Request(std::string _request)
{
    std::vector<std::string> lines = split_string(_request, "\n");

    // spliting start line
    std::vector<std::string> start_line = split_string(lines[0], "\n");
    method = start_line[0];
    url = start_line[1];
    version = start_line[2];
    
    // spliting headers
    int i = 1;
    while (i < lines.size() && lines[i] != "\n")
    {
        std::vector<std::string> header = split_string(lines[i], ":");
        headers[header[0]] = header[1];
        ++i;
    }
    
    // spliting body
    ++i;
    while (i < lines.size())
    {
        body += lines[i];
        ++i;
    }
}


std::vector<std::string> Request::split_string(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> strings;
    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    strings.push_back(str.substr(prev));

    return strings;
}

std::string Response::make_response()
{
    std::string response = "";
    // conecting status line
    response += version + " " + status_code + " " + status_text + "\n";

    // conecting headers
    for (const auto& header : headers)
    {
        response += header.first + "\n";
    }

    // conecting body
    response += body;
    
    return response;
}

Server::Server(int p) : port(p)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Error while creating a socket" << std::endl;
        exit(errno);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int bound = bind(server_fd, (const struct sockaddr*)&address, sizeof(address));

    if (bound < 0)
    {
        std::cerr << "Could not bind to given port" << std::endl;
        exit(errno);
    }
}

void Server::serve(int client_fd)
{
    std::string request_str;
    ssize_t received_bytes = recv(client_fd, (void*)&request_str, sizeof(request_str), 0);

    if (received_bytes < 0) {
        std::cerr << "Could not read from client. Error: " << strerror(errno) << std::endl;
        close(client_fd);
        continue;
    }

    Request request(request_str);
    Response response = handle(request);
    std::string response_str  = response.make_responce();

    ssize_t sent_bytes = send(client_fd, (const void*)&response_str, sizeof(response_str), 0);

    if (sent_bytes < 0)
    {
        std::cerr << "Could not write to client. Error: " << errno << std::endl;
        close(client_fd);
        continue;
    }

    close(client_fd);
}

void Server::run()
{
    int listening = listen(server_fd, 1024);
    if (listening < 0)
    {
        std::cerr << "Could not start listening" << std::endl;
        exit(errno);
    }

    while (true)
    {
        sockaddr_in client_address;
        unsigned int client_address_length;
        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_length);
        struct in_addr client_addr = client_address.sin_addr;
        int client_port = ntohs(client_address.sin_port);
        char* client_ip = inet_ntoa(client_addr);
        std::cout << "Addr: " << client_addr.s_addr << std::endl;
        std::cout << "Got connection from " << client_ip << ":" << client_port << std::endl;

        serve(client_fd);
    }
}


void Server::add_handler(const Handler& h)
{
    handlers.push_back(h);
}
