#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <unistd.h> 
using namespace std;
void createChild(){
    if(fork()==0){
cout<<"Id of the child process:"<<getpid()<<endl;
cout<<"Id of the parent process of current child:"<<getppid()<<endl<<endl;
int timer=rand()%5;
cout<<"the child process: "<<getpid()<<" slept for "<<timer<<"seconds\n";
sleep(timer);
cout<<"the child process: "<<getpid()<<" is awake now\n";
    }else{
cout<<"Id of the parent process:"<<getpid()<<endl;
cout<<"Id of the parent process of current parent:"<<getppid()<<endl<<endl;
    int child_id;
        int status;
        child_id=wait(&status);
        if(WIFEXITED(status)==true)
        {
            cout<<"child process id:"<<child_id<<"\nterminated with status:"<<WEXITSTATUS(status);
            if(WEXITSTATUS(status)==0)
            {
                cout<<" =>Child process commands successful";
            }
            if(WEXITSTATUS(status)==127)
            {
                cout<<" =>Invalid path";
            }
        }
    }
}
int main(){
createChild();
return 0;
}