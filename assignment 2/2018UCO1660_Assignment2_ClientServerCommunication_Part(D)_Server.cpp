#include <iostream>
#include<sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <fstream>
using namespace std;
int main(int argc, char const *argv[]){
    char *cd="/home/red/CEC13/assignment 2";
    if(argc<2){
        cout<<"Enter the port number to listen to";
        return 0;
    }
    bool first=true;
    int srvrfd, newSocket, valrd;
    struct sockaddr_in Addr;
    int optn = 1;
    int Addrlen = sizeof(Addr);
    int prtNo=atoi(argv[1]);

    if ((srvrfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons( prtNo );
    if (bind(srvrfd, (struct sockaddr *)&Addr,
                                 sizeof(Addr))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(srvrfd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((newSocket = accept(srvrfd, (struct sockaddr *)&Addr,
                       (socklen_t*)&Addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while(true){
        char buffer[1024]={0};
        valrd = read( newSocket , buffer, 1024);
        if(buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t'){
            send(newSocket,"SEE YOU LATER",12,0);
            return 0;
        }
        if(buffer[0]=='p' && buffer[1]=='u' && buffer[2]=='t'){
            char file[2000];
            valrd=read(newSocket,file,2000);
            cout<<file<<endl;
        }
        if(buffer[0]=='g' && buffer[1]=='e' && buffer[2]=='t'){
                char file[1024];
                valrd=read(newSocket,file,1024);
                ifstream ifobj;
                ifobj.open(file);
                if(ifobj.fail()){
                    send(newSocket,"No Such File Found",12,0);
                    continue;
                }
                char ch;
                int i=0;
                char bufstring[2000];
                while(ifobj>>ch){
                    bufstring[i]=ch;
                    i++;
                }
                ifobj.close();
                send(newSocket,bufstring,strlen(bufstring),0);
        }
        if(buffer[0]=='c'){
            if(buffer[2]=='l' && buffer[3]=='s'){
              
            }
            else if(buffer[2]=='p' && buffer[3]=='w' && buffer[4]=='d'){
              
            }
            else if(buffer[2]=='c' && buffer[3]=='d'){
                
            }
        }
        if(buffer[0]=='s'){
            if(buffer[2]=='l' && buffer[3]=='s'){
                char*args[]={"/home/red/CEC13/assignment 2/2018UCO1660_Assignment2_ClientServerCommunication_Part(D)_S_ls",cd,NULL};
                if(fork()==0)
                {execv(args[0],args);}
            }
            else if(buffer[2]=='p' && buffer[3]=='w' && buffer[4]=='d'){
                cout<<cd<<endl;
            }
            if(buffer[2]=='c' && buffer[3]=='d'){
                char dirname[1024]={0};
                valrd=read(newSocket,dirname,1024);
                cd=dirname;
            }
        }
    }
    return 0;
}
