#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <thread>

struct ClientConfig {
    std::string serverAddress;
    int PORT;
};

void loadClientConfig(ClientConfig& config) {
    std::ifstream configFile("client_config.txt");
    if (configFile.is_open()) {
        configFile >> config.serverAddress >> config.PORT;
        configFile.close();
    } else {
        std::cerr << "Error: Unable to open client config file." << std::endl;
    }
}

void communicateWithServer() {
    // Client communication with server
}

int main() {
    ClientConfig clientConfig;
    loadClientConfig(clientConfig);

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
      std::cerr << "Error: Unable to create socket." << std::endl;
      return -1;
  }

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(clientConfig.PORT);
  inet_pton(AF_INET, clientConfig.serverAddress.c_str(), &serverAddr.sin_addr);

  if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
      std::cerr << "Error: Unable to connect to the server." << std::endl;
      close(clientSocket);
      return -1;
  }

   const char* message = "Hello, Server!";
   send(clientSocket, message, strlen(message), 0);
    // Client setup code using clientConfig.serverAddress and clientConfig.port
    // ...
    close(clientSocket);

    std::thread(communicateWithServer).detach();

    return 0;
}

