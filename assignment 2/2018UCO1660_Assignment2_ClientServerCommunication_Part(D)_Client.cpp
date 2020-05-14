#include<sys/wait.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
using namespace std;
int main(int argc, char const *argv[]){
    char *cd="/home/red/CEC13/assignment 2";
    if(argc<3){
        cout<<"Enter ip address and port";
        return 0;
    }
    int prtNo=atoi(argv[2]);
    int sock = 0, valrd;
    struct sockaddr_in servAddr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error in Socket creation \n");
        return -1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(prtNo);

    if(inet_pton(AF_INET, argv[1], &servAddr.sin_addr)<=0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }
    cout<<"CONNECTION SUCCESSFULLY ESTABLSHED\n";
    while(true){
        char *cmnd;
        cout<<"Enter commands:";
        cin>>cmnd;
        send(sock , cmnd , strlen(cmnd) , 0 );
        if(cmnd[0]=='g' && cmnd[1]=='e' && cmnd[2]=='t'){
            cout<<"Enter your file's name:";
            char filename[1024];
            cin>>filename;
            send(sock,filename,strlen(filename),0);
            char result[2000];
            valrd = read( sock , result, 2000);
            cout<<result<<endl;
        }
        if(cmnd[0]=='p' && cmnd[1]=='u' && cmnd[2]=='t'){
            cout<<"Enter your file's name:";
            char fileName[1024];
            cin>>fileName;
            ifstream ifobj;
            ifobj.open(fileName);
            if(ifobj.fail()){
                send(sock,"File NOT FOUND",12,0);
                continue;
            }
            char *data=new char[2000];
            char ch;
            int *zx=new int;
            *zx=0;
            while(ifobj>>ch){
                data[*zx]=ch;
                (*zx)++;
            }
            cout<<data<<endl;
            send(sock,data,strlen(data),0);
            ifobj.close();
        }
        if(cmnd[0]=='c'){
            if(cmnd[2]=='p' && cmnd[3]=='w' && cmnd[4]=='d'){
                cout<<cd<<endl;
            }
            else if(cmnd[2]=='l' && cmnd[3]=='s'){
                if(fork()==0){
                    char *args[]={"./2018UCO1660_Assignment2_ClientServerCommunication_Part(D)_S_ls",cd,NULL};
                    execv(args[0],args);
                }
                else{wait(NULL);}
            }
            else if(cmnd[2]=='c' && cmnd[3]=='d'){
                char dirname[1024];
                cout<<"Enter the directory name:";
                cin>>dirname;
                cd=dirname;
            }
        }
        if(cmnd[0]=='s'){
            if(cmnd[2]=='c' && cmnd[3]=='d'){
                char dirname[1024];
                cout<<"\nEnter the directory name:";
                cin>>dirname;
                send(sock,dirname,strlen(dirname),0);
            }
        }
        if(cmnd[0]=='q' && cmnd[1]=='u' && cmnd[2]=='i' && cmnd[3]=='t'){
            char buffer[1024]={0};
            valrd = read( sock , buffer, 1024);
            cout<<buffer<<endl<<"CLOSING CONNECTION";
            return 0;
        }
    }
    return 0;
}
