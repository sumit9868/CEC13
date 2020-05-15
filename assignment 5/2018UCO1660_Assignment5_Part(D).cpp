#include<iostream>
using namespace std;

int main() {
    int data[10];
    int recvdData[10],c,c1,c2,c3,i;
    cout<<"Enter 4 bits of data one at a time:";
    cin>>data[0];
    cin>>data[1];
    cin>>data[2];
    cin>>data[4];
    data[6]=data[0]^data[2]^data[4];
	data[5]=data[0]^data[1]^data[4];
	data[3]=data[0]^data[1]^data[2];
	cout<<"\nData after encoding:";
	for(i=0;i<7;i++){cout<<data[i];}
	cout<<"\n\nEnter received data bits one at a time:";
    for(i=0;i<7;i++){cin>>recvdData[i];}
    c1=recvdData[6]^recvdData[4]^recvdData[2]^recvdData[0];
	c2=recvdData[5]^recvdData[4]^recvdData[1]^recvdData[0];
	c3=recvdData[3]^recvdData[2]^recvdData[1]^recvdData[0];
	c=c3*4+c2*2+c1 ;
    if(c==0) {cout<<"\nNo error\n";}
	else
    {
		cout<<"\nError on position:"<<c;
		cout<<"\nData sent:";
		for(i=0;i<7;i++){cout<<data[i];}
		cout<<"\nData received:";
        for(i=0;i<7;i++){cout<<recvdData[i];}
		cout<<"\nCorrect message is:";
		if(recvdData[7-c]==0){recvdData[7-c]=1;}
        else{recvdData[7-c]=0;}
		for (i=0;i<7;i++){cout<<recvdData[i];}
	}
	return 0;
}
