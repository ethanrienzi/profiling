#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    std::ifstream configFile("config.txt");
    if (!configFile) {
        std::cerr << "Error opening config file.\n";
        return EXIT_FAILURE;
    }

    std::string serverIP;
    int serverPort, communicationTime;

    if (!(configFile >> serverIP >> serverPort >> communicationTime)) {
        std::cerr << "Error reading config file.\n";
        return EXIT_FAILURE;
    }

    configFile.close();

    std::cout << "Read configuration - ServerIP: " << serverIP << ", Port: " << serverPort << ", CommunicationTime: " << communicationTime << " seconds\n";

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        error("Error opening socket");
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error("Error on binding");
    }

    if (listen(serverSocket, 1) < 0) {
        error("Error on listen");
    }

    std::cout << "Server listening on " << serverIP << ":" << serverPort << std::endl;

    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLen);
    if (clientSocket < 0) {
        error("Error on accept");
    }

    char buffer[256];
    ssize_t bytesRead, bytesSent;

    std::cout << "Connection established with client" << std::endl;

    int messageCount = 0; // Counter for messages exchanged

while (communicationTime > 0) {
    // Read data from client
    bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead <= 0) {
        // Connection closed by client
        std::cerr << "Error reading from client\n";
        break;
    }

    std::cout << "Received message from client: " << buffer << std::endl;
    messageCount++;

    // Send a response back to client
    const char* responseMessage = "Hello from the server";
    bytesSent = write(clientSocket, responseMessage, strlen(responseMessage) + 1);  // +1 to include null terminator
    if (bytesSent < 0) {
        error("Error writing to socket");
    }

    std::cout << "Sent response to client" << std::endl;

    communicationTime--;
}
    }

    close(clientSocket);
    close(serverSocket);

    std::cout << "Communication complete. Server shutting down.\n";
    std::cout << "Number of messages exchanged: " << messageCount << std::endl;

    return 0;
}
