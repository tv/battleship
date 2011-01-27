#include "Network.h"

bool Network_init(NH * nh) {
	int i = 0;
	for(;;) {
		i++;
		printf("Registering %s... ", DEV_ETHER_NAME);
		NutSleep(500);
		if(NutRegisterDevice(&DEV_ETHER, 0, 0) == 0) {
			printf("ok\n");
			break;
		}
		printf("Failed, retry\n");
		if ( i > 2 )
			return false;
	}

	int ip_end = rand()%128+1;
	char ip[12];
	sprintf(ip, "10.10.7.%d", ip_end);

	uint8_t mac[6] =   {0x00, 0x06, 0x98, 0x30, 0x00, ip_end};
	printf("configuring... ");
	NutNetIfConfig(DEV_ETHER_NAME, mac, inet_addr(ip), inet_addr(NMASK));
	printf("IP: %s\n\n\n", inet_ntoa(inet_addr(ip)));
	NutSleep(2000);
	printf("ok\n");
	fflush(stdout);

	printf("Creating Udp socket... ");
	if ((nh->udpSock = NutUdpCreateSocket(SNMP_PORT)) == 0)
	{
		printf("failed\n");
	}
	printf("ok\n");

	printf("Creating Udp TRAP socket... ");
	if ((nh->udpTrapSock = NutUdpCreateSocket(TRAP_PORT)) == 0)
	{
		printf("failed\n");
	}
	printf("ok\n");

	//allocate room for packets and values
	SNMP_packetPool = (snmpPacket**)malloc(PACKETPOOL_SIZE * sizeof(snmpPacket*));
	for(i = 0; i < PACKETPOOL_SIZE; i++)
		SNMP_packetPool[i] = (snmpPacket*)malloc(sizeof(snmpPacket));
	SNMP_valuePool = (uint8_t*)malloc(VALUEPOOL_SIZE * sizeof(uint8_t));
	PP_ptr = 0; VP_ptr = 0; VP_reserved = 0; PP_reserved = 0;

	SNMP_valuePool[215] = 'G'; //DEBUG
	//printf("PP_ptr: %d, valuepool[0]: %d, valuepool[215]: %c\n", PP_ptr, SNMP_valuePool[0], SNMP_valuePool[215]); //DEBUG

	NutThreadCreate("udpReceiveThread", udpReceiveThread, nh, 1024);
	NutThreadCreate("udpTrapReceiveThread", udpTrapReceiveThread, nh, 756);
	printf("ok\n");
	return true;
}

THREAD(udpTrapReceiveThread, arg)
{
	//NutThreadSetPriority(32);
	NH * nh = (NH*)arg;

	uint8_t data[256];
	uint32_t addr;
	uint16_t port;
	int i;

	for(;;)
	{
		i = NutUdpReceiveFrom(nh->udpTrapSock, &addr, &port, data, 256, 0);

		//printf("Some trap traffic...\n");
		data[i] = '\0';
		if (i > 0)
		{
			//printf("a trap received...\n");
			if(nh->opponent == addr)
			{
				snmpPacket* packetIn = snmp_packetalloc(); //input
				snmp_parse(packetIn, data, i);

				uint8_t* ptr = (uint8_t*)packetIn->value;
				strcpy(nh->trapMessage, ptr);
				// printf("Viesti: \"%s\"\n", ptr);
				nh->opponent = 0;
				snmp_free(packetIn);
			}
		}else if( i== 0) printf("trap thread running\n");
		else printf("trap thread running\n");
	}
}

