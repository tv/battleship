#include "NetHandler.h"

NH * NH_construct(void) //init all needed values here
{
	NH * nh = (NH*)malloc(sizeof(NH));
	nh->opponentList = (uint32_t*)malloc(MAX_OPPONENTS* sizeof(uint32_t));
	nh->lastEnemySalvo = (Grids*)malloc(sizeof(Grids)*5);
	nh->salvoResults = (uint8_t*)malloc(sizeof(uint8_t)*5);
	nh->enemyShotResults = (uint8_t*)malloc(sizeof(uint8_t)*5);
	//nh->chatMsgs = (uint8_t**)malloc(sizeof(uint8_t*) * MAX_MESSAGES);
	//for(i = 0; i < MAX_MESSAGES; i++)
	//	nh->chatMsgs[i] = (uint8_t*)malloc(sizeof(uint8_t) * MAX_MSG_LENGTH);

	//nh->lastPacketFromOpponent = NULL;
	nh->trapMessage = malloc(128);
	NH_init(nh);
	return nh;
}

void NH_destruct(NH * nh)
{
	free(nh->opponentList);
	//free(nh->lastPacketFromOpponent);
	free(nh->lastEnemySalvo);
	free(nh->salvoResults);
	//for(i = 0; i < MAX_MESSAGES; i++) free(nh->chatMsgs[i]);
	//free(nh->chatMsgs);
	free(nh);
}


void NH_init(NH * nh)
{
	nh->numOpponents = 0;
	nh->currentOID = 0;
	nh->opponent = 0;
	nh->numMsgs = 0;
	nh->emptyMsg = 0;
	nh->readyToPlay = false;
	nh->openRequest = false;
	nh->getOpponentList = false;
	nh->challengeAccepted = false;
	nh->enemyStartedTurn = false;
	nh->enemyOK = false;
	nh->enemySalvoReceived = false;
	nh->salvoResultsReceived = false;
	nh->resultsQueryReceived = false;
}
long NH_sendSnmpPacket(NH *nh, uint32_t addr, uint8_t port, uint8_t command, uint8_t valuetype, void *value,  uint8_t oid, long reqid)
{
	snmpPacket* packet = snmp_packetalloc();

	packet->error = 0;

	packet->command = command;
	packet->objectID = oid;

	if(reqid == 0) {
		reqid = rand()+1;
	}

	packet->requestID = reqid;

	packet->valueType = valuetype;
	packet->value = value;

	snmp_construct(packet);

	NutUdpSendTo(nh->udpSock, addr, port, packet->packet, packet->length);

	snmp_free(packet);

	return reqid;
}

long NH_sendSnmpPacketOCT(NH *nh, uint32_t addr, uint8_t port, uint8_t command, uint8_t valuetype, void *value, size_t valueLength, uint8_t oid, long reqid)
{
	snmpPacket* packet = snmp_packetalloc();

	packet->error = 0;

	packet->command = command;
	packet->objectID = oid;

	if(reqid == 0) {
		reqid = rand()+1;
	}

	packet->requestID = reqid;

	packet->valueType = valuetype;
	packet->value = value;
	packet->valueLength = valueLength;

	snmp_construct(packet);

	NutUdpSendTo(nh->udpSock, addr, port, packet->packet, packet->length);

	snmp_free(packet);

	return reqid;
}

void NH_sendTrap(NH *nh, char * msg) //TODO: this is just a sceleton
{
	snmpPacket* packet = snmp_packetalloc();

	packet->genericType = 3;
	packet->specificType = 0;
	packet->command = SNMP_TRAP;

	packet->objectID = 6;

	packet->valueType = SNMP_OCT;
	packet->value = (void*)msg;
	packet->valueLength = strlen(msg);
	snmp_trapConstruct(packet);


	NutUdpSendTo(nh->udpTrapSock, nh->opponent, TRAP_PORT, packet->packet, packet->length);
}

