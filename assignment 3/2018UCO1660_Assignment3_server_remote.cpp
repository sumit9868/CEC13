#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include<fstream>
using namespace std;


void* ActionsByServerOnClient(void * );
void* checkForAbnormalActivities(void*);
int listeningServer = -1;
string portOfServer_removedClient = "";

class Client {
   
    public:
    
    Client(string ip,string port,string name,string clientCode,int socket){
      
        this->port = port;
         this->ip = ip;
        this->name = name;
        this->clientCode = clientCode;
        this->socket = socket;
      
    }
    
    
    Client(){
        
    }

    
    string name;
      string clientCode;
     
      int Attdns=0;
    
    
    string port;
    string ip;
    int socket = -1;
    
   
  
    
    
    void updateClientAttend(){
      
        this->Attdns += 1;
        
      
        fstream stream;
        string fileName = this->clientCode + "_" +this->name+ + ".txt";
        stream.open(fileName,ios::out);
        
        stream<<this->Attdns;
        stream.close();
        
    }
    
  
};


class Clients{
public:
 Client Clients[10];
    int arrayTail = 0;
    
    
    Client addNewClient(string ip,string port,string name,string clientCode,int socket){
        
        Client newClient = Client(ip, port, name,clientCode,socket);
        
        Clients[arrayTail] = newClient;
        arrayTail += 1;
       
        return newClient;
        
    }
    
    
   
    string getAllClientDetails(){
        string displayString = "Nothing to show";
        for(int i = 0;i<10;++i){
            
            if(Clients[i].name == ""){
                continue;
            }
            
            if(displayString == "Nothing to show"){
                displayString = "";
            }
            
            displayString += "IP : "+Clients[i].ip +" :: Port : " +Clients[i].port + " Name : "+ Clients[i].name +" :: clientCode : "+ Clients[i].clientCode + " :: Attendance : " +  to_string(Clients[i].Attdns);
            displayString += "\n";
        }
        
        return displayString;
    }
    
    
    
 
    Client* getClientByPort(string key){
        
        for(int i = 0;i<10;++i){
            
            if(Clients[i].port == key){
                return &Clients[i];
            }
            
        }
        
        return NULL;
        
    }
    
    string getClientDetailsByPort(string key){
        string displayString = "";
          for(int i = 0;i<10;++i){
              
              if(Clients[i].port == key){
                  displayString += "IP : "+Clients[i].ip +" :: Port : " +Clients[i].port +  " Name : "+ Clients[i].name +" :: clientCode : "+ Clients[i].clientCode + " :: Attdns : " +  to_string(Clients[i].Attdns);
                             displayString += "\n";
                  
                  return displayString;
              }
              
          }
          
          return "Nothing to show";
          
      }
    
    
    string getIPandPortList(){
        string displayString = "Nothing to show";
        for(int i = 0;i<10;++i){
          
            if (Clients[i].socket == -1){
                continue;
            }
            
            if(displayString == "Nothing to show") {
                displayString = "";
            }
            
            displayString += "IP : " +Clients[i].ip + " :: Port : "+ Clients[i].port;
            displayString += "\n";
            
        }
        
        return displayString;
    }
    
  
    
    
}AllClients;

 



