#pragma once

// Use this to pick apart the network stream, must be packed
#pragma pack(1)
typedef struct
{
	int		netID;
	int		sequenceNumber;
	int		packetID : 16;
	int		nSplitSize : 16;
} SPLITPACKET;
#pragma pack()



// Pad a number so it lies on an N byte boundary.
// So PAD_NUMBER(0,4) is 0 and PAD_NUMBER(1,4) is 4
#define PAD_NUMBER(number, boundary) \
	( ((number) + ((boundary)-1)) / (boundary) ) * (boundary)


// This is the packet payload without any header bytes(which are attached for actual sending)
#define	NET_MAX_PAYLOAD			( 262144 - 4)	// largest message we can send in bytes
#define NET_MAX_PAYLOAD_BITS	18		// 2^NET_MAX_PAYLOAD_BITS > NET_MAX_PAYLOAD
// This is just the client_t->netchan.datagram buffer size (shouldn't ever need to be huge)
#define NET_MAX_DATAGRAM_PAYLOAD 4000	// = maximum unreliable playload size

// UDP has 28 byte headers
#define UDP_HEADER_SIZE				(20+8)	// IP = 20, UDP = 8

#define MAX_ROUTABLE_PAYLOAD		1200	// x360 requires <= 1260, but now that listen servers can support "steam" mediated sockets, steam enforces 1200 byte limit

#if (MAX_ROUTABLE_PAYLOAD & 3) != 0
#error Bit buffers must be a multiple of 4 bytes
#endif

#define MIN_ROUTABLE_PAYLOAD		16		// minimum playload size

#define NETMSG_TYPE_BITS	8	// must be 2^NETMSG_TYPE_BITS > SVC_LASTMSG

// This is the payload plus any header info (excluding UDP header)

#define HEADER_BYTES	9	// 2*4 bytes seqnr, 1 byte flags

// Pad this to next higher 16 byte boundary
// This is the largest packet that can come in/out over the wire, before processing the header
//  bytes will be stripped by the networking channel layer
#define	NET_MAX_MESSAGE	PAD_NUMBER( ( NET_MAX_PAYLOAD + HEADER_BYTES ), 16 )

#define NET_HEADER_FLAG_SPLITPACKET				-2
#define NET_HEADER_FLAG_COMPRESSEDPACKET		-3



#define MIN_USER_MAXROUTABLE_SIZE	576  // ( X.25 Networks )
#define MAX_USER_MAXROUTABLE_SIZE	MAX_ROUTABLE_PAYLOAD

#define MAX_SPLIT_SIZE	(MAX_USER_MAXROUTABLE_SIZE - sizeof( SPLITPACKET ))
#define MIN_SPLIT_SIZE	(MIN_USER_MAXROUTABLE_SIZE - sizeof( SPLITPACKET ))



// Calculate MAX_SPLITPACKET_SPLITS according to the smallest split size
#define MAX_SPLITPACKET_SPLITS ( NET_MAX_MESSAGE / MIN_SPLIT_SIZE )



#define CONNECTIONLESS_HEADER			0xFFFFFFFF	// all OOB packet start with this sequence



#define C2S_CONNECT				'k'	// client requests to connect



#define FRAGMENT_BITS		8
#define FRAGMENT_SIZE		(1<<FRAGMENT_BITS)
#define MAX_FILE_SIZE_BITS	26
#define MAX_FILE_SIZE		((1<<MAX_FILE_SIZE_BITS)-1)	// maximum transferable size is	64MB