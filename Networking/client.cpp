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
    int NUM_MESSAGES;
};

void loadClientConfig(ClientConfig& config) {
    std::ifstream configFile("client_config.txt");
    if (configFile.is_open()) {
        configFile >> config.serverAddress >> config.PORT >> config.NUM_MESSAGES;
        configFile.close();
    } else {
        std::cerr << "Error: Unable to open client config file." << std::endl;
    }
}

void communicateWithServer(int clientSocket, int numMessages) {
    // Client communication with server
    const char* message = "Hello, Server!";

    for (int i = 0; i < numMessages; ++i) {
        // Send a message to the server
        send(clientSocket, message, strlen(message), 0);

        // Print the sent message
        std::cout << "Sent to server: " << message << std::endl;

        // Receive a response from the server
        const int bufferSize = 1024;
        char buffer[bufferSize];
        ssize_t bytesRead = recv(clientSocket, buffer, bufferSize - 1, 0);

        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cout << "Server disconnected." << std::endl;
            } else {
                perror("Error receiving data from server");
            }
            break;
        }

        buffer[bytesRead] = '\0';

        // Print the received response
        std::cout << "Received from server: " << buffer << std::endl;
    }

    // Close the client socket
    close(clientSocket);
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
    

    communicateWithServer(clientSocket, clientConfig.NUM_MESSAGES);
   // std::thread([&clientSocket](){
//		    communicateWithServer(clientSocket, clientConfig.NUM_MESSAGES);
//		    }).detach();
    //std::thread(communicateWithServer).detach();

    close(clientSocket);
    return 0;
}

