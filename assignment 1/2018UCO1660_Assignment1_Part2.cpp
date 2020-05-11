#include<fstream>
#include<iostream>
#include<vector>
using namespace std;
int main()
{
    ifstream f;
    f.open("mypack.pcap");
    unsigned char temp;
    int c=1;
    vector<unsigned char> MAC_Dest_add;
    vector<unsigned char> MAC_Src_add;
    vector<unsigned char> IP_Hdr_CkSum;
    vector<unsigned char> IP_Src_add;
    vector<unsigned char> IP_Dest_add;
    vector<unsigned char> TCP_Src_Port;
    vector<unsigned char> TCP_Dest_Port;
    vector<unsigned char> TCP_Seq_No;
    vector<unsigned char> TCP_Ack_No;
    vector<unsigned char> SYN;
    while(f.good())
    {
        f>>temp;
        if(c>=41 &&c<=46){MAC_Dest_add.push_back(temp);}
        else if(c>=47 && c<=52){MAC_Src_add.push_back(temp);}
        else if(c==65 || c==66){IP_Hdr_CkSum.push_back(temp);}
        else if(c>=67 && c<=70){IP_Src_add.push_back(temp);}
        else if(c>=71 && c<=74){IP_Dest_add.push_back(temp);}
        else if(c==75 || c==76){TCP_Src_Port.push_back(temp);}
        else if(c==77 || c==78){TCP_Dest_Port.push_back(temp);}
        else if(c>=79 && c<=82){TCP_Seq_No.push_back(temp);}
        else if(c>=83 && c<=85){TCP_Ack_No.push_back(temp);}
        else if(c==87){SYN.push_back(temp);}
        c++;
    }
    cout<<"\nDestination mac:";
    for(int i=0;i<MAC_Dest_add.size();i++){printf("%X ",MAC_Dest_add[i]);}
    cout<<"\nSource mac:";
    for(int i=0;i<MAC_Src_add.size();i++){printf("%X ",MAC_Src_add[i]);}
    cout<<"\nIP Checksum:";
    for(int i=0;i<IP_Hdr_CkSum.size();i++){printf("%X ",IP_Hdr_CkSum[i]);}
    cout<<"\nIP Source:";
    for(int i=0;i<IP_Src_add.size();i++){printf("%X ",IP_Src_add[i]);}
    cout<<"\nDestination IP:";
    for(int i=0;i<IP_Dest_add.size();i++){printf("%X ",IP_Dest_add[i]);}
    cout<<"\nSource TCP port:";
    for(int i=0;i<TCP_Src_Port.size();i++){printf("%X ",TCP_Src_Port[i]);}
    cout<<"\nDestination TCP port:";
    for(int i=0;i<TCP_Dest_Port.size();i++){printf("%X ",TCP_Dest_Port[i]);}
    cout<<"\nSequence Number:";
    for(int i=0;i<TCP_Seq_No.size();i++){printf("%X ",TCP_Seq_No[i]);}
    cout<<"\nAcknowledgment Number:";
    for(int i=0;i<TCP_Ack_No.size();i++){printf("%X ",TCP_Ack_No[i]);}
    cout<<"\nSYN Bit:";
    for(int i=0;i<MAC_Src_add.size();i++){printf("%X ",SYN[i]);}
}
