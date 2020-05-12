#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
using namespace std;

int main(int argc, char const *argv[])
{
    bool first=true;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    while(true)
    {
        char*msg;
        while(true)
        {
            printf("enter your message:");
            if(first==false)
            {cin.ignore();}
            first=false;
            cin.getline(msg,1024,'\n');
            send(sock , msg , strlen(msg) , 0 );
            if(msg[0]=='q' && msg[1]=='u' && msg[2]=='i'&& msg[3]=='t')
            {
                break;
            }
            printf("\nmessage sent,enter \'+\' to send more messages:");
            char test;
            scanf("%c",&test);
            if(test!='+'){send(sock,"+",1,0);break;}
        }
        while(true)
        {
            char buffer[1024]={0};
            printf("SERVER:");
            valread = read( sock , buffer, 1024);
            if(buffer[0]=='+'){break;}
            printf("%s\n",buffer );
            if(msg[0]=='q' && msg[1]=='u' && msg[2]=='i'&& msg[3]=='t')
            {
                return 0;
            }
        }
    }
    return 0;
}