#include <unistd.h>
#include <stdio.h>
#include<iostream>
#include <stdlib.h>
#include<time.h>
#include <fcntl.h>
using namespace std;

int main()
{
    int sndrFDS1[2],sndrFDS2[2],ACKFDS1[2],ACKFDS2[2];
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

    rtrnStatus4=pipe(ACKFDS2);
    if(rtrnStatus4==-1)
    {
        cout<<"Error in creating pipe 4\n";
        return 1;
    }

    int nonblockstatus=fcntl(ACKFDS2[0], F_SETFL, O_NONBLOCK);
    if(nonblockstatus<0){cout<<"Error";return 1;}

    int n1 = fork();
    int n2 = fork();

    if (n1 > 0 && n2 > 0) {
        char readmsg[20];
        close(sndrFDS1[0]);
        close(sndrFDS2[0]);close(sndrFDS2[1]);
        close(ACKFDS1[0]);close(ACKFDS1[1]);
        close(ACKFDS2[1]);
        while(true)
        {
            cout<<"Sending the message from sender....\n";
            sleep(1);
            write(sndrFDS1[1],"hello receiver",sizeof("hello receiver"));
            bool res=false;
            for(int i=0;i<8;i++)
            {
                sleep(1);
                if(read(ACKFDS2[0],readmsg,sizeof(readmsg))!=-1){res=true;break;}
            }
            if(res==true){cout<<"Acknowledged\n";}
            else{cout<<"Resending message form the reciever\n";}
            sleep(5);
        }
    }
    else if (n1 == 0 && n2 > 0)
    {
        char readmsg[20];
        close(sndrFDS1[1]);
        close(sndrFDS2[0]);
        close(ACKFDS1[1]);
        close(ACKFDS2[0]);
        srand(time(0));
        while(true)
        {
            read(sndrFDS1[0],readmsg,sizeof(readmsg));
            int num=rand()%100+1;
            if(num<30)
            {
                cout<<"Message dropped by the channel\n";
                continue;
            }
            cout<<"Message in channel transmitting message to receiver"<<endl;
            sleep(1);
            write(sndrFDS2[1],readmsg,sizeof(readmsg));
            read(ACKFDS1[0],readmsg,sizeof(readmsg));
            num=rand()%100+1;
            if(num<30)
            {
                cout<<"Acknowledge message dropped\n";
                continue;
            }
            cout<<"Transmitting acknowledgment signal\n";
            sleep(1);
            write(ACKFDS2[1],readmsg,sizeof(readmsg));
        }
    }
    else if (n1 > 0 && n2 == 0)
    {
        char readmsg[20];
        close(sndrFDS1[0]);close(sndrFDS1[1]);
        close(sndrFDS2[1]);
        close(ACKFDS1[0]);
        close(ACKFDS2[0]);close(ACKFDS2[1]);
        while(true)
        {
            read(sndrFDS2[0],readmsg,sizeof(readmsg));
            cout<<"Receiver:"<<readmsg<<endl<<"Sending acknowledgment\n";
            sleep(1);
            write(ACKFDS1[1],"+",sizeof("+"));
        }
    }
    return 0;
}
