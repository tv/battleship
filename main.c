
#include <cfg/crt.h>    /* Floating point configuration. */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/heap.h>
#include <sys/thread.h>
#include <sys/confnet.h>

#include <dev/board.h>
#include <sys/timer.h>
#include <sys/heap.h>

#include <netinet/ip.h>

#include <sys/socket.h>


#include <netinet/tcp.h>
#include <netinet/icmp.h>
#include <netinet/ip_icmp.h>

#include <arpa/inet.h>
#include <net/route.h>

#include <pro/snmp.h>
#include <pro/snmp_config.h>
#include <pro/snmp_mib.h>
#include <pro/snmp_api.h>
#include <pro/snmp_agent.h>

#define IP_NETMASK "255.255.255.0"

#include "snmp.h"


THREAD(udpTrapReceiveThread, arg)
{
	NutThreadSetPriority(32);
	UDPSOCKET* socket = NutUdpCreateSocket(TRAP_PORT);
	uint8_t data[256];
	uint32_t addr;
	uint16_t port;
	int i;

	for(;;)
	{
		i = NutUdpReceiveFrom(socket, &addr, &port, data, 256, 0);

		//printf("Some trap traffic...\n");
		data[i] = '\0';
		if (i > 0)
		{
			printf("a trap received...\n");
				// TODO: trap-parse
				printf("TRAP RECEIVED!\n"); //debug

				snmpPacket* packetIn = snmp_alloc(); //input
				snmp_parse(packetIn, data, i);

				printf("%d %s\n", packetIn->genericType, packetIn->value);


				snmpPacket* packetOut = snmp_alloc(); //output

				packetOut->command 		= SNMP_TRAP;
				packetOut->objectID		= packetIn->objectID;

				char * value = "I lost";

				packetOut->valueType	= SNMP_OCT;
				packetOut->valueLength	= 7;
				packetOut->value		= &value;
				packetOut->error 		= 0x00;

				snmp_trapConstruct(packetOut);
				NutUdpSendTo(socket, addr, port, packetOut->packet, packetOut->length);

				snmp_free(packetIn);
				snmp_free(packetOut);

				// TODO: remove current opponent and return to menu or something

		}else if( i== 0) printf("trap thread running\n");
		else printf("trap thread running\n");
	}
}

THREAD(udpReceiveThread, arg)
{
	UDPSOCKET* socket = NutUdpCreateSocket(SNMP_PORT);
	uint8_t data[256];
	uint32_t addr;
	uint16_t port;
	int i;
	for(;;)
	{
		i = NutUdpReceiveFrom(socket, &addr, &port, data, 256, 0);
		data[i] = '\0';
		if (i > 0)
		{
			printf("Udp connection\n");
			printf("%s:%d \n", inet_ntoa(addr), port);
			fflush(stdout);
			long value;
			snmpPacket* packetIn = snmp_alloc(); //input
			snmp_parse(packetIn, data, i);

			snmpPacket* packetOut = snmp_alloc(); //output

			packetOut->command 		= SNMP_GET_RESP;
			packetOut->requestID 	= packetIn->requestID;
			packetOut->objectID		= packetIn->objectID;

			packetOut->valueType	= SNMP_INT;
			packetOut->valueLength	= 1;
			packetOut->value		= &value;
			packetOut->error 		= 0x00;

			if(packetIn->valueType == SNMP_OCT)
				printf("%s\n", packetIn->value);

			snmp_construct(packetOut);
			NutUdpSendTo(socket, addr, port, packetOut->packet, packetOut->length);

			snmp_free(packetIn);
			snmp_free(packetOut);
		}
	}
}

int main(void)
{
	u_long baud = 115200;

	#ifdef NUT_DEBUG
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
	freopen(DEV_DEBUG_NAME, "r+", stdout);
	freopen(DEV_DEBUG_NAME, "r+", stdin);
	#else
	NutRegisterDevice(&DEV_UART, 0, 0);
	freopen(DEV_UART0_NAME, "r+", stdout);
	freopen(DEV_UART0_NAME, "r+", stdin);
	#endif

	_ioctl(_fileno(stdout), UART_SETSPEED, &baud);
	_ioctl(_fileno(stdin),  UART_SETSPEED, &baud);

	printf("Jee!\n");


	for(;;) {
		printf("Registering %s... ", DEV_ETHER_NAME);
		NutSleep(500);
		if(NutRegisterDevice(&DEV_ETHER, 0, 0) == 0) {
			printf("ok\n");
			break;
		}
		printf("Failed, retry\n");
	}

	int ip_end = rand()%128 +1;

	char ip[11];
	sprintf(ip, "10.10.7.%d", ip_end);

	//u_char mac[6] =   {0x00, 0x06, 0x98, 0x30, 0x00, ip_end};

	printf("Configuring %s... ", DEV_ETHER_NAME);
	NutNetIfConfig(DEV_ETHER_NAME, NULL, inet_addr("10.10.7.46"), inet_addr("255.255.255.0"));
	NutSleep(2000);
	//printf("ok\n");

	printf("Creating Udp socket... ");
	NutThreadCreate("udpReceiveThread", udpReceiveThread, NULL, 1024);
	NutThreadCreate("udpTrapReceiveThread", udpTrapReceiveThread, NULL, 756);
	printf("ok\n");

	for(;;) {
		printf("IP: %s\n", inet_ntoa(confnet.cdn_ip_addr));
		NutSleep(3000);
	}
}
