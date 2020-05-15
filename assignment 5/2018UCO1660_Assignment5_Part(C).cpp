#include<iostream>
#include<string>
using namespace std;

string divide(string curr,string gen)
{
    if(curr[0]=='0'){return curr.substr(1);}
    string ans="";
    for(int i=1;i<gen.length();i++)
    {
        if(gen[i]==curr[i]){ans+='0';}
        else{ans+='1';}
    }
    return ans;
}

int main()
{
    string data,gen;
    cout<<"enter the Data AND  Generator\n";cin>>data>>gen;
    string tmpdata=data;
    if(gen.length()>data.length()){cout<<"INVALID";return 0;}
    for(int i=1;i<gen.length();i++){tmpdata+='0';}
    string curr=tmpdata.substr(0,gen.length());
    curr=divide(curr,gen);
    for(int i=gen.length();i<tmpdata.length();i++)
    {
        curr+=tmpdata[i];
        curr=divide(curr,gen);
    }
    string dataword=data+curr;
    cout<<"Dataword:"<<dataword;
    curr=dataword.substr(0,gen.length());
    curr=divide(curr,gen);
    for(int i=gen.length();i<dataword.length();i++)
    {
        curr+=dataword[i];
        curr=divide(curr,gen);
    }
    for(int i=0;i<curr.length();i++)
    {
        if(curr[i]=='1'){cout<<"\nERROR DETECTED";return 0;}
    }
    cout<<"\nNO ERROR DETECTED";
}
