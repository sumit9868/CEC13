#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <iostream>
using namespace std;
void sighup(int signum){
    cout<<"recieved signal "<<signum<<"form SIGHUP";
    exit(signum);
} 
void sigint(int signum){
    cout<<"recieved signal "<<signum<<"form SIGINT";
    exit(signum);
}
void sigquit(int signum){
    cout<<"recieved signal "<<signum<<"form SIGQUIT";
    exit(signum);
}
  
// driver code 
int main() 
{ 
    int Cid; 
  
    if ((Cid = fork()) < 0) { 
        perror("fork"); 
        exit(1); 
    } 
  
    if (Cid == 0) { 
        signal(SIGHUP, sighup); 
        signal(SIGINT, sigint); 
        signal(SIGQUIT, sigquit); 
        for (;;) 
            ; /* loop for ever */
    } 
  
    else 
    { 
        
        cout<<"\nPARENT: sending SIGHUP\n\n"; 
        sleep(2);
        kill(Cid, SIGHUP); 
  
        
        cout<<"\nPARENT: sending SIGINT\n\n"; 
        sleep(2);
        kill(Cid, SIGINT); 
  
        
        cout<<"\nPARENT: sending SIGQUIT\n\n"; 
        sleep(2);
        kill(Cid, SIGQUIT); 
        
    } 

    return 0;
} 
  
