#include <unistd.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include<time.h>
#include <fcntl.h>
using namespace std;

int main()
{
    int sndrFDS[2],ACKFDS1[2],ACKFDS2[2];
    if(pipe(sndrFDS)==-1)
    {
        cout<<"Error in creating pipe\n";
        return 1;
    }
    if(pipe(ACKFDS1)==-1)
    {
        cout<<"Error in creating pipe\n";
        return 1;
    }
    if(pipe(ACKFDS2)==-1)
    {
        cout<<"Error in creating pipe\n";
        return 1;
    }
    if(fcntl(ACKFDS1[0], F_SETFL, O_NONBLOCK)==-1)
    {
        cout<<"Error creating non-blocking process\n";
        return 1;
    }
    if(fcntl(ACKFDS2[0], F_SETFL, O_NONBLOCK)==-1)
    {
        cout<<"Error creating non-blocking process\n";
        return 1;
    }
    if(fcntl(sndrFDS[0],F_SETFL,O_NONBLOCK)==-1)
    {
        cout<<"Error creating non-blocking process\n";
        return 1;
    }
    int n1=fork();
    int n2=fork();

    if(n1>0 && n2>0)
    {
        close(sndrFDS[0]);
        close(ACKFDS1[1]);
        close(ACKFDS2[0]);close(ACKFDS2[1]);
        char msg[4][15]={"1_This","1_is","1_process","1_1"};
        int cur=0;
        srand(time(0));
        char readMsg[25];
        int tries=2;
        while(true)
        {
            cout<<"Process 1 sending\n";
            write(sndrFDS[1],msg[cur],sizeof(msg[cur]));
            int num=rand()%2+5;
            bool ack=false;
            for(int i=0;i<num;i++)
            {
                sleep(1);
                if(read(ACKFDS1[0],readMsg,sizeof(readMsg))!=-1)
                {
                    cur+=1;
                    cout<<"Acknowledged\n";
                    ack=true;break;
                }
            }
            if(ack==false)
            {
                tries-=1;
                if(tries==0)
                {cout<<"No more tries left for process 1\n";cur+=1;tries=2;}
                else
                {cout<<"Process 1 resending\n";}
            }
            if(cur==4){break;}
            sleep(5);
        }
        cout<<"Process 1 transfer complete\n";
        close(sndrFDS[1]);
    }
    else if(n1==0 && n2>0)
    {
        close(sndrFDS[0]);
        close(ACKFDS1[0]);close(ACKFDS1[1]);
        close(ACKFDS2[1]);
        char msg[4][15]={"2_This","2_is","2_process","2_2"};
        int cur=0;
        srand(time(0));
        char readMsg[25];
        int tries=2;
        while(true)
        {
            cout<<"Process 2 sending\n";
            write(sndrFDS[1],msg[cur],sizeof(msg[cur]));
            int num=rand()%2+6;
            bool ack=false;
            for(int i=0;i<num;i++)
            {
                sleep(1);
                if(read(ACKFDS2[0],readMsg,sizeof(readMsg))!=-1)
                {
                    cur+=1;
                    cout<<"Acknowledged\n";
                    ack=true;break;
                }
            }
            if(ack==false)
            {
                tries-=1;
                if(tries==0)
                {cout<<"No more tries left for process 2\n";cur+=1;tries=2;}
                else
                {cout<<"Process 2 resending\n";}
            }
            if(cur==4){break;}
            sleep(5);
        }
        cout<<"Process 2 transfer complete\n";
        close(sndrFDS[1]);
    }
    else if(n1>0 && n2==0)
    {
        close(sndrFDS[1]);
        close(ACKFDS1[0]);
        close(ACKFDS2[0]);
        while(true)
        {
            sleep(2);
            char readMsg[25],temp[25];
            int reads=0;
            while(read(sndrFDS[0],readMsg,sizeof(readMsg))==-1){continue;}
            reads=1;
            sleep(2);
            while(read(sndrFDS[0],temp,sizeof(temp))!=-1){reads++;}
            if(reads>1)
            {cout<<"Collision detected\n";continue;}
            if(readMsg[0]=='1')
            {
                cout<<"Receiver:Process 1 message->"<<readMsg+2<<endl;
                write(ACKFDS1[1],"+",2);
            }
            else
            {
                cout<<"Receiver:Process 2 message->"<<readMsg+2<<endl;
                write(ACKFDS2[1],"+",2);
            }
        }
        close(ACKFDS1[1]);
        close(ACKFDS2[1]);
    }
    return 0;
}
