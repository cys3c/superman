#ifndef _SUPERMAN_PACKET_H
#define _SUPERMAN_PACKET_H

#include <linux/ip.h>
#include <linux/skbuff.h>
#include <asm/byteorder.h>

#include "superman.h"

enum {
	SUPERMAN_DISCOVERY_REQUEST_TYPE = 1,
#define SUPERMAN_DISCOVERY_REQUEST_TYPE SUPERMAN_DISCOVERY_REQUEST_TYPE

	SUPERMAN_CERTIFICATE_REQUEST_TYPE = 2,
#define SUPERMAN_CERTIFICATE_REQUEST_TYPE SUPERMAN_CERTIFICATE_REQUEST_TYPE

	SUPERMAN_CERTIFICATE_EXCHANGE_TYPE = 3,
#define SUPERMAN_CERTIFICATE_EXCHANGE_TYPE SUPERMAN_CERTIFICATE_EXCHANGE_TYPE

	SUPERMAN_AUTHENTICATED_SK_REQUEST_TYPE = 4,
#define SUPERMAN_AUTHENTICATED_SK_REQUEST_TYPE SUPERMAN_AUTHENTICATED_SK_REQUEST_TYPE

	SUPERMAN_AUTHENTICATED_SK_RESPONSE_TYPE = 5,
#define SUPERMAN_AUTHENTICATED_SK_RESPONSE_TYPE SUPERMAN_AUTHENTICATED_SK_RESPONSE_TYPE

	SUPERMAN_SK_INVALIDATE_TYPE = 6,
#define SUPERMAN_SK_INVALIDATE_TYPE SUPERMAN_SK_INVALIDATE_TYPE

	SUPERMAN_BROADCAST_KEY_EXCHANGE_TYPE = 7,
#define SUPERMAN_BROADCAST_KEY_EXCHANGE_TYPE SUPERMAN_BROADCAST_KEY_EXCHANGE_TYPE

	__SUPERMAN_MAX_TYPE,
#define SUPERMAN_MAX_TYPE (__SUPERMAN_MAX_TYPE - 1)
};

// We need 1 byte alignment, otherwise the header size ends up being 6 bytes big, rather than 5.
#pragma pack(push)
#pragma pack(1)
struct superman_header {
	__u8	type;
	__be16	timestamp;
	__be16	payload_len;
};
#pragma pack(pop)

struct superman_footer {

};

void SendDiscoveryRequest(void);
void SendCertificateRequest(struct sk_buff* rx_sk);
void SendCertificateResponse(struct sk_buff* rx_sk);
bool ReceiveCertificateExchange(struct sk_buff* rx_sk);
bool HaveSK(struct sk_buff* rx_sk);
void SendAuthenticatedSKRequest(struct sk_buff* sk);
void SendAuthenticatedSKResponse(struct sk_buff* rx_sk);
bool ReceiveAuthenticatedSKResponse(struct sk_buff* rx_sk);
void InvalidateSK(struct sk_buff* rx_sk);
bool SendP2PPacket(struct sk_buff* tx_sk);
bool SendE2EPacket(struct sk_buff* tx_sk);
bool ReceiveP2PPacket(struct sk_buff* rx_sk);
bool ReceiveE2EPacket(struct sk_buff* rx_sk);

//struct sk_buff* EncapsulateSupermanPacket(struct sk_buff *skb, u_int8_t type, u_int16_t timestamp);
//struct sk_buff* DecapsulateSupermanPacket(struct sk_buff *skb);

#endif
