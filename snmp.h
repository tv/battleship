/**
 * \file snmp.h
 * SNMP-packet struct, constructor and parser. The low-level network tool.
 */
#ifndef SNMP_H
#define SNMP_H

#include <cfg/crt.h>    /* Floating point configuration. */

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/heap.h>
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
// #include <pro/snmp_config.h>
// #include <pro/snmp_mib.h>
// #include <pro/snmp_api.h>
// #include <pro/snmp_agent.h>

#define SNMP_PORT		161
#define TRAP_PORT		162

#define SNMP_GET_REQ	0xA0
#define SNMP_GET_RESP	0xA2
#define SNMP_SET_REQ	0xA3
#define SNMP_TRAP		0xA4

#define SNMP_OID		0x06
#define SNMP_SEQ		0x30
#define SNMP_INT		0x02
#define SNMP_OCT		0x04
#define SNMP_NULL		0x05
#define SNMP_ADDR		0x40
#define SNMP_TIME		0x43

#define PACKETPOOL_SIZE 10
#define VALUEPOOL_SIZE	4096
#define PACKET_SIZE 255

typedef struct {
	// internal variables
	uint8_t* 	packet;
	size_t		availableBytes;
	uint8_t 	length;
	uint8_t* 	pCursor;

	// internal lengths
	uint8_t* 	rSnmpMsgLen;
	uint8_t* 	rSnmpPduLen;
	uint8_t* 	rVarbindListLen;
	uint8_t* 	rVarbindLen;

	// command specific options
	uint8_t		command;
	long 		requestID;
	uint8_t		objectID;
	uint8_t 	valueType;
	void *		value;
	size_t		valueLength;

	uint8_t		error;

	// trap specific options
	uint8_t		genericType;
	uint8_t		specificType;

} snmpPacket;

//memory pools
snmpPacket ** SNMP_packetPool;
uint8_t * SNMP_valuePool;
volatile uint8_t PP_ptr;
volatile uint8_t PP_reserved;
volatile uint8_t VP_ptr;
volatile uint8_t VP_reserved;
//MUTEX * PP_mutex;
//MUTEX * VP_mutex;

snmpPacket * snmp_packetalloc(void);

uint8_t * snmp_valuealloc(int size);

void snmp_free(snmpPacket * sp);

void snmp_parse(snmpPacket * sp, uint8_t * data, int length);

void snmp_construct (snmpPacket * sp);

void snmp_trapConstruct (snmpPacket * sp);

#endif
