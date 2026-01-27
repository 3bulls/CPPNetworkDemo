#pragma once
#include <string>
using namespace std;

class HttpClient {
private:
    string parseURL(const string& url, string& host, string& path, int& port);
    string resolveHostToIP(const string& host);
public:
    bool get(const string& url, string& response);

};