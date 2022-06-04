#include "HTTP_Server.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

Request::Request(char* _request)
{
    size_t start_idx = 0;

    // splitting start line
    std::string first_line = split_char_until(_request, start_idx, '\n');
    std::vector<std::string> first_line_splitted = split_string(first_line, " ");
    method = first_line_splitted[0];
    url = first_line_splitted[1];
    version = first_line_splitted[2];

    // splitting headers
    size_t content_length = 0;
    std::string line = split_char_until(_request, start_idx, '\n');

    while (line != "\n")
    {
        std::vector<std::string> header = split_string(line, ": ");
        headers[header[0]] = header[1];
        if (header[0] == "Content-Length")
        {
            content_length = std::stoi(header[0]);
        }
        line = split_char_until(_request, start_idx, '\n');
    }

    // splitting body
    body = "";
    size_t all_length = start_idx + content_length;
    ++start_idx;

    while (start_idx < all_length)
    {
        body += _request[start_idx];
        ++start_idx;
    }
}

std::string Request::split_char_until(const char* _request, size_t& start_idx, const char& delimiter)
{
    std::string line = "";
    while (_request[start_idx] != delimiter)
    {
        line += _request[start_idx]
        ++start_idx;
    }
    ++start_idx;
    return line;
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

char* Response::make_response()
{
    std::string response = "";
    // connecting status line
    response += version + " " + status_code + " " + status_text + "\n";

    // connecting headers
    for (const auto& header : headers)
    {
        response += header.first + "\n";
    }

    // connecting body
    response += body;


    // making char array

    char response_char[response.length() + 1];

    strcpy(response_char, response.c_str());

    return response_char;
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
    std::string response_str  = response.make_Response();

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
