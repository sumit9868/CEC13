#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
using namespace std;
int main(int argc, char const *argv[])
{
    int sckt = 0, valrd;
    struct sockaddr_in servAddr;
    if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error in socket creation\n");
        return -1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr)<=0){
        printf("\nInvalid address/ Address is not supported here \n");
        return -1;
    }
    if (connect(sckt, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        printf("\nConnection is Failed \n");
        return -1;
    }
    while(true){
        char buffer[1024]={0};
        cout<<"WRITE YOUR MESSAGE HERE->";
        char *msg;
        cin.getline(msg,1024,'\n');
        send(sckt , msg , strlen(msg) , 0 );
        valrd = read( sckt , buffer, 1024);
        printf("%s\n",buffer );
        if(msg[0]=='q' && msg[1]=='u' && msg[2]=='i' && msg[3]=='t'){
            cout<<"CONNECTION CLOSING";
            break;
        }
    }
    return 0;
}
