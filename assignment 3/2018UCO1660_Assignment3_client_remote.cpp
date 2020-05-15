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
int attd = 0;

void userDataToFile(string userCred,string attd){
    fstream fstrm;

    fstrm.open(userCred+".txt",ios::out);
    string data = userCred +" : " + " Attendance : "+attd+"\n";
    fstrm>>data;
    fstrm.close();
}


string srvrResponse(char*bffr,int sock);

int attachClientToServer(string accntData){

    bool loggedIn = false;
       int sock = socket(AF_INET, SOCK_STREAM, 0);
       if (sock == -1)
       {
           return 1;
       }
       int port = 54000;
       string ipAddress = "0.0.0.0";

       sockaddr_in hint;
       hint.sin_family = AF_INET;
       hint.sin_port = htons(port);
       inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

       int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
       if (connectRes == -1)
       {
           return 1;
       }


       char bffr[4096];
       string srverRqst;


     cout<<srvrResponse(bffr,sock);


       do {
            if(loggedIn){
           cout<<"\nSee your saved information - 1"<<endl;
             cout<<"Attend a class -2"<<endl;



           cout << "YOU-> ";
           cin>>srverRqst;


                if(srverRqst == "1"){

                }

          else  if(srverRqst == "2"){

               attd += 1;
               string atten = to_string(attd);
               userDataToFile(accntData,atten);


           }
           else if(srverRqst == "3"){
               cout<<"DISCONNECTED FROM SERVER"<<endl;
               break;

           }else{
               cout<<"Wrong input\n";
               continue;
           }






            }else{
                srverRqst = accntData;
                              loggedIn = true;
            }




           int sendRes = send(sock, srverRqst.c_str(), srverRqst.size() + 1, 0);
           if (sendRes == -1)
           {
               cout << "Could not send to server\n";
               continue;
           }

           cout<<srvrResponse(bffr,sock);
       } while(true);

      

    close(sock);

       return 0;
}


int main()
{


    cout<<"To Register yourself."<<endl;
    string name,code;
    cout<<"Enter your name"<<endl;
    cin>>name;
    cout<<"Enter your code"<<endl;
    cin>>code;


    string accntData = name+"_"+code;




    attachClientToServer(accntData);
}



string srvrResponse(char*bffr,int sock){
    
              memset(bffr, 0, 4096);
              int bytsRcvd = recv(sock, bffr, 4096, 0);
              if (bytsRcvd == -1)
              {
                 
                  return "Error while getting a response";
              }
              else
              {
                  return string(bffr, bytsRcvd);
              }


}


