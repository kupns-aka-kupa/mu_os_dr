#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

struct Task
{
    double a;
    double b;
    int function;
    double result;
};

void WriteToNormalFile()
{
    FILE * inputFile;
    std::ofstream outputFile("result.txt");
    inputFile = fopen("result.dat", "rb");
    struct Task task;
    size_t count = fread((char *)&task, sizeof(struct Task), 1, inputFile);
    outputFile << "Low Border = " << task.a << " High Border = " << task.b << " Number of Function = " << task.function << " Result = " << task.result << std::endl;
    fclose(inputFile);
    outputFile.close();
}

void PrintError(const char * message)
{
    std::cerr << message;std::cout << '\n';
    exit(1);
}

int main()
{
    FILE * output;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        PrintError("ERROR intime creating client socket");
    }
    int portNumber = 8080;
    sockaddr_in serverSockaddrIn = {}, cli_addr;
    serverSockaddrIn.sin_port = htons(portNumber);
    serverSockaddrIn.sin_family = AF_INET;
    serverSockaddrIn.sin_addr.s_addr = INADDR_ANY;
    int bindResult = bind(serverSocket, (struct sockaddr *) &serverSockaddrIn, sizeof(serverSockaddrIn));

    if (bindResult < 0)
    {
        PrintError("ERROR intime associating address with a socket");
    }

    listen(serverSocket, 2);
    int clientSocket;
    socklen_t clientSocketLength;
    clientSocketLength = sizeof(cli_addr);
    clientSocket = accept(serverSocket, (struct   sockaddr   *)&cli_addr, &clientSocketLength);
    if (clientSocket < 0)
    {
        PrintError("ERROR intime accepting connection");
    }
    output = fopen("result.dat", "wb");
    struct Task task;
    while (true)
    {
        int data = recv(clientSocket, (char *)&task, sizeof(struct Task), 0);
        if (data == 0)
        {
            fclose(output);
            break;
        }
        if (data > 0)
        {
            fwrite((char *)&task, 1, data, output);
        }
    }
    close(clientSocket);
    close(serverSocket);
    WriteToNormalFile();
    return 0;
}
