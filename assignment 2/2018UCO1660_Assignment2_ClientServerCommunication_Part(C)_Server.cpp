#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <csignal>
#define PORT 8080
using namespace std;

int serverFile,newSckt;

void signalCallback(int signum){
   cout << "Closing server " << signum << endl;
   close(newSckt);
   exit(signum);
}

int main(int argc, char const *argv[]){
    int i=0;
    int valrd;
    struct sockaddr_in Addr;
    int opt = 1;
    int addrlen = sizeof(Addr);

    if ((serverFile = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failuer ");
        exit(EXIT_FAILURE);
    }
    Addr.sin_family = AF_INET;
    Addr.sin_addr.s_addr = INADDR_ANY;
    Addr.sin_port = htons( PORT );
    if (bind(serverFile, (struct sockaddr *)&Addr,
                                 sizeof(Addr))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(serverFile, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while(true){
        signal(SIGINT, signalCallback);
        if ((newSckt = accept(serverFile, (struct sockaddr *)&Addr,
                           (socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        int pid=fork();
        if(pid<0){cout<<"ERROR ON FORK";}
        if(pid==0){
            close(serverFile);
            while(true){
                char bffr[1024]={0};
                valrd = read( newSckt , bffr, 1024);
                cout<<"[PORT ADDRESS:"<<Addr.sin_port<<" IP-ADDRESS:"<<Addr.sin_addr.s_addr<<"]=>";
                printf("%s\n",bffr );
                if(bffr[0]=='q' && bffr[1]=='u' && bffr[2]=='i' && bffr[3]=='t')
                {
                    send(newSckt , "SEE YOU LATER" , 12 , 0 );
                    break;
                }
                send(newSckt , "RECIEVED BY SERVER" , 18 , 0 );
            }
            exit(0);
        }
        else
        {close(newSckt);}
    }
    return EXIT_SUCCESS;
}
