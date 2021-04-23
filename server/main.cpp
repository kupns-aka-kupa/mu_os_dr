#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstdlib>

using namespace std;

void PrintError(const char * message)
{
    cerr << message;
    cout << '\n';
    exit(1);
}

void SendTaskToServer()
{
    FILE * file;
    file = fopen("result.dat", "rb");
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket < 0)
    {
        PrintError("ERROR in the intime creating socket");
    }
    sockaddr_in sockaddrIn = {};
    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(8080);
    sockaddrIn.sin_addr.s_addr = htonl(0x7F000001);
    int  connectResult  =  connect(mySocket,  (struct  sockaddr  *)  &sockaddrIn, sizeof(sockaddr));
    if (connectResult < 0)
    {
        PrintError("ERROR  in the intime connecting");
    }
    int count;
    char buffer[1024];
    count = fread(buffer, 1, sizeof(buffer), file);

    if (send(mySocket, buffer, count, 0) < 0){
        fclose(file);
        PrintError("ERROR while sending the data");

    }bzero(buffer, 1024);
    fclose(file);
    close(mySocket);

}
int main()
{
    int m=0;
    float res, a, b=0.0;
    char comp;
    cout << "Menu: "<< '\n';
    cout << "1. Addition (a+b)"<< '\n';
    cout << "2. Difference (a-b) "<< '\n';
    cout << "3. Multiplication (a*b)"<< '\n';
    cout << "4. Division (a/b)"<< '\n';
    cout << "5. Compare a and b "<< '\n';
    cout << "6. Exit"<< '\n'<< '\n';
    cout << "Enter number of menu: ";
    cin >> m;
    cout << "Enter a: "<<'\n';
    cin >> a;

    cout << "Enter b: "<<'\n';
    cin >> b;
    ofstream fout("result.dat", "a");
    if (fout.is_open())
    {
        switch(m)
        {
            case 1 :res= a+b;
                cout << "a+b: " << res<< endl;
                fout << "a+b: " << res<< endl;
                break;
                case 2 :res= a-b;
                cout << "a-b= " << res<< endl;
                fout << "a-b: " << res<< endl;
                break;
                case 3 :res= a*b;
                cout << "a*b= " << res<< endl;
                fout << "a*b: " << res<< endl;
                break;
                case 4 :res= a/b;
                cout << "a/b=: " << res<< endl;
                fout << "a/b: " << res<< endl;
                break;
                case 5 :if (a>b)
                {
                    comp='>';
                }
                else if (a<b)
                {
                    comp='<';
                }
                else
                {
                    comp='=';
                }
                cout << "a" << comp<< "b" << endl;
                fout << "a" << comp<< "b" << endl;
                break;
            case 6: return 0;
            default :cout << "Incorrect value." << endl;
        }
    }
    else
    {
        cout << "Error opening file." << endl;
    }
    fout.close();
    cout << "Sending result file in server..." << endl;
    SendTaskToServer();
    return 0;
}