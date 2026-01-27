#include "utils.hpp"

class UDPServer {
private:
    int serverSocket;
    int serverPort;
    bool isRunning;
public:
    UDPServer(int port) : serverPort(port), serverSocket(-1), isRunning(false) {
        if (!NetworkUtils::initializeNetwork()) {
            throw std::runtime_error("Failed to initialize network");
        }
    }   
    ~UDPServer() {
        stop();
        NetworkUtils::cleanupNetwork();
    }

    bool start() {
        serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(serverPort);

        if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Bind failed" << std::endl;
            NetworkUtils::closeSocket(serverSocket);
            return false;
        }

        isRunning = true;
        return true;
    }

    void stop() {
        if (isRunning) {
            NetworkUtils::closeSocket(serverSocket);
            isRunning = false;
        }
    }
    void run() {
        char buffer[1024];
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);

        while (isRunning) {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0,
                                         (struct sockaddr*)&clientAddr, &addrLen);
            if (bytesReceived > 0) {
                std::cout << "Received: " << buffer << std::endl;
                // Echo back the received data
                sendto(serverSocket, buffer, bytesReceived, 0,
                   (struct sockaddr*)&clientAddr, addrLen);
            } else {
                std::cerr << "Receive failed" << std::endl;
            }
        }
    }
};