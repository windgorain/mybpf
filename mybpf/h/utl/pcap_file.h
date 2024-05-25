/*================================================================
*   Created：2018.08.22
*   Description：
*
================================================================*/
#ifndef _PCAP_FILE_H
#define _PCAP_FILE_H
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    UINT magic;
    USHORT major;
    USHORT minor;
    UINT this_zone;
    UINT sig_figs;
    UINT snaplen;
    UINT link_type;
}PCAPFILE_HEADER_S;

typedef struct {
    UINT64 time;
    UINT cap_len;
    UINT pkt_len;
}PCAP_PKT_HEADER_S;

#pragma pack(1)
typedef struct {
    USHORT packet_type;
    USHORT link_type;
    USHORT link_len;
    UCHAR  address[8];
    USHORT protocol;
}PCAP_LINUX_COOKED_S;
#pragma pack()

FILE * PCAPFILE_Open(char *pcFileName, char *flag);
void PCAPFILE_WriteHeader(FILE *fp, int linktype);
int PCAPFILE_WritePkt(FILE *fp, void *pkt, void *ts, UINT cap_len, UINT pkt_len);
int PCAPFILE_ReadHeader(FILE *fp, OUT PCAPFILE_HEADER_S *header);
int PCAPFILE_ReadPkt(FILE *fp, OUT void *data, IN UINT data_size, OUT PCAP_PKT_HEADER_S *header);
void PCAPFILE_Close(FILE *fp);


#ifdef __cplusplus
}
#endif
#endif 
