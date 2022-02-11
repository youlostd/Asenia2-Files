#include "StdAfx.h"
#include "PingManager.h"
#ifdef ENABLE_PINGTIME
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

extern const char *	global_ip;

int __cdecl CPingManager::PingTimeRev()  {
	static HANDLE hIcmpFile;
	static unsigned long ipaddr = INADDR_NONE;
	static DWORD dwRetVal = 0;
	static char SendData[32] = "Data Buffer";
	static LPVOID ReplyBuffer = NULL;
	static DWORD ReplySize = 0;

	ipaddr = inet_addr(global_ip);

	if (ipaddr == INADDR_NONE) {
		return 1;
	}

	hIcmpFile = IcmpCreateFile();

	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		return 1;
	}

	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);

	if (ReplyBuffer == NULL) {
		return 1;
	}

	dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
	PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;

	if (dwRetVal != 0) {
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		return pEchoReply->RoundTripTime;
	}

	else {
		return 0;
	}
}
#endif






