#include "UDPClient.cpp"

int main() {
    const std::string serverIP = "127.0.0.1";
    const int serverPort = 8080;
    UDPClient client(serverIP, serverPort);
    if (!client.connectToServer()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }
    while (true) {
        std::string message;
        std::cout << "Enter message to send (or 'exit' to quit): ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break;
        }
        if (!client.sendData(message)) {
            std::cerr << "Failed to send data" << std::endl;
            continue;
        }
        std::string response;
        if (client.receiveData(response)) {
            std::cout << "Received from server: " << response << std::endl;
        } else {
            std::cerr << "Failed to receive data" << std::endl;
        }
    }
    client.disconnect();
    return 0;
}