THREAD(udpReceiveThread, arg)
{
	NH * nh = (NH*)arg;
	uint8_t data[256];
	uint32_t addr;
	uint16_t port;
	int i;
	for(;;)
	{
		i = NutUdpReceiveFrom(nh->udpSock, &addr, &port, data, 256, 0);
		data[i] = '\0';
		if (i > 0)
		{
			//printf("Udp connection\n");
			//printf("%s:%d \n", inet_ntoa(addr), port);
			//fflush(stdout);

			//bool doCleanPacketIn = true;

			snmpPacket* packetIn = snmp_packetalloc();
			snmpPacket* packetOut = snmp_packetalloc();
			snmp_parse(packetIn, data, i);

			packetOut->command 		= SNMP_GET_RESP;
			packetOut->requestID 	= packetIn->requestID;
			packetOut->objectID		= packetIn->objectID;

			long value;

			// --- vastataan ready to play -viestiin
			if(packetIn->objectID == 1 && packetIn->command == SNMP_GET_REQ) //READY-TO-PLAY -message
			{
				if(nh->readyToPlay == true)
					value = 1;
				else value = 0;
				packetOut->valueType	= SNMP_INT;
				packetOut->valueLength	= 1;
				packetOut->value		= &value;
				packetOut->error 		= 0x00;

				snmp_construct(packetOut);
				NutUdpSendTo(nh->udpSock, addr, port, packetOut->packet, packetOut->length);
			}
			// --- odotetaan vastausta ready to play viestiin
			else if(nh->getOpponentList && packetIn->objectID == 1 && packetIn->command == SNMP_GET_RESP)
			{
				uint8_t tmp;
				tmp = *((uint8_t*)(packetIn->value));
				if(tmp == 1) //fill the opponent list, if broadcast is sent
				{
					nh->opponentList[nh->numOpponents] = addr;
					nh->numOpponents++;
				}
			}
			// --- haaste vastaanotettu
			else if (nh->opponent == 0 && packetIn->objectID == 2 && packetIn->command == SNMP_SET_REQ)
			{
				nh->openRequest = true;
				nh->opponent  = addr;
				nh->requestID = packetIn->requestID;
			}
			//haaste vastaanotettu, mutta meillä oli jo haaste päällä
			else if ( (nh->openRequest || addr != nh->opponent) && packetIn->objectID == 2 && packetIn->command == SNMP_SET_REQ)
				NH_sendSnmpError(nh, addr, SNMP_PORT, SNMP_GET_RESP, 2, packetOut->requestID);
			// --- odotetaan vastausta haasteeseen
			else if( packetIn->objectID == 2 && nh->challengeSend == true && nh->opponent == addr &&
					 packetIn->command == SNMP_GET_RESP )
			{
				nh->challengeSend = false;
				if( packetIn->error != 0x00)
					nh->opponent = false;
				else
					nh->challengeAccepted = true;
			}
			// --- odotetaan vastustajan START-TURN
			else if(packetIn->objectID == 2 && nh->opponent == addr &&
					packetIn->command == SNMP_SET_REQ )
			{
				nh->enemyStartedTurn = true;
				//printf("NW: enemy started turn\n");
			}

			// --- odotetaan vastustajalta OK-viestiä
			else if(nh->opponent == addr && packetIn->command == SNMP_GET_RESP
					&& packetIn->objectID == nh->currentOID && packetIn->valueType == SNMP_INT)
			{
				if(packetIn->error == 0x00)
				{
					nh->enemyOK = true;
					//printf("NW: Received enemyOK!\n");
				}
			}

			// --- odotetaan salvoja
			else if( packetIn->objectID == 3 && nh->opponent == addr && packetIn->command == SNMP_SET_REQ )
			{
				nh->enemySalvoReceived = true;
				//printf("NW: SALVO received (%d):\n", nh->enemySalvoReceived); // debug
				uint8_t* ptr = (uint8_t*)packetIn->value;
				for(i = 0; i < 5; i++)
				{
					nh->lastEnemySalvo[i].x = ptr[i*2];
					nh->lastEnemySalvo[i].y = ptr[i*2+1];
					//printf("%d - %d\n", nh->lastEnemySalvo[i].x, nh->lastEnemySalvo[i].y);
				}
				// OK vastaus
				NH_sendBooleanResponse(nh, 3, true);
			}
			// --- Odotetaan salvon  tuloksia
			else if( packetIn->objectID == 4 && nh->opponent == addr && packetIn->command == SNMP_GET_RESP)
			{
				//printf("NW: SALVO results received: \n"); // debug
				uint8_t* ptr = (uint8_t*)packetIn->value;
				for (i = 0; i < 5; i++)
					nh->salvoResults[i] = ptr[i];

				//debug
				//for (i = 0; i < 5; i++)
					//printf("%d:  %d\n", i, nh->salvoResults[i]);

				nh->salvoResultsReceived = true;
			}
			// --- odotetaan salvon tuloksien pyyntöä
			else if(packetIn->objectID == 4 && packetIn->command == SNMP_GET_REQ && nh->opponent == addr)
			{
				nh->resultsQueryReceived = true;
				//printf("NW: enemy asked for salvo results: %d\n", nh->resultsQueryReceived);
			}
			// --- chattiviesti:
			else if(packetIn->objectID == 5 && packetIn->command == SNMP_SET_REQ && packetIn->valueType == SNMP_OCT)
			{
				/*
				uint8_t* ptr = (uint8_t*)packetIn->value;

				//lisätään viestin alkuun lähettähän ip
				sprintf(nh->chatMsgs[nh->emptyMsg], inet_ntoa(addr));
				strcat(nh->chatMsgs[nh->emptyMsg], ":\n");
				strncat(nh->chatMsgs[nh->emptyMsg], message, MAX_MSG_LENGTH - 16);

				//lisätään viesti listaan
				sprintf(chatMsgs[nh->emptyMsg], ptr);
				nh->manageMessages();
				*/
			}
			else if(packetIn->command == SNMP_GET_REQ) //debug
			{
				printf("NW: weird SNMP_GET_REQ received, OID: %d, opponent: %s", packetIn->objectID, inet_ntoa(nh->opponent));
			}
			snmp_free(packetIn);
			snmp_free(packetOut);
		}
		else if (i == 0)
		{
			printf ("Udp timeout\n");
		}
		else
		{
			printf ("Udp error\n");
		}

	}
}
