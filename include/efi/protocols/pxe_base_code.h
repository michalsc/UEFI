#ifndef INCLUDE_EFI_PROTOCOLS_PXE_BASE_CODE_H_
#define INCLUDE_EFI_PROTOCOLS_PXE_BASE_CODE_H_

#include <efi/types.h>
#include <efi/memory.h>

#define EFI_PXE_BASE_CODE_PROTOCOL_GUID \
    { 0x03C4E603, 0xAC28, 0x11d3,\
    { 0x9A, 0x2D, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D }}

#define EFI_PXE_BASE_CODE_PROTOCOL_REVISION 0x00010000

//*******************************************************
// Maximum ARP and Route Entries
//*******************************************************
#define EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES 8
#define EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES 8

//*******************************************************
// EFI_PXE_BASE_CODE_DHCPV4_PACKET
//*******************************************************
typedef struct {
    UINT8 BootpOpcode;
    UINT8 BootpHwType;
    UINT8 BootpHwAddrLen;
    UINT8 BootpGateHops;
    UINT32 BootpIdent;
    UINT16 BootpSeconds;
    UINT16 BootpFlags;
    UINT8 BootpCiAddr[4];
    UINT8 BootpYiAddr[4];
    UINT8 BootpSiAddr[4];
    UINT8 BootpGiAddr[4];
    UINT8 BootpHwAddr[16];
    UINT8 BootpSrvName[64];
    UINT8 BootpBootFile[128];
    UINT32 DhcpMagik;
    UINT8 DhcpOptions[56];
} EFI_PXE_BASE_CODE_DHCPV4_PACKET;

//***********************************************
// DHCPV6 Packet structure
//***********************************************
typedef struct {
    UINT32 MessageType:8;
    UINT32 TransactionId:24;
    UINT8 DhcpOptions[1024];
} EFI_PXE_BASE_CODE_DHCPV6_PACKET;

//*******************************************************
// EFI_PXE_BASE_CODE_PACKET
//*******************************************************
typedef union {
    UINT8 Raw[1472];
    EFI_PXE_BASE_CODE_DHCPV4_PACKET Dhcpv4;
    EFI_PXE_BASE_CODE_DHCPV6_PACKET Dhcpv6;
} EFI_PXE_BASE_CODE_PACKET;

#define EFI_PXE_BASE_CODE_MAX_IPCNT 8
//*******************************************************
// EFI_PXE_BASE_CODE_IP_FILTER
//*******************************************************
typedef struct {
    UINT8 Filters;
    UINT8 IpCnt;
    UINT16 reserved;
    EFI_IP_ADDRESS IpList[EFI_PXE_BASE_CODE_MAX_IPCNT];
} EFI_PXE_BASE_CODE_IP_FILTER;
#define EFI_PXE_BASE_CODE_IP_FILTER_STATION_IP 0x0001
#define EFI_PXE_BASE_CODE_IP_FILTER_BROADCAST 0x0002
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS 0x0004
#define EFI_PXE_BASE_CODE_IP_FILTER_PROMISCUOUS_MULTICAST 0x0008

//*******************************************************
// EFI_PXE_BASE_CODE_ARP_ENTRY
//*******************************************************
typedef struct {
    EFI_IP_ADDRESS IpAddr;
    EFI_MAC_ADDRESS MacAddr;
} EFI_PXE_BASE_CODE_ARP_ENTRY;

//*******************************************************
// EFI_PXE_BASE_CODE_ROUTE_ENTRY
//*******************************************************
typedef struct {
    EFI_IP_ADDRESS IpAddr;
    EFI_IP_ADDRESS SubnetMask;
    EFI_IP_ADDRESS GwAddr;
} EFI_PXE_BASE_CODE_ROUTE_ENTRY;

//*******************************************************
// EFI_PXE_BASE_CODE_ICMP_ERROR
//*******************************************************
typedef struct {
    UINT8 Type;
    UINT8 Code;
    UINT16 Checksum;
    union {
        UINT32 reserved;
        UINT32 Mtu;
        UINT32 Pointer;
        struct {
        UINT16 Identifier;
        UINT16 Sequence;
        } Echo;
    } u;
    UINT8 Data[494];
} EFI_PXE_BASE_CODE_ICMP_ERROR;

