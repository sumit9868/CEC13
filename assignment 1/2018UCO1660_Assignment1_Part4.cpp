#include <pcap.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string>
using namespace std;
int main(){
 string file = "mypack.pcap";

    char errbuff[PCAP_ERRBUF_SIZE];

   
    pcap_t * pcap = pcap_open_offline(file.c_str(), errbuff);

   struct pcap_pkthdr *header;

   const u_char *data;
   u_int packetCnt = 0;
    while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
    {
        printf("Packet # %i\n", ++packetCnt);
     printf("Packet size: %d bytes\n", header->len);

        if (header->len != header->caplen)
            printf("Error! Capture size != packet size: %ld bytes\n", header->len);
       printf("Epoch Time: %d:%d seconds\n", header->ts.tv_sec, header->ts.tv_usec);
       for (u_int i=0; (i < header->caplen ) ; i++)
        {
            if ( (i % 16) == 0) printf("\n");
        printf("%.2x ", data[i]);
        }

        printf("\n\n");
    }
}
