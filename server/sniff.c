#include<pcap.h>
#include<stdio.h>
#include<stdlib.h> // for exit()
#include<string.h> //for memset

#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header

void print_udp_packet(const u_char *Buffer, int Size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *) (Buffer + sizeof (struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr*) (Buffer + iphdrlen + sizeof (struct ethhdr));
    int header_size = sizeof (struct ethhdr) +iphdrlen + sizeof udph;
    fprintf(stderr, "\nUDP Header\n");
    fprintf(stderr, "   |-Source Port      : %d\n", ntohs(udph->source));
    fprintf(stderr, "   |-Destination Port : %d\n", ntohs(udph->dest));
    fprintf(stderr, "   |-UDP Length       : %d\n", ntohs(udph->len));
    fprintf(stderr, "   |-UDP Checksum     : %d\n", ntohs(udph->check));
    fprintf(stderr, "\n");
}

void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr,
                      const u_char* packet)
{
    int i = 0;
    static int count = 0;
    int size = pkthdr->len;
    print_udp_packet(packet, size);

    printf("Packet Count: %d\n", ++count); /* Количество пакетов */
    printf("Recieved Packet Size: %d\n", pkthdr->len); /* Длина заголовка */
}

int main(int argc, char **argv)
{
    int i;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    const u_char *packet;
    struct pcap_pkthdr hdr;
    struct ether_header *eptr;
    struct bpf_program fp;
    bpf_u_int32 maskp;
    bpf_u_int32 netp;

    if (argc != 2)
    {
        fprintf(stdout, "Usage: %s \"expression\"\n"
                , argv[0]);
        return 0;
    }
    /*
        dev = pcap_lookupdev(errbuf); 
     */
    //This section we will listen localhost
    dev = "lo";

    if (dev == NULL)
    {
        fprintf(stderr, "%s\n", errbuf);
        exit(1);
    }
    pcap_lookupnet(dev, &netp, &maskp, errbuf);

    descr = pcap_open_live(dev, BUFSIZ, 1, -1, errbuf);
    if (descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    if (pcap_compile(descr, &fp, argv[1], 0, netp) == -1)
    {
        fprintf(stderr, "Error calling pcap_compile\n");
        exit(1);
    }

    if (pcap_setfilter(descr, &fp) == -1)
    {
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }

    pcap_loop(descr, -1, another_callback, NULL);
    return 0;
}