//*******************************************************
// EFI_PXE_BASE_CODE_TFTP_ERROR
//*******************************************************
typedef struct {
    UINT8 ErrorCode;
    CHAR8 ErrorString[127];
} EFI_PXE_BASE_CODE_TFTP_ERROR;

//*******************************************************
// EFI_PXE_BASE_CODE_MODE
//
// The data values in this structure are read-only and
// are updated by the code that produces the
// EFI_PXE_BASE_CODE_PROTOCOLfunctions. //
// *******************************************************
typedef struct {
    BOOLEAN Started;
    BOOLEAN Ipv6Available;
    BOOLEAN Ipv6Supported;
    BOOLEAN UsingIpv6;
    BOOLEAN BisSupported;
    BOOLEAN BisDetected;
    BOOLEAN AutoArp;
    BOOLEAN SendGUID;
    BOOLEAN DhcpDiscoverValid;
    BOOLEAN DhcpAckReceivd;
    BOOLEAN ProxyOfferReceived;
    BOOLEAN PxeDiscoverValid;
    BOOLEAN PxeReplyReceived;
    BOOLEAN PxeBisReplyReceived;
    BOOLEAN IcmpErrorReceived;
    BOOLEAN TftpErrorReceived;
    BOOLEAN MakeCallbacks;
    UINT8 TTL;
    UINT8 ToS;
    EFI_IP_ADDRESS StationIp;
    EFI_IP_ADDRESS SubnetMask;
    EFI_PXE_BASE_CODE_PACKET DhcpDiscover;
    EFI_PXE_BASE_CODE_PACKET DhcpAck;
    EFI_PXE_BASE_CODE_PACKET ProxyOffer;
    EFI_PXE_BASE_CODE_PACKET PxeDiscover;
    EFI_PXE_BASE_CODE_PACKET PxeReply;
    EFI_PXE_BASE_CODE_PACKET PxeBisReply;
    EFI_PXE_BASE_CODE_IP_FILTER IpFilter;
    UINT32 ArpCacheEntries;
    EFI_PXE_BASE_CODE_ARP_ENTRY ArpCache[EFI_PXE_BASE_CODE_MAX_ARP_ENTRIES];
    UINT32 RouteTableEntries;
    EFI_PXE_BASE_CODE_ROUTE_ENTRY RouteTable[EFI_PXE_BASE_CODE_MAX_ROUTE_ENTRIES];
    EFI_PXE_BASE_CODE_ICMP_ERROR IcmpError;
    EFI_PXE_BASE_CODE_TFTP_ERROR TftpError;
} EFI_PXE_BASE_CODE_MODE;

//*******************************************************
// EFI_PXE_BASE_CODE_UDP_PORT
//*******************************************************
typedef UINT16 EFI_PXE_BASE_CODE_UDP_PORT;

#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_IP 0x0001
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_SRC_PORT 0x0002
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_IP 0x0004
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_ANY_DEST_PORT 0x0008
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_USE_FILTER 0x0010
#define EFI_PXE_BASE_CODE_UDP_OPFLAGS_MAY_FRAGMENT 0x0020
#define DEFAULT_TTL 16
#define DEFAULT_ToS 0

