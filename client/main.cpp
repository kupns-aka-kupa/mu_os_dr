#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "converter.hpp"

void PrintError(const char * message)
{
    std::cerr << message;
    std::cout << '\n';
    exit(1);
}

void SendTaskToServer()
{
    FILE * file = fopen("result.dat", "rb");
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket < 0)
    {
        PrintError("ERROR in the intime creating socket");
    }
    sockaddr_in sockaddrIn = { };
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(8080);
    sockaddrIn.sin_addr.s_addr = htonl(0x7F000001);
    int  connectResult  =  connect(mySocket, (struct  sockaddr  *)&sockaddrIn, sizeof(sockaddr));
    if (connectResult < 0)
    {
        PrintError("ERROR  in the intime connecting");
    }
    int count;
    char buffer[1024];
    count = fread(buffer, 1, sizeof(buffer), file);

    if (send(mySocket, buffer, count, 0) < 0)
    {
        fclose(file);
        PrintError("ERROR while sending the data");
    }
    bzero(buffer, 1024);
    fclose(file);
    close(mySocket);

}

void cli(int &from, std::string &number, int &to, int argc, char *argv[])
{
    for(int count = 0; count < argc; count++)
    {
        if(strcmp(argv[count], "-f") == 0) from = std::stoi(argv[++count]);
        else if(strcmp(argv[count], "-t") == 0) to = std::stoi(argv[++count]);
        else if(strcmp(argv[count], "-n") == 0) number = argv[++count];
    }
}

void prompt(int &number, std::string &from, int &to)
{
    std::cout << "Enter number: ";
    std::cin >> number;
    std::cout << "Enter base from: " << '\n';
    std::cin >> from;
    std::cout << "Enter base to: " << '\n';
    std::cin >> to;
}

int main(int argc, char *argv[])
{
    int to, from;
    std::string number;

    if(argc > 1) cli(from, number, to, argc, argv);
    else prompt(from, number, to);

    std::ofstream fout("result.dat", std::ios_base::app);
    if (fout.is_open())
    {
        int decimal = toDecimal(number, from);
        fout << "Number: " << number
        << "From base: " << from
        << "To base: " << to
        << "Result: " <<  decimalTo(decimal, to) << std::endl;
    }
    else std::cout << "Error opening file." << std::endl;

    fout.close();
    std::cout << "Sending result file in client..." << std::endl;
    SendTaskToServer();
    return 0;
}
