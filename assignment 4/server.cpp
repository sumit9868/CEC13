#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int clientHandler();
int listening = -1;

class user {

    public:
    string port;
    string IP;
    string name = "";
    int socket = -1;
    int groupNo = 0;

    user(){

    }

    user(string IP,string port,string name,int socket){

        this->port = port;
        this->IP = IP;
        this->name = name;
        this->socket = socket;
        this->groupNo = rand()%2;
    }

};


class users{
public:
 user users[10];
    int Position = 0;

    user addNewuser(string IP,string port,string name,int socket){

        user newuser = user(IP, port, name,socket);

        users[Position] = newuser;
        Position += 1;

        return newuser;

    }


    string getAlluserNames(){
        string userList = "";
        for(int i = 0;i<10;++i){

            if(users[i].name == ""){
                continue;
            }

            userList += users[i].name + "of Group : " + to_string(users[i].groupNo);
            userList += "\n";
        }

        return userList;
    }

    user* getuserByPort(string prt){

        for(int i = 0;i<10;++i){

            if(users[i].port == prt){
                return &users[i];
            }

        }

        return NULL;

    }


    user* getuserByName(string name){

           for(int i = 0;i<10;++i){

               if(users[i].name == name){
                   return &users[i];
               }

           }

           return NULL;

       }


}Systemusers;


class MessagesSystem {

public:  MessagesSystem(){
        for(int i =0;i<100;++i){
            messages[i] = Message();
        }
    }

    class Message {

    public:
        
        string message;
        user* sender = NULL;
        user* reciever = NULL;

        Message(){

        }

          Message(string msg,user* sndr, user* rcvr){
            this->message = msg;
            this->sender = sndr;
            this->reciever = rcvr;
          }

      };


    Message messages[100];
    int Position = 0;

    bool addMessage(string msg,string sndrPrt,string rcvrName){
       
        user* sender = Systemusers.getuserByPort(sndrPrt);
        user* reciever = Systemusers.getuserByName(rcvrName);

        if (sender != NULL && reciever != NULL){
        Message newMessage = Message(msg, sender, reciever);

        messages[Position] = newMessage;
            Position += 1;

            return true;

        }else{
            cout<<"Sender or reciever not found\n";
                return false;
            }

    }

}SystemMessages;



void *handleSingleClient(void *Tid){


           sockaddr_in client;
            socklen_t clientSize = sizeof(client);

       cout<<"Waiting for connection\n";
       int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
       cout<<"Connected successfuly\n";
   

       char host[NI_MAXHOST];      
       char service[NI_MAXSERV];  

       memset(host, 0, NI_MAXHOST); 
       memset(service, 0, NI_MAXSERV);

       if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
       {
           cout << host << " \nconnected on port :" << service << endl;
       }
       else
       {
           inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
           cout << host << " connected on port :" << ntohs(client.sin_port) << endl;
       }



    bool accountCreated = false;
    string clientName = "";

    string clientMsg = "Welcome to the LAN Chat Application";

     char *msg = &clientMsg[0];
      send(clientSocket, msg,clientMsg.length(), 0);


       char bffr[4096];
       char quit[] = {'q','u','i','t'};

        while (true)
        {
            memset(bffr, 0, 4096);

            int bytesReceived = recv(clientSocket, bffr, 4096, 0);
            if (bytesReceived == -1)
            {
                cout << "Error in recv().\n Quitting" << endl;
                break;
            }

            if (bytesReceived == 0)
            {
                cout << "Client disconnected " << endl;
                break;
            }

            cout<<string(bffr, 0, bytesReceived) <<" :: By "<<host<<" Port "<<service<< endl;

            user senderuser = user();
            if (!accountCreated){
          senderuser  =  Systemusers.addNewuser(string(host), string(service), string(bffr, 0, bytesReceived),clientSocket);
                accountCreated = true;

                clientName = senderuser.name;

                 string notif = "New user - " +clientName+ " joined. Press 1 to see the user's list\n";
                for(int i = 0;i<10;++i){
                    int notifSocket = Systemusers.users[i].socket;
                    if (notifSocket == -1){
                        continue;
                    }
                    send(Systemusers.users[i].socket,&notif[0],notif.length(),0);
                }




            }else{
                string clientRespone = string(bffr, 0, bytesReceived);
                string message = "";
                int counter = 0;
                    for(int i = 0;i<clientRespone.length();++i){

                        if(clientRespone[i] != '@'){
                            message += clientRespone[i];
                        }else{
                            break;
                        }

                        counter += 1;

                    }

                counter += 1;
                if (counter<bytesReceived){
                string recName = string(bffr, counter, bytesReceived);

                    if (bytesReceived-counter  == 2 ){
                         string recMessage = message + " - Group Message By " + clientName;
                        for(int i = 0;i<10;++i){
                                          int notifSocket = Systemusers.users[i].socket;
                            int groupNo = Systemusers.users[i].groupNo;
                            string groupNoStr = to_string(groupNo) + "\n";
                                          if (notifSocket == -1 || groupNoStr != recName){
                                              continue;
                                          }
                                          send(Systemusers.users[i].socket,&recMessage[0],recMessage.length(),0);
                                      }


                    }else{
               

                SystemMessages.addMessage(message, service, recName);
               user* recuser =  Systemusers.getuserByName(recName);
              
                cout<<"Sender Name"<<clientName<<endl;

                string recMessage = message + " -  By " + clientName;
                  send(recuser->socket, &recMessage[0],recMessage.length(), 0);
                }
                }

            }


            string clientReq = string(bffr,0,bytesReceived);
            string quitStr = string(quit,0,4);
           quitStr += "\n";


            if (clientReq.compare(quitStr)==0){
                string closingMsg = "Client closing the connection";
                char *msg = &closingMsg[0];
                send(clientSocket, msg,closingMsg.length(), 0);
                cout<<closingMsg<<endl;
                break;
            }

            else if (clientReq.compare("1\n")==0){
                string newList = Systemusers.getAlluserNames();
                send(clientSocket,&newList[0],newList.length(),0);
            }

            


        }


       close(clientSocket);


    return 0;

}


int main(){

     clientHandler();

    return 0;
}


int clientHandler()
{

     listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cout << "Error creating a socket! Quitting" << endl;
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    cout<<"Starting to listening"<<endl;
    listen(listening, SOMAXCONN);
    cout<<"Started listnening"<<endl;


    int maxClients = 10;
    pthread_t threads[maxClients];
      int rc;
      int i;

      for( i = 0; i < maxClients; i++ ) {
         void* arg;
          arg=&i;
          rc = pthread_create(&threads[i], NULL,  handleSingleClient, arg);

         if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
         }
      }
      pthread_exit(NULL);

    return 0;
}