//*******************************************************
// Bootstrap Types
//*******************************************************
#define EFI_PXE_BASE_CODE_BOOT_TYPE_BOOTSTRAP 0
#define EFI_PXE_BASE_CODE_BOOT_TYPE_MS_WINNT_RIS 1
#define EFI_PXE_BASE_CODE_BOOT_TYPE_INTEL_LCM 2
#define EFI_PXE_BASE_CODE_BOOT_TYPE_DOSUNDI 3
#define EFI_PXE_BASE_CODE_BOOT_TYPE_NEC_ESMPRO 4
#define EFI_PXE_BASE_CODE_BOOT_TYPE_IBM_WSoD 5
#define EFI_PXE_BASE_CODE_BOOT_TYPE_IBM_LCCM 6
#define EFI_PXE_BASE_CODE_BOOT_TYPE_CA_UNICENTER_TNG 7
#define EFI_PXE_BASE_CODE_BOOT_TYPE_HP_OPENVIEW 8
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_9 9
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_10 10
#define EFI_PXE_BASE_CODE_BOOT_TYPE_ALTIRIS_11 11
#define EFI_PXE_BASE_CODE_BOOT_TYPE_NOT_USED_12 12
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REDHAT_INSTALL 13
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REDHAT_BOOT 14
#define EFI_PXE_BASE_CODE_BOOT_TYPE_REMBO 15
#define EFI_PXE_BASE_CODE_BOOT_TYPE_BEOBOOT 16
//
// Values 17 through 32767 are reserved.
// Values 32768 through 65279 are for vendor use.
// Values 65280 through 65534 are reserved.
//
#define EFI_PXE_BASE_CODE_BOOT_TYPE_PXETEST 65535
#define EFI_PXE_BASE_CODE_BOOT_LAYER_MASK 0x7FFF
#define EFI_PXE_BASE_CODE_BOOT_LAYER_INITIAL 0x0000

//*******************************************************
// EFI_PXE_BASE_CODE_SRVLIST
//*******************************************************
typedef struct {
    UINT16 Type;
    BOOLEAN AcceptAnyResponse;
    UINT8 reserved;
    EFI_IP_ADDRESS IpAddr;
} EFI_PXE_BASE_CODE_SRVLIST;

//*******************************************************
// EFI_PXE_BASE_CODE_DISCOVER_INFO
//*******************************************************
typedef struct {
    BOOLEAN UseMCast;
    BOOLEAN UseBCast;
    BOOLEAN UseUCast;
    BOOLEAN MustUseList;
    EFI_IP_ADDRESS ServerMCastIp;
    UINT16 IpCnt;
    EFI_PXE_BASE_CODE_SRVLIST SrvList[];
} EFI_PXE_BASE_CODE_DISCOVER_INFO;

//*******************************************************
// EFI_PXE_BASE_CODE_TFTP_OPCODE
//*******************************************************
typedef enum {
EFI_PXE_BASE_CODE_TFTP_FIRST,
EFI_PXE_BASE_CODE_TFTP_GET_FILE_SIZE,
EFI_PXE_BASE_CODE_TFTP_READ_FILE,
EFI_PXE_BASE_CODE_TFTP_WRITE_FILE,
EFI_PXE_BASE_CODE_TFTP_READ_DIRECTORY,
EFI_PXE_BASE_CODE_MTFTP_GET_FILE_SIZE,
EFI_PXE_BASE_CODE_MTFTP_READ_FILE,
EFI_PXE_BASE_CODE_MTFTP_READ_DIRECTORY,
EFI_PXE_BASE_CODE_MTFTP_LAST
} EFI_PXE_BASE_CODE_TFTP_OPCODE;

//*******************************************************
// EFI_PXE_BASE_CODE_MTFTP_INFO
//*******************************************************
typedef struct {
    EFI_IP_ADDRESS MCastIp;
    EFI_PXE_BASE_CODE_UDP_PORT CPort;
    EFI_PXE_BASE_CODE_UDP_PORT SPort;
    UINT16 ListenTimeout;
    UINT16 TransmitTimeout;
} EFI_PXE_BASE_CODE_MTFTP_INFO;

