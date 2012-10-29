#include<pcap.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<netinet/udp.h>	
#include<netinet/ip.h>

struct sockaddr_in source, dest;
struct udp_header
{
    int src_port;
    int dest_port;
    int udp_length;
    int udp_checksum;
};

//recived packet
static int packet_count = 0;

//was resended
static int resended_count = 0;

//show UDP headers
void _PrintUdpPacket(const u_char *buffer, int size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof (struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr*) (buffer + iphdrlen + sizeof (struct ethhdr));
    int header_size = sizeof (struct ethhdr) +iphdrlen + sizeof udph;
    fprintf(stderr, "\nUDP Header\n");
    fprintf(stderr, "   |-Source Port      : %d\n", ntohs(udph->source));
    fprintf(stderr, "   |-Destination Port : %d\n", ntohs(udph->dest));
    fprintf(stderr, "   |-UDP Length       : %d\n", ntohs(udph->len));
    fprintf(stderr, "   |-UDP Checksum     : %d\n", ntohs(udph->check));
    fprintf(stderr, "\n");
}
//show IP headers
void _PrintIpHeader(const u_char * buffer, int Size)
{
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof (struct ethhdr));
    iphdrlen = iph->ihl * 4;
    memset(&source, 0, sizeof (source));
    source.sin_addr.s_addr = iph->saddr;

    memset(&dest, 0, sizeof (dest));
    dest.sin_addr.s_addr = iph->daddr;

    fprintf(stderr, "\n");
    fprintf(stderr, "IP Header\n");
    fprintf(stderr, "   |-IP Version        : %d\n", (unsigned int) iph->version);
    fprintf(stderr, "   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int) iph->ihl, ((unsigned int) (iph->ihl))*4);
    fprintf(stderr, "   |-Type Of Service   : %d\n", (unsigned int) iph->tos);
    fprintf(stderr, "   |-IP Total Length   : %d  Bytes(Size of Packet)\n", ntohs(iph->tot_len));
    fprintf(stderr, "   |-Identification    : %d\n", ntohs(iph->id));
    fprintf(stderr, "   |-TTL      : %d\n", (unsigned int) iph->ttl);
    fprintf(stderr, "   |-Protocol : %d\n", (unsigned int) iph->protocol);
    fprintf(stderr, "   |-Checksum : %d\n", ntohs(iph->check));
    fprintf(stderr, "   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    fprintf(stderr, "   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
}

void _SetHeader(struct udp_header * packet, int dest, int src, int length, int check)
{
    packet->dest_port = dest;
    packet->src_port = src;
    packet->udp_length = length;
    packet->udp_checksum = check;
}

int _HeaderEmpty(struct udp_header * packet)
{
    if (packet->dest_port == 0 &&
         packet->udp_checksum == 0 &&
         packet->udp_length == 0)
        return 1;
    else
        return 0;
            
}
// Check if packet was send and recive saccesfully
void _ChekHeader(const u_char *buffer, int size, int count)
{
    static struct udp_header packet_recv = {0, 0, 0, 0}, packet_send = {0, 0, 0, 0};
    unsigned short iphdrlen;
    struct iphdr *iph = (struct iphdr *) (buffer + sizeof (struct ethhdr));
    iphdrlen = iph->ihl * 4;
    struct udphdr *udph = (struct udphdr*) (buffer + iphdrlen + sizeof (struct ethhdr));

/*
    fprintf(stderr, "recv%d\n", sizeof(packet_send));
*/
    if (_HeaderEmpty((struct udp_header *) &packet_recv))
    {
        _SetHeader((struct udp_header *)  &packet_recv, ntohs(udph->dest), ntohs(udph->source), ntohs(udph->len), ntohs(udph->check));
        if (packet_recv.dest_port == packet_send.dest_port)
            resended_count++;
        memset(&packet_send, 0, sizeof(packet_send));
        return;
    }
    _SetHeader((struct udp_header *)  &packet_send, ntohs(udph->dest), ntohs(udph->source), ntohs(udph->len), ntohs(udph->check));

    if (packet_recv.dest_port == packet_send.src_port &&
            packet_recv.udp_checksum == packet_send.udp_checksum &&
            packet_recv.udp_length == packet_send.udp_length
            )
        packet_count++;
    memset(&packet_recv, 0, sizeof(packet_recv));
}

// Callback function which work any time when recived packets
void LoopCallback(u_char *arg, const struct pcap_pkthdr* pkthdr,
                      const u_char* packet)
{
    static int count = 0;
    int size = pkthdr->len;
    
    _PrintUdpPacket(packet, size);
    _PrintIpHeader(packet, size);

    printf("All packet Count: %d\n", ++count);
    printf("Recieved Packet Size: %d\n", pkthdr->len);
    printf("Success Packet Recieved---: %d\n", packet_count + 1);
    printf("Resended Packet---: %d\n", resended_count);
    _ChekHeader(packet, size, count);
}

int main(int argc, char **argv)
{
    int i;
    char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
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

    pcap_loop(descr, -1, LoopCallback, NULL);
    return 0;
}
