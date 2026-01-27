#include "TCPClient.cpp"

int main() {
    const std::string serverIP = "127.0.0.1";
    const int serverPort = 8080;
    TCPClient client(serverIP, serverPort);
    if (!client.connct()) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }
    client.runInteractive();
}