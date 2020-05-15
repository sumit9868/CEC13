#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
using namespace std;

int main(int argc, char const *argv[])
{
    bool first=true;
    int server_fd, new_socket, valread;
    struct sockaddr_in Addrs;
    int opt = 1;
    int addrlen = sizeof(Addrs);
    string helloMessage="Hello form the server side";
    const char *hello = helloMessage.c_str();

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    Addrs.sin_family = AF_INET;
    Addrs.sin_addr.s_addr = INADDR_ANY;
    Addrs.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&Addrs,sizeof(Addrs))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&Addrs,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true)
    {
        while(true)
        {
            char buffer[1024]={0};
            valread = read( new_socket , buffer, 1024);
            if(buffer[0]=='+'){break;}
            cout<<"CLIENT:";
            if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t')
            {
                send(new_socket,"See you later",12,0);
                cout<<buffer;
                return 0;
            }
            printf("%s\n",buffer );
    break;
        }
        while(true)
        {
            cout<<"Enter your message:";
            char *msg;
            if(first==false)
            {cin.ignore();}
            first=false;
            cin.getline(msg,1024,'\n');
            send(new_socket , msg , strlen(msg) , 0 );
            printf("\nmessage sent,enter \'+\' to send more messages:");
            char test;
            scanf("%c",&test);
            if(test!='+'){send(new_socket,"+",1,0);break;}
        }
    }
    return 0;
}