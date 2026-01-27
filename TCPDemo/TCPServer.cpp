#include "utils.hpp"
#include "TCPServer.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

// 步骤：
// 1. 创建server socket
// 2. 绑定端口和IP地址 bind
// 3. 监听连接 listen
// 4. 接受连接 accept
// 5. 处理客户端请求 handleClient 

TCPServer::TCPServer(int port) : port(port), isRunning(false), serverSocket(-1) {
    if (!NetworkUtils::initializeNetwork()) {
        throw runtime_error("Failed to initialize network");
    }
}

TCPServer::~TCPServer() {
    stop();
    NetworkUtils::cleanupNetwork();
}

bool TCPServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "Failed to create socket" << endl;
        return false;
    }

    // allow address reuse
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in serverAddr;

    //含义：设置监听的 IP 地址和端口号
    //解释：INADDR_ANY 是一个宏，通常等于 0（即 0.0.0.0）。它告诉内核，“请监听本机所有网卡（网络接口）上的连接”。

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    // htons: host to network short
    serverAddr.sin_port = htons(port);

    // 绑定server和socket文件描述符
    // std::bind is different from ::bind
    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Bind failed" << endl;
        NetworkUtils::closeSocket(serverSocket);
        return false;
    }

    // 监听Socket
    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed" << endl;
        NetworkUtils::closeSocket(serverSocket);
        return false;
    }

    isRunning = true;

    cout << "Server started on port " << port << endl;
    return true;
}

 void TCPServer::stop() {
    if (isRunning) {
        isRunning = false;
        NetworkUtils::closeSocket(serverSocket);
        cout << "Server stopped." << endl;
    }
}

void TCPServer::run() {
    if (!isRunning) {
        cerr << "Server is not running. Call start() first." << endl;
        return;
    }
    acceptClients();
}

void TCPServer::acceptClients() {
    vector<thread> clientThreads;
    while (isRunning) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        // accept使server进入阻塞状态，等待客户端连接
        // 传入clientAddr用于获取客户端的地址信息
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            if (isRunning) {
                cerr << "Accept failed" << endl;
            }
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        cout << "Connection from " << clientIP << ":" << ntohs(clientAddr.sin_port) << endl;
        // 分发的模型：
        // 主线程用于接受连接
        // 每个连接到来后，分发到新线程用于处理客户端请求
        // 因为vector内部是thread，所以这里用emplace_back传的是thread的构造所需的参数
        // this指向当前对象，等同于传入TCPServer*，因为handleClient是成员函数，必须要要指明是哪个对象的成员函数
        clientThreads.emplace_back(&TCPServer::handleClient, this, clientSocket);
    }
    for (auto& t : clientThreads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void TCPServer::handleClient(int clientSocket) {
    char buffer[1024];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        cout << "Received: " << buffer << endl;
        send(clientSocket, buffer, bytesRead, 0); // Echo back
    }
    NetworkUtils::closeSocket(clientSocket);
    cout << "Client disconnected." << endl;
}