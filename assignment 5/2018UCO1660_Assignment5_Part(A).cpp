#include<iostream>
#include<string>
using namespace std;

int main()
{
    cout<<"String:";
    string str;
    cin>>str;
    if(str.length()<8){cout<<"Invalid Input";return 0;}
    for(int i=0;i<str.length();i++)
    {
        if(str[i]!='0' && str[i]!='1'){cout<<"Invalid Input";return 0;}
    }
    string res=str.substr(0,6);
    string prv=res;
    for(int i=6;i<str.length();i++)
    {
        if(prv=="011111")
        {
            res+='0';
        }
        res+=str[i];
        prv=prv.substr(1)+str[i];
    }
    cout<<"Output:"<<res;
    string input=res.substr(0,6);
    prv=input;
    for(int i=6;i<res.length();i++)
    {
        if(prv!="011111"){input+=res[i];}
        prv=prv.substr(1)+res[i];
    }
    cout<<"\nExtracted Input String:"<<input;
}
