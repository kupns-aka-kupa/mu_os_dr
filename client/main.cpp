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

using namespace std;

void PrintError(const char * message)
{
    cerr << message;
    cout << '\n';
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

int cli(int &from, string &number, int &to, int argc, char *argv[])
{
    for(int count = 0; count < argc; count++)
    {
        if(strcmp(argv[count], "-f") == 0) from = stoi(argv[++count]);
        else if(strcmp(argv[count], "-t") == 0) to = stoi(argv[++count]);
        else if(strcmp(argv[count], "-n") == 0) number = argv[++count];
    }
}

int prompt(int &number, string &from, int &to)
{
    cout << "Enter number: ";
    cin >> number;
    cout << "Enter base from: " << '\n';
    cin >> from;
    cout << "Enter base to: " << '\n';
    cin >> to;
}

int main(int argc, char *argv[])
{
    int to, from;
    string number;

    if(argc > 0) cli(from, number, to, argc, argv);
    else prompt(from, number, to);

    ofstream fout("result.dat", std::ios_base::app);
    if (fout.is_open())
    {
        int decimal = toDecimal(number, from);
        fout << "Number: " << number
        << "From base: " << from
        << "To base: " << to
        << "Result: " <<  decimalTo(decimal, to) << endl;
    }
    else cout << "Error opening file." << endl;

    fout.close();
    cout << "Sending result file in client..." << endl;
    SendTaskToServer();
    return 0;
}