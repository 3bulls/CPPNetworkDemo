#pragma once


class TCPServer
{private:
    int serverSocket;
    int port;
    bool isRunning;
public:
    TCPServer(int port);
    ~TCPServer();
    bool start();
    void stop();
    void run();
    void acceptClients();
    void handleClient(int clientSocket);
};