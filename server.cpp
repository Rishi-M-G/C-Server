#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

std::mutex coutMutex; //used to synchronize std::cout among multiple threads

//logic for handling logic for handling communication with a single client
void handleClient(int clientSocket)
{
    //Example server: Echo (or) Mirror Server
    char buffer[1024];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer),0)) > 0){
        send(clientSocket, buffer, bytesRead,0);
    }
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socketlen_t clientAddrLen = sizeof(cliendAddr);

    //create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set Up server address struct
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    //Bind the socket
    if(bind(serverSocket,(struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
        perror("Error binding socket");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    //Listen for incoming connections
    if(listen(serverSocket, 10) == -1)
    {
        perror("Error listening");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout<<"Server listening on port 8080...\n";

    while(true)
    {
        //Accept a connection
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if(clientSocket == -1)
        {
            perror("Error accepting connection");
            continue;
        }

        std::thread(handleClient,clientSocket).detach();
    }

    close(serverSocket);
    return 0;

}