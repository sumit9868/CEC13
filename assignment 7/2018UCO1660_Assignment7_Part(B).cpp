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
    int sndrFDS1[2],sndrFDS2[2],ACKFDS1[2],ACKFDS1[2];
    int rtrnStatus1, rtrnStatus2,rtrnStatus3,rtrnStatus4;

    rtrnStatus1 = pipe(sndrFDS1);
    if (rtrnStatus1 == -1)
    {
        cout<<"Error in creating pipe 1 \n";
        return 1;
    }

    rtrnStatus2 = pipe(sndrFDS2);
    if (rtrnStatus2 == -1)
    {
        cout<<"Error in creating pipe 2 \n";
        return 1;
    }

    rtrnStatus3=pipe(ACKFDS1);
    if(rtrnStatus3==-1)
    {
        cout<<"Error in creating pipe 3\n";
        return 1;
    }

    rtrnStatus4=pipe(ACKFDS1);
    if(rtrnStatus4==-1)
    {
        cout<<"Error in creating pipe 4\n";
        return 1;
    }

    int nonblockstatus=fcntl(ACKFDS1[0], F_SETFL, O_NONBLOCK);
    if(nonblockstatus<0){cout<<"Error";return 1;}

    nonblockstatus=fcntl(sndrFDS2[0], F_SETFL, O_NONBLOCK);
    if(nonblockstatus<0){cout<<"Error";return 1;}

    int n1 = fork();
    int n2 = fork();

    if (n1 > 0 && n2 > 0) {
        int seq=0;int ack=-1;int remaining=0;
        char readMsg[20];
        close(sndrFDS1[0]);
        close(sndrFDS2[0]);close(sndrFDS2[1]);
        close(ACKFDS1[0]);close(ACKFDS1[1]);
        close(ACKFDS1[1]);
        ifstream ifile("slidingWindow_send_file1.txt");
        ofstream ofile("output_1.txt");
        char t[10];
        while(ifile.good())
        {
            if(remaining<3)
            {
                char temp[10];
                ifile>>temp;
                t[0]=48+seq;
                t[1]='_';
                if(ack==-1){t[2]='@';}
                else{t[2]='0'+ack;}
                ack=-1;
                t[3]='_';
                t[4]=temp[0];
                t[5]=temp[1];
                t[6]='\0';
                seq=(1+seq)%3;
                remaining+=1;
                cout<<"1_Sender sending meassage....\n";
                sleep(1);
                write(sndrFDS1[1],t,sizeof(t));
            }
            for(int i=0;i<5;i++)
            {
                sleep(1);
                int d=read(ACKFDS1[0],readMsg,sizeof(readMsg));
                if(d!=-1)
                {
                    ack=readMsg[0]-'0';
                    if(readMsg[2]!='@'){remaining-=1;cout<<"1_Frame "<<readMsg[2]<<" acknowledged\n";}
                    cout<<"data saved"<<"_1"<<endl;
                    ofile<<readMsg+4;ofile<<" ";
                }
                else if(!d){break;}

            }
        }
        close(sndrFDS1[1]);
        ifile.close();
        ofile.close();
        cout<<"Transfer of messages Completed\n";
    }
    else if (n1 == 0 && n2 > 0)
    {
        int c=1;
        char readMsgA[20],readMsgB[20];
        close(sndrFDS1[1]);
        close(sndrFDS2[0]);
        close(ACKFDS1[1]);
        close(ACKFDS1[0]);
        srand(time(0));
        while(true)
        {
            if(read(sndrFDS1[0],readMsgA,sizeof(readMsgA)))
            {
                cout<<"Message in channel transmitting message to B"<<endl;
                write(sndrFDS2[1],readMsgA,sizeof(readMsgA));
            }
            if(read(ACKFDS1[0],readMsgB,sizeof(readMsgB)))
            {
                cout<<"Message in channel transmitting message to A\n";
                write(ACKFDS1[1],readMsgB,sizeof(readMsgB));
            }
            c++;if(c==9){break;}
        }
        close(ACKFDS1[1]);
        close(sndrFDS2[1]);
    }
    else if (n1 > 0 && n2 == 0)
    {
        int seq=0;int ack=-1;int remaining=0;
        char readMsg[20];
        close(sndrFDS1[0]);close(sndrFDS1[1]);
        close(sndrFDS2[1]);
        close(ACKFDS1[0]);
        close(ACKFDS1[0]);close(ACKFDS1[1]);
        ifstream ifile("slidingWindow_send_file2.txt");
        ofstream ofile("output_2.txt");
        char t[10];
        while(ifile.good())
        {
            if(remaining<3)
            {
                char temp[10];
                ifile>>temp;
                t[0]=48+seq;
                t[1]='_';
                if(ack==-1){t[2]='@';}
                else{t[2]='0'+ack;}
                ack=-1;
                t[3]='_';
                t[4]=temp[0];
                t[5]=temp[1];
                t[6]='\0';
                seq=(1+seq)%3;
                remaining+=1;
                cout<<"2_Sender sending message....\n";
                sleep(1);
                write(ACKFDS1[1],t,sizeof(t));
            }
            for(int i=0;i<5;i++)
            {
                sleep(1);
                int d=read(sndrFDS2[0],readMsg,sizeof(readMsg));
                if(d!=-1)
                {
                    ack=readMsg[0]-'0';
                    if(readMsg[2]!='@'){remaining-=1;cout<<"2_Frame "<<readMsg[2]<<" acknowledged\n";}
                    cout<<"data saved"<<"_2"<<endl;
                    ofile<<readMsg+4;ofile<<" ";
                }
                else if(!d){break;}
            }
        }
        ofile.close();
        ifile.close();
    }
    return 0;
}


