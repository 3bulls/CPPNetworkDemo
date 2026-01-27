#include "UDPServer.hpp"

int main() {
    const int port = 8080;
    UDPServer server(port);

    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return -1;
    }
    server.run();

    return 0;
}