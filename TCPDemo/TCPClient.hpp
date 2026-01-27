#pragma once

#include <string>

class TCPClient{
private:
    int clientSocket;
    std::string serverIP;
    int serverPort;
public:

    TCPClient(const std::string& ip, int port);
    ~TCPClient();
    bool connect();
    void disconnect();
    bool sendData(const std::string& data);
    int receiveData(char* buffer, size_t bufferSize);
    std::string receiveData(size_t maxSize = 1024);
    void runInteractive();
};
