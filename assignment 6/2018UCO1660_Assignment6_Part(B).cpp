#include <unistd.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include<time.h>
#include <fcntl.h>
#include<fstream>
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
        cout<<"Error in creating a non-blocking process\n";
        return 1;
    }
    if(fcntl(ACKFDS2[0], F_SETFL, O_NONBLOCK)==-1)
    {
        cout<<"Error in creating a non-blocking process\n";
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
        int curr=0;
        srand(time(0));
        char rdMsg[25];
        int tries=2;
        for(int z=1;;z++)
        {
            if((z%7)==0)
            {
                cout<<"Process 1 sending\n";
                write(sndrFDS[1],msg[curr],sizeof(msg[curr]));
                int num=rand()%2+5;
                bool ack=false;
                for(int i=0;i<num;i++)
                {
                    sleep(1);
                    if(read(ACKFDS1[0],rdMsg,sizeof(rdMsg))!=-1)
                    {
                        curr+=1;
                        cout<<"Acknowledged\n";
                        ack=true;break;
                    }
                }
                if(ack==false)
                {
                    tries-=1;
                    if(tries==0)
                    {cout<<"No more tries left for process 1\n";curr+=1;tries=2;}
                    else
                    {cout<<"Process 1 resending\n";}
                }
                if(curr==4){break;}
                sleep(5);
            }
            else{cout<<"Process 1 didn't send in this slot\n";}
            sleep(1);
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
        int curr=0;
        srand(time(0));
        char rdMsg[25];
        int tries=2;
        for(int z=1;;z++)
        {
            if((z%5)==0)
            {
                cout<<"Process 2 sending\n";
                write(sndrFDS[1],msg[curr],sizeof(msg[curr]));
                int num=rand()%2+6;
                bool ack=false;
                for(int i=0;i<num;i++)
                {
                    sleep(1);
                    if(read(ACKFDS2[0],rdMsg,sizeof(rdMsg))!=-1)
                    {
                        curr+=1;
                        cout<<"Acknowledged\n";
                        ack=true;break;
                    }
                }
                if(ack==false)
                {
                    tries-=1;
                    if(tries==0)
                    {cout<<"No more tries left for process 2\n";curr+=1;tries=2;}
                    else
                    {cout<<"Process 2 resending\n";}
                }
                if(curr==4){break;}
                sleep(5);
            }
            else{cout<<"Process 2 didn't send in this slot\n";}
            sleep(1);
        }
        cout<<"Process 2 transfer complete\n";
        close(sndrFDS[1]);
    }
    else if(n1>0 && n2==0)
    {
        close(sndrFDS[1]);
        close(ACKFDS1[0]);
        close(ACKFDS2[0]);
        char rdMsg[25];
        ofstream process1("process1.txt"),process2("process2.txt");
        while(read(sndrFDS[0],rdMsg,sizeof(rdMsg)))
        {
            if(rdMsg[0]=='1')
            {
                process1<<rdMsg+2<<" ";
                cout<<"Receiver:Process 1 message->"<<rdMsg+2<<" Saved in process1.txt"<<endl;
                write(ACKFDS1[1],"+",2);
            }
            else
            {
                process2<<rdMsg+2<<" ";
                cout<<"Receiver:Process 2 message->"<<rdMsg+2<<" Saved in process2.txt"<<endl;
                write(ACKFDS2[1],"+",2);
            }
            sleep(2);
        }
        process1.close();
        process2.close();
        close(ACKFDS1[1]);
        close(ACKFDS2[1]);
    }
    return 0;
}

