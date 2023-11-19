#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>

struct ServerConfig {
    int PORT;
};

void loadServerConfig(ServerConfig& config) {
  std::ifstream configFile("server_config.txt");
  if (configFile.is_open()) {
      configFile >> config.PORT;
      configFile.close();
  } else {
      std::cerr << "Error: Unable to open server config file." << std::endl;
        }
  }
void handleClient(int clientSocket) {
    // Handle client communication
}

int main() {
    ServerConfig serverConfig;
    loadServerConfig(serverConfig);

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    // Server setup code using serverConfig.PORT
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  
    if (serverSocket == -1) {
      perror("Error creating socket");
      exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverConfig.PORT);

    // Bind the server socket and listen for incoming connections
    // ...
    // Accept client connection
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
      perror("Error binding socket");
      close(serverSocket);
      exit(EXIT_FAILURE);
    }

     if (listen(serverSocket, 10) == -1) {
      perror("Error listening for connections");
      close(serverSocket);
      exit(EXIT_FAILURE);
  }
      while (true) {
       clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
      if (clientSocket == -1) {
       perror("Error accepting connection");
      continue;
      }
        // Handle the connection (you need to implement this part)
        // e.g., launch a new thread or perform some operation on the clientSocket
    }
    close(serverSocket);
  
    return 0;
}


