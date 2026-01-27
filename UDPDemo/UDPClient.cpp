#include "utils.hpp"
#include <string>

class UDPClient {
private:
    int clientSocket;
    std::string serverIP;
    int serverPort;
public:
    UDPClient(const std::string& ip, int port) : serverIP(ip), serverPort(port), clientSocket(-1) {
        if (!NetworkUtils::initializeNetwork()) {
            throw std::runtime_error("Failed to initialize network");
        }
    }

    ~UDPClient() {
        disconnect();
        NetworkUtils::cleanupNetwork();
    }

    bool connectToServer() {
        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        return true;
    }

    void disconnect() {
        if (clientSocket >= 0) {
            NetworkUtils::closeSocket(clientSocket);
            clientSocket = -1;
        }
    }

    bool sendData(const std::string& data) {
        if (clientSocket < 0) {
            std::cerr << "Socket is not connected" << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);

        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported" << std::endl;
            return false;
        } else {
            std::cout << "Server address set to " << serverIP << ":" << serverPort << std::endl;
        }

        int bytesSent = sendto(clientSocket, data.c_str(), data.size(), 0,
                               (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent < 0) {
            std::cerr << "Send failed" << std::endl;
            return false;
        }
        return true;
    }

    bool receiveData(std::string& data) {
        if (clientSocket < 0) {
            std::cerr << "Socket is not connected" << std::endl;
            return false;
        }

        char buffer[1024];
        sockaddr_in fromAddr;
        socklen_t addrLen = sizeof(fromAddr);
        memset(buffer, 0, sizeof(buffer));

        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                                     (struct sockaddr*)&fromAddr, &addrLen);
        if (bytesReceived < 0) {
            std::cerr << "Receive failed" << std::endl;
            return false;
        }

        data = std::string(buffer, bytesReceived);
        return true;
    }

    
};