void *manageClient(void *threadid){
    
           
           sockaddr_in client;
            socklen_t clientSize = sizeof(client);
           
    
       int clientSocket = accept(listeningServer, (sockaddr*)&client, &clientSize);
          
               
       char host[NI_MAXHOST];
       char service[NI_MAXSERV];
    
       memset(host, 0, NI_MAXHOST);
       memset(service, 0, NI_MAXSERV);
    
       if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
       {
          
       }
       else
       {
           inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
         
       }
    
      
    
  
    bool hasLoggedIn = false;
    string clientName = "";
    
  
    string introMessage_ = "\nSetting up your account\n";
    
    string introMessage = introMessage_;
    
     char *mess = &introMessage[0];
      send(clientSocket, mess,introMessage.length(), 0);
     
      Client senderClient = Client();
     
   
        char buffer[4096];
    string quit = "quit\n";
     
        while (true)
        {
            
            if (string(service) == portOfServer_removedClient){
                cout<<"Removing machine at IP"<<host<<" and port"<<service<<endl;
                string mess = "your connection terminated by server. bye! bye!\n";
                send(clientSocket, &mess[0],mess.length(), 0);
                break;
            }
            
            
            memset(buffer, 0, 4096);
     
          
            int recievedData = recv(clientSocket, buffer, 4096, 0);
            if (recievedData == -1)
            {
                cout << "Error in recv(). Quitting" << endl;
                break;
            }
     
            if (recievedData == 0)
            {
                cout << "Client disconnected " << endl;
                break;
            }
            
        
            string clientReq = string(buffer,0,recievedData);
            clientReq += "\n";
            
            
        
            cout<<" Port-> "<<service<<endl;
            cout<<string(buffer, 0, recievedData)<< endl;
          
         
          
            if (!hasLoggedIn){
           
                
               string informationForClient[2];
                int currentPos = 0;
                for(int i = 0;i<recievedData;++i){
                    if(clientReq[i]=='_'){
                        
                        currentPos += 1;
                        continue;
                    }else{
                        informationForClient[currentPos] += clientReq[i];
                    }
                }
                
                
                
                
                
                
                
               
          senderClient  =  AllClients.addNewClient(string(host), string(service), informationForClient[0],informationForClient[1],clientSocket);
                hasLoggedIn = true;
             
                clientName = senderClient.name;
                cout<<"New Client Connected In"<<"::  IP : "<<host<<" :: Port : "<<service<<endl;
                  string notif = "Logged in Completely ";
                
                    send(clientSocket,&notif[0],notif.length(),0);
                
                
                
                continue;
                
            }
            
           
            if (clientReq.compare(quit)==0){
                string closingMess = "Client wants to close the connection. - CLOSING";
                char *mess = &closingMess[0];
                send(clientSocket, mess,closingMess.length(), 0);
                cout<<closingMess<<endl;
                break;
            }
            
            else if (clientReq.compare("1\n")==0){
              string ClientData =  AllClients.getClientDetailsByPort(string(service));
             
                send(clientSocket,&ClientData[0],ClientData.length(),0);
            }
         
            else if (clientReq.compare("2\n")==0){
                        
                Client* Client = AllClients.getClientByPort(service);
                Client->updateClientAttend();
                cout<<Client->name<<":: Port "<<Client->port<<" Attendance Updated";
                string mess= "Attendance updated on server\n";
                 send(clientSocket,&mess[0],mess.length(),0);
          
                                    
                                 }
            
            
            else{
                cout<<"Wrong Request From Client\n";
            }
            
            
          
        }
       
  
  
       // Close the socket
       close(clientSocket);
       
       
    return 0;
       
}


int main(){
  
    
 
     listeningServer = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningServer == -1)
    {
        cout << "Error in creating the ssocket" << endl;
        return -1;
    }
 

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listeningServer, (sockaddr*)&hint, sizeof(hint));
 
    listen(listeningServer, SOMAXCONN);
 
    
    int allowedClients = 10;
    pthread_t threads[allowedClients+1];
      int rc;
      int i;
      
      for( i = 0; i < allowedClients; i++ ) {
     
         rc = pthread_create(&threads[i], NULL,  manageClient, (void *)i);
         
         if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
         }
      }
    
   rc = pthread_create(&threads[i], NULL,  ActionsByServerOnClient, (void *)i);
            
            if (rc) {
               cout << "Error:unable to create thread," << rc << endl;
               exit(-1);
            }
    
    pthread_exit(NULL);
   
 
    return 0;
   
    
    
    
return 0;
}
 


void* ActionsByServerOnClient(void * threadid){

    
    while(true){
       
    cout<<"IP and Port list of connected machines - 1\nGet  details of all clients connected - 2\nGet details of a particular Client - 3\nRemove client - 4"<<endl;
    
        
    
        int input = -1;
        cin>>input;
        
        if(input == 1){
          
             cout<<"Requested List is Below"<<endl;
           cout<<AllClients.getIPandPortList()<<endl;
           
        }
        else  if(input == 2){
            cout<<AllClients.getAllClientDetails()<<endl;
        }
        else  if(input == 3){
            cout<<"Enter port of the machine\n";
            string inputPort = "";
            cin>>inputPort;
            cout<<AllClients.getClientDetailsByPort(inputPort)<<endl;
            
               }
        else  if(input == 4){
            cout<<"Here is the list of active computers"<<endl;
            cout<<AllClients.getIPandPortList()<<endl;
            cout<<"Enter in Port of the machine that you want to remove "<<endl;
            string inputPort = "";
            cin>>inputPort;
            portOfServer_removedClient = inputPort;
          
               }
      
   
    
    
    }
    
    
    return 0;
}