long NH_sendSnmpError(NH *nh, uint32_t addr, uint8_t port, int command, int oid, int reqid)
{
	snmpPacket* packet = snmp_packetalloc();

	packet->error = 0x05;
	packet->command = command;
	packet->objectID = oid;

	packet->requestID = reqid;
	packet->valueType = SNMP_NULL;

	snmp_construct(packet);

	NutUdpSendTo(nh->udpSock, addr, port, packet->packet, packet->length);

	snmp_free(packet);

	return reqid;
}

void NH_sendChallenge(NH * nh, int opponentID)
{
	nh->opponent = nh->opponentList[opponentID]; //set our opponent
	uint32_t value = 0;
	NH_sendSnmpPacket(nh, nh->opponent, SNMP_PORT, SNMP_SET_REQ, SNMP_INT, &value, 2, 0);
	nh->challengeSend = true;
}


int NH_getOpponentList(NH * nh)
{
	//phase 1:
	//	broadcast a GET-REQUEST message and gather the responds & build the opponent list
	NH_sendSnmpPacket(nh, inet_addr(BCAST), SNMP_PORT, SNMP_GET_REQ, SNMP_NULL, NULL, 1, 0);
	nh->getOpponentList = true;
	nh->numOpponents = 0;
	NutSleep(1000); //wait for response
	nh->getOpponentList = false;
	return nh->numOpponents;
}

bool NH_getShots(NH * nh, Grids * enemyShots)
{
	return true;
}

/*
bool NH_sendShotResults(NH * nh, uint8_t * eShotResults){
	uint8_t sequence[5];
	int i;
	for ( i = 0; i < 5; i++)
	{
		sequence
	}
	NH_sendSnmpPacketOCT(nh, nh->opponent, SNMP_PORT, SNMP_)
	return true;
}
*/

bool NH_getShotResults(NH * nh, uint8_t * myShotResults){
	return true;
}

void NH_sendShots(NH * nh, Grids * myShots, int iShots)
{
	char sequence[10];
	int i;
	for(i = 0; i < iShots; i++)
	{
		sequence[2*i] = myShots[i].x;
		sequence[2*i+1]= myShots[i].y;
	}
	NH_sendSnmpPacketOCT(nh, nh->opponent, SNMP_PORT, SNMP_SET_REQ, SNMP_OCT, sequence, 2*iShots, 3, 0);
}

void NH_sendStartTurn(NH * nh, int turn)
{
	uint32_t value = turn;
	NH_sendSnmpPacket(nh, nh->opponent, SNMP_PORT, SNMP_SET_REQ, SNMP_INT, &value, 2, 0);
}


void NH_sendBooleanResponse(NH * nh, int oid, bool userResponse)
{
	int value = 1;
	if(userResponse != 0)
		NH_sendSnmpPacket(nh, nh->opponent, SNMP_PORT, SNMP_GET_RESP, SNMP_INT, &value, oid, nh->requestID);
	else
		NH_sendSnmpError (nh, nh->opponent, SNMP_PORT, SNMP_GET_RESP, oid, nh->requestID);
}

void NH_manageMessages(NH * nh)
{
	/*
	int i, k;
	nh->emptyMsg++;
	if(nh->emptyMsg == MAX_MESSAGES) nh->emptyMsg = 0;
	if(nh->numMsgs < MAX_MESSAGES)	//the list is not full of messages
		nh->numMsgs++;
	else						//the list is full of messages, so rotate
	{
		k = nh->emptyMsg;
		uint8_t* tmp = nh->chatMsgs[0];
		for(i = 0; i < MAX_MESSAGES; i++)
		{
			if(k == MAX_MESSAGES -1)
			{
				nh->chatMsgs[k] = tmp;
				k = 0;
			}
			else
			{
				nh->chatMsgs[k] = nh->chatMsgs[k+1];
				k++;
			}
		}
	}
	*/
}
