#include "httpClient.hpp"
#include <iostream>

using namespace std;

int main()
{
    HttpClient httpClient;
    std::string url = "http://www.example.com/";
    std::string response;
    if (httpClient.get(url, response)) {
        std::cout << "HTTP GET Response:\n"
                  << response << std::endl;
    } else {
        std::cerr << "Failed to perform HTTP GET request" << std::endl;
    }

    return 0;
}