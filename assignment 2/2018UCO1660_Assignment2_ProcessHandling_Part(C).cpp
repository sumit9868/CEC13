#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
void sigHandler(int signo){
if(signo == SIGINT)
printf("\nSignal recieved:%d",signo);
return;
}

int main(void){
pid_t pid, ppid;
ppid = getpid();
printf("%d \n", ppid);
if((pid = fork()) == 0){ 
kill(ppid, SIGINT);
}
else{
printf("%d %d \n",ppid, pid);
struct sigaction sig;
sigemptyset(&sig.sa_mask);
sig.sa_flags = 0;
sig.sa_handler = sigHandler;
if(sigaction(SIGINT,&sig,NULL) == 0)
printf("Signal processed successfully ");
sleep(10);
}
}
