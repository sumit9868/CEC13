#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

int main() {

   char* c = new char;
   int   cCount = 0;
//F:\semester 4\computer networking\lab practical\2018UCO1658_Assignment1_Part3.cpp
   ifstream f("F:\\Semester 4\\computer networking\\lab practical\\mypacket.pcap", ios::in | ios::binary); 
   if ( f.fail() ) {
      cout << "F:\\Semester 4\\computer networking\\lab practical\\mypacket.pcap" << endl;
      return 1;
   }

   cout << "Hex dump of file " << "F:\\Semester 4\\computer networking\\lab practical\\mypacket.pcap" << ":" << endl << endl;
   string Head = "  00  01  02  03  04  05  06  07      08  09  0A  0B  0C  0D  0E  0F";
   cout << Head << endl;
    unsigned short buf[20];
    int i=0;
    int k = 1;
   f.read(c, 1);
   cout << setfill('0') << uppercase;

   while ( f ) {

      cout << "  " << hex << setw(2)
           << (short(*c) & 0x00FF);
           if(k>=55&&k<=74)
            { buf[i] = (short)(*c);
            i++;
             }

      cCount++;
      if (cCount % 8 == 0) {
         if (cCount % 16 == 0)
            cout << endl;
         else
            cout << "    ";
      }
      f.read(c, 1);
      k++;
   }
   cout << endl;
   f.close();
    unsigned short total =0;
    for(int i=0;i<20;i++){
        total+=buf[i];
        if(total & 0xFFFF0000){
           // total = total & 0xFFFF;
            total++;
        }
    }

    cout<<"Checksum :      "<<1+(~(total&0xFFFF0000));


   return 0;
}
