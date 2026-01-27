#include <iostream>
#include <string>
#include <sstream>
#include "../TCPDemo/TCPClient.hpp"
#include <netdb.h>
#include <arpa/inet.h>

#include "../TCPDemo/utils.hpp"
#include "httpClient.hpp"

using namespace std;

string HttpClient::parseURL(const string& url, string& host, string& path, int& port) {
    size_t protocal_pos = url.find("://");
    if (protocal_pos == string::npos) {
        return "Invalid URL format";
    }
    string protocol = url.substr(0, protocal_pos);
    string remaining = url.substr(protocal_pos + 3);

    size_t path_pos = remaining.find("/");
    if (path_pos == string::npos) {
        host = remaining;
        path = "/";
    } else {
        host = remaining.substr(0, path_pos);
        path = remaining.substr(path_pos);
    }

    size_t port_pos = host.find(":");
    if (port_pos != string::npos) {
        port = stoi(host.substr(port_pos + 1));
        host = host.substr(0, port_pos);
    } else {
        // 443 for https, 80 for http
        port = (protocol == "https") ? 443 : 80;
    }
    return "";
}

string HttpClient::resolveHostToIP(const string& host) {
        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(host.c_str(), nullptr, &hints, &res) != 0) {
            return "";
        }

        char ipStr[INET_ADDRSTRLEN];
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
        inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, sizeof(ipStr));

        freeaddrinfo(res);
        std::cout<<"Input host: "<<host<<std::endl
                <<" Resolved IP: "<<ipStr<<std::endl;
        return string(ipStr);
    }

bool HttpClient::get(const string& url, string& response) {
    string host, path;
    int port;

    string parseError = parseURL(url, host, path, port);
    if (!parseError.empty()) {
        cerr << parseError << endl;
        return false;
    }

    string ip = resolveHostToIP(host);
    if (ip.empty()) {
        cerr << "Failed to resolve host: " << host << endl;
        return false;
    }

    TCPClient tcpClient(ip, port);
    if (!tcpClient.connect()) {
        return false;
    }

    stringstream requestStream;
    requestStream << "GET " << path << " HTTP/1.1\r\n"
                    << "Host: " << host << "\r\n"
                    << "Connection: close\r\n\r\n";
    string request = requestStream.str();

    if (!tcpClient.sendData(request)) {
        tcpClient.disconnect();
        return false;
    }

    char buffer[4096];
    int bytesRead;
    response.clear();
    while ((bytesRead = tcpClient.receiveData(buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout<<"Received From " << url <<" : "<<std::endl
                    << bytesRead<<" bytes"<<std::endl;
        response += string(buffer);
    }

    tcpClient.disconnect();
    return true;
}