struct EFI_PXE_BASE_CODE_PROTOCOL;
typedef struct EFI_PXE_BASE_CODE_PROTOCOL EFI_PXE_BASE_CODE_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_START)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN BOOLEAN UseIpv6);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_STOP)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_DHCP)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN BOOLEAN SortOffers);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_DISCOVER)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN UINT16 Type,
    IN UINT16 *Layer,
    IN BOOLEAN UseBis,
    IN EFI_PXE_BASE_CODE_DISCOVER_INFO *Info OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_MTFTP)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN EFI_PXE_BASE_CODE_TFTP_OPCODE Operation,
    IN OUT VOID *BufferPtr, OPTIONAL
    IN BOOLEAN Overwrite,
    IN OUT UINT64 *BufferSize,
    IN UINTN *BlockSize, OPTIONAL
    IN EFI_IP_ADDRESS *ServerIp,
    IN CHAR8 *Filename, OPTIONAL
    IN EFI_PXE_BASE_CODE_MTFTP_INFO *Info, OPTIONAL
    IN BOOLEAN DontUseBuffer);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_UDP_WRITE)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN UINT16 OpFlags,
    IN EFI_IP_ADDRESS *DestIp,
    IN EFI_PXE_BASE_CODE_UDP_PORT *DestPort,
    IN EFI_IP_ADDRESS *GatewayIp, OPTIONAL
    IN EFI_IP_ADDRESS *SrcIp, OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT *SrcPort, OPTIONAL
    IN UINTN *HeaderSize, OPTIONAL
    IN VOID *HeaderPtr, OPTIONAL
    IN UINTN *BufferSize,
    IN VOID *BufferPtr);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_UDP_READ)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN UINT16 OpFlags,
    IN OUT EFI_IP_ADDRESS *DestIp, OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT *DestPort, OPTIONAL
    IN OUT EFI_IP_ADDRESS *SrcIp, OPTIONAL
    IN OUT EFI_PXE_BASE_CODE_UDP_PORT *SrcPort, OPTIONAL
    IN UINTN *HeaderSize, OPTIONAL
    IN VOID *HeaderPtr, OPTIONAL
    IN OUT UINTN *BufferSize,
    IN VOID *BufferPtr);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_SET_IP_FILTER)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN EFI_PXE_BASE_CODE_IP_FILTER *NewFilter);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_ARP)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN EFI_IP_ADDRESS *IpAddr,
    IN EFI_MAC_ADDRESS *MacAddr OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_SET_PARAMETERS)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN BOOLEAN *NewAutoArp, OPTIONAL
    IN BOOLEAN *NewSendGUID, OPTIONAL
    IN UINT8 *NewTTL, OPTIONAL
    IN UINT8 *NewToS, OPTIONAL
    IN BOOLEAN *NewMakeCallback OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_SET_STATION_IP)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN EFI_IP_ADDRESS *NewStationIp, OPTIONAL
    IN EFI_IP_ADDRESS *NewSubnetMask OPTIONAL);

typedef EFI_STATUS (EFIAPI *EFI_PXE_BASE_CODE_SET_PACKETS)(
    IN EFI_PXE_BASE_CODE_PROTOCOL *This,
    IN BOOLEAN *NewDhcpDiscoverValid, OPTIONAL
    IN BOOLEAN *NewDhcpAckReceived, OPTIONAL
    IN BOOLEAN *NewProxyOfferReceived, OPTIONAL
    IN BOOLEAN *NewPxeDiscoverValid, OPTIONAL
    IN BOOLEAN *NewPxeReplyReceived, OPTIONAL
    IN BOOLEAN *NewPxeBisReplyReceived, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewDhcpDiscover, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewDhcpAck, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewProxyOffer, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewPxeDiscover, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewPxeReply, OPTIONAL
    IN EFI_PXE_BASE_CODE_PACKET *NewPxeBisReply OPTIONAL);

typedef struct EFI_PXE_BASE_CODE_PROTOCOL {
    UINT64 Revision;
    EFI_PXE_BASE_CODE_START Start;
    EFI_PXE_BASE_CODE_STOP Stop;
    EFI_PXE_BASE_CODE_DHCP Dhcp;
    EFI_PXE_BASE_CODE_DISCOVER Discover;
    EFI_PXE_BASE_CODE_MTFTP Mtftp;
    EFI_PXE_BASE_CODE_UDP_WRITE UdpWrite;
    EFI_PXE_BASE_CODE_UDP_READ UdpRead;
    EFI_PXE_BASE_CODE_SET_IP_FILTER SetIpFilter;
    EFI_PXE_BASE_CODE_ARP Arp;
    EFI_PXE_BASE_CODE_SET_PARAMETERS SetParameters;
    EFI_PXE_BASE_CODE_SET_STATION_IP SetStationIp;
    EFI_PXE_BASE_CODE_SET_PACKETS SetPackets;
    EFI_PXE_BASE_CODE_MODE *Mode;
} EFI_PXE_BASE_CODE_PROTOCOL;

#endif /* INCLUDE_EFI_PROTOCOLS_PXE_BASE_CODE_H_ */
