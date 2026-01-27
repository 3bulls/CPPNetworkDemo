#include "utils.hpp"
#include "TCPClient.hpp"


TCPClient::TCPClient(const std::string& ip, int port) : serverIP(ip), serverPort(port), clientSocket(-1) {
        if (!NetworkUtils::initializeNetwork()) {
            throw std::runtime_error("Failed to initialize network");
        }
    }

TCPClient::~TCPClient() {
    disconnect();
    NetworkUtils::cleanupNetwork();
}

bool TCPClient::connect() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    
    // pton: presentation to network
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        NetworkUtils::closeSocket(clientSocket);
        return false;
    }

    if (::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        NetworkUtils::closeSocket(clientSocket);
        return false;
    }

    return true;
}

void TCPClient::disconnect() {
    if (clientSocket >= 0) {
        NetworkUtils::closeSocket(clientSocket);
        clientSocket = -1;
    }
}

bool TCPClient::sendData(const std::string& data) {
    if (clientSocket < 0) {
        std::cerr << "Socket is not connected" << std::endl;
        return false;
    }

    ssize_t bytesSent = send(clientSocket, data.c_str(), data.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Failed to send data" << std::endl;
        return false;
    }

    return true;
}

int TCPClient::receiveData(char* buffer, size_t bufferSize) {
    if (clientSocket < 0) {
        std::cerr << "Socket is not connected" << std::endl;
        return -1;
    }

    // 调用recv接收数据，会阻塞直到有数据到达
    ssize_t bytesRead = recv(clientSocket, buffer, bufferSize, 0);
    if (bytesRead < 0) {
        std::cerr << "Failed to receive data" << std::endl;
        return -1;
    }

    return static_cast<int>(bytesRead);
}

std::string TCPClient::receiveData(size_t maxSize) {
    if (clientSocket < 0) {
        std::cerr << "Socket is not connected" << std::endl;
        return "";
    }

    char buffer[1024];
    // 调用recv接收数据，会阻塞直到有数据到达
    ssize_t bytesRead = recv(clientSocket, buffer, std::min(maxSize, sizeof(buffer) - 1), 0);
    if (bytesRead < 0) {
        std::cerr << "Failed to receive data" << std::endl;
        return "";
    }

    buffer[bytesRead] = '\0'; // Null-terminate the received data
    return std::string(buffer);
}

void TCPClient::runInteractive() {
    if (clientSocket < 0) {
        std::cerr << "Socket is not connected" << std::endl;
        return;
    }

    std::string input;
    while (true) {
        std::cout << "Enter message (type 'exit' to quit): ";
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }
        if (!sendData(input)) {
            break;
        }
        std::string response = receiveData();
        if (response.empty()) {
            break;
        }
        std::cout << "Server response: " << response << std::endl;
    }
}
