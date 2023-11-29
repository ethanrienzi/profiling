#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    std::ifstream configFile("config.txt");
    if (!configFile) {
        std::cerr << "Error opening config file.\n";
        return 1;
    }

    std::string serverIP;
    int serverPort, communicationTime;

    // Read Server IP
    if (!(configFile >> serverIP)) {
        std::cerr << "Error reading Server IP.\n";
        return 1;
    }

    // Read Server Port
    if (!(configFile >> serverPort)) {
        std::cerr << "Error reading Server Port.\n";
        return 1;
    }

    // Read Communication Time
    if (!(configFile >> communicationTime)) {
        std::cerr << "Error reading Communication Time.\n";
        return 1;
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

    listen(serverSocket, 1);
    std::cout << "Server listening on " << serverIP << ":" << serverPort << std::endl;

    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLen);
    if (clientSocket < 0) {
        error("Error on accept");
    }

    std::cout << "Connection established with client" << std::endl;

    char buffer[256];
    ssize_t bytesRead;

    for (int i = 0; i < communicationTime; ++i) {
        // Read data from client
        bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            error("Error reading from socket");
        }

        std::cout << "Received message from client: " << buffer << std::endl;

        // Send a response back to client
        const char* responseMessage = "Hello from the server";
        ssize_t bytesSent = write(clientSocket, responseMessage, strlen(responseMessage));
        if (bytesSent < 0) {
            error("Error writing to socket");
        }

        sleep(1);  // Sleep for 1 second between exchanges
    }

    close(clientSocket);
    close(serverSocket);

    std::cout << "Communication complete. Server shutting down." << std::endl;

    return 0;
}
