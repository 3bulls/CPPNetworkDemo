#include <iostream>
#include <string>
#include <cstring>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace std;

class NetworkUtils {
public:
    static bool initializeNetwork() {
#ifdef _WIN32
        WSADATA wsaData;
        return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
#else
        return true;
#endif
    }
    
    static void cleanupNetwork() {
#ifdef _WIN32
        WSACleanup();
#endif
    }
    
    static void closeSocket(int socket) {
#ifdef _WIN32
        closesocket(socket);
#else
        close(socket);
#endif
    }
};


