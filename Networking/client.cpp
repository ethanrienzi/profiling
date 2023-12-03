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

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        error("Error opening socket");
    }

    sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddr.sin_port = htons(serverPort);

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        error("Error connecting to server");
    }

    std::cout << "Connected to server at " << serverIP << ":" << serverPort << std::endl;

    const char* message = "Hello from the client";
    char responseBuffer[256];
    ssize_t bytesRead, bytesSent;

    int messageCount = 0; // Counter for messages exchanged

    while (communicationTime > 0) {
        // Send data to server
        bytesSent = write(clientSocket, message, strlen(message) + 1);  // +1 to include null terminator
        if (bytesSent < 0) {
            error("Error writing to socket");
        }

        std::cout << "Sent message to server" << std::endl;
        messageCount++;

        // Read response from server
        bytesRead = read(clientSocket, responseBuffer, sizeof(responseBuffer));
        if (bytesRead <= 0) {
            // Connection closed by server
            std::cerr << "Error reading from server\n";
            break;
        }

        std::cout << "Received response from server: " << responseBuffer << std::endl;

        //sleep(1);  // Sleep for 1 second between exchanges
        communicationTime--;
    }

    close(clientSocket);

    std::cout << "Communication complete. Client shutting down.\n";
    std::cout << "Number of messages exchanged: " << messageCount << std::endl;

    return 0;
}
