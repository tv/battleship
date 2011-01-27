#include "snmp.h"

snmpPacket * snmp_packetalloc(void) {
	//reserve memory from the pre-allocated pool
	snmpPacket * packet = SNMP_packetPool[PP_ptr];
	PP_ptr++;
	PP_reserved++;
	if(PP_ptr == PACKETPOOL_SIZE) PP_ptr = 0;
	if(PP_reserved > PACKETPOOL_SIZE) printf("ERROR IN SNMP_PACKETALLOC! PACKETPOOL OVERFLOW!\n");
	packet->availableBytes = 0;
	packet->valueLength = 0;
	return packet;
}

uint8_t * snmp_valuealloc(int size)
{
	//reserve memory from the pre-allocated pool
	uint8_t * value = &SNMP_valuePool[VP_ptr];
	VP_ptr += size;
	VP_reserved += size;
	if(VP_ptr >= VALUEPOOL_SIZE) VP_ptr -= VALUEPOOL_SIZE;
	if(VP_reserved > VALUEPOOL_SIZE) printf("ERROR IN SNMP_VALUEALLOC! VALUEPOOL OVERFLOW!\n");
	//TODO: pit‰s olla varmaan joku eventti, joka laukee, kun tilaa tulee lis‰‰... ettei ohjelma kaadu t‰h‰n virheeseen
	return value;
}

void snmp_free(snmpPacket * sp)
{
	//printf("snmp_free\n");
	//fflush(stdout); //DEBUG

	//free a packet
	PP_ptr--; PP_reserved--;
	if(PP_ptr < 0) PP_ptr += PACKETPOOL_SIZE;
	//free all data
	VP_ptr -= (sp->valueLength + sp->availableBytes); VP_reserved -= (sp->valueLength + sp->availableBytes);
	if(VP_ptr < 0) VP_ptr += VALUEPOOL_SIZE;

	//printf("snmp_free_end\n");
	//fflush(stdout); //DEBUG
}

int checkOID(uint8_t * data, int off)
{
	//printf("checkOID\n");
	//fflush(stdout); //DEBUG
	return (data[off]   == 0x06 && data[off+1] == 0x07 && data[off+2] == 0x2b &&
			data[off+3] == 0x06 && data[off+4] == 0x01 && data[off+5] == 0x03 &&
			data[off+6] == 0x37 && data[off+7] == 0x00 );
}

void parseValues(snmpPacket * sp, uint8_t* data, int off)
{
	//printf("parseValues\n");
	//fflush(stdout); //DEBUG
	sp->valueType = data[off];

	if( data[off] == SNMP_NULL ) {
		// Null Type
		sp->valueLength = 0;

	} else if ( data[off] == SNMP_INT) {
	// Int Type
		sp->valueLength = data[off+1];
		//sp->value = malloc( sizeof(uint8_t)*data[off+1] );
		sp->value = snmp_valuealloc(sp->valueLength);

		uint8_t* temp = sp->value;

		int j;
		for (j = 0; j<data[off+1]; j++) {
			temp[j] = (uint8_t)data[off+2+j];
		}

	} else if ( data[off] == SNMP_OCT) {
		// Octet type
		sp->valueLength = data[off+1];

		//sp->value = malloc( sizeof(uint8_t)* data[off+1]);
		sp->value = snmp_valuealloc(sp->valueLength);

		memcpy((void*)sp->value, &data[off+2], data[off+1]);

		uint8_t * handle = sp->value;
		//handle = handle +off+1;
		handle[data[off+1]] = '\0';
	} else {
		printf("Strange value\n");
	}
	//printf("parseValues_end\n");
	//fflush(stdout); //DEBUG
}

void snmp_parse(snmpPacket * sp, uint8_t * data, int length)
{
	//printf("snmp_parse\n");
	//fflush(stdout); //DEBUG
	if( data[0] == SNMP_SEQ ) {
		sp->length = data[1];
		sp->availableBytes = data[1];

		if( data[2] == SNMP_INT && data[3] == 0x01 && data[4] == 0x00 ) {

			if( data[7]  == 0x42 && data[8]  == 0x61 && data[9]  == 0x74 &&
				data[10] == 0x74 && data[11] == 0x6c && data[12] == 0x65 &&
				data[13] == 0x73 && data[14] == 0x68 && data[15] == 0x69 && data[16] == 0x70 ) {
					// PDU start

				sp->command = data[17];

				if(sp->command == SNMP_TRAP) // TRAP parse
				{
					if( checkOID(data, 19) )
					{
						sp->objectID = data[27];

						if( data[28] == SNMP_ADDR)
						{
							if ( data[34] == 0x02 && data[35] == 0x01)
							{
								sp->genericType = data[36];

								if ( data[37] == 0x02 && data[38] == 0x01)
								{
									sp->specificType = data[39];

									if ( data[40] == SNMP_TIME )
									{
										int i = data[41];

										if ( data[42+i] == SNMP_SEQ && data[43+1] != 0x00)
										{
											if ( data[44+i] == SNMP_SEQ && checkOID(data, 46+i) )
											{
												sp->objectID = data[54+i];

												parseValues(sp, data, 55+i);
											}
										}
										else
										{
											sp->valueLength = 0;
											sp->valueType = SNMP_NULL;
										}
									}
								}
								else
								{
									printf("Wrong specificType\n");
								}
							}
							else
							{
								printf("Wrong GenericType\n");
							}
						}
						else
						{
							printf("Wrong address\n");
						}
					}
					else
					{
						printf("Wrong OID\n");
					}
				}
				else 	// SNMP parse
				{
					if( data[19] == SNMP_INT ) {
					// REQID int
						uint8_t i = data[20];

						//char* temp_req = malloc(sizeof(char)*i); //MIKS KUMMASSA MALLOCOIDAAN??
						char temp_req[i];

						sprintf(temp_req, "%x", (uint8_t)data[21]);

						int j;
						for(j=1; j<i; j++) {
							sprintf(temp_req, "%s%x", temp_req, (uint8_t)data[21+j]);
						}
						sp->requestID = strtoul(temp_req, NULL, 16);
						//free(temp_req);

					//uint8_t type = ASN_INTEGER;
					//AsnUnsignedParse(&data[21], &sp->availableBytes, &type, &sp->requestID);

						if( data[21+i] == 0x02 && data[22+i] == 0x01 &&
						data[24+i] == 0x02 && data[25+i] == 0x01 && data[26+i] == 0x00 ) {

							sp->error = data[23+i];

							if( data[27+i] == SNMP_SEQ && data[29+i] == SNMP_SEQ ) {

								if( checkOID(data, 31+i) ) {

									sp->objectID = data[39+i];

									parseValues(sp, data, 40+i);

									/*sp->valueType = data[40+i];

									if( data[40+i] == SNMP_NULL ) {
										// Null Type
										sp->valueLength = 0;

									} else if ( data[40+i] == SNMP_INT) {
									// Int Type
										sp->valueLength = data[41+i];
										sp->value = malloc( sizeof(uint8_t)*data[41+i] );

										uint8_t* temp = sp->value;

										int j;
										for (j = 0; j<data[41+i]; j++) {
											temp[j] = (uint8_t)data[42+i+j];
										}

									} else if ( data[40+i] == SNMP_OCT) {
										// Octet type
										sp->valueLength = data[41+i];

										sp->value = malloc( sizeof(uint8_t)* data[41+i]);
										memcpy((void*)sp->value, &data[42+i], data[41+i]);

									} else {
										printf("Strange value\n");
									}*/

								} else {
									printf("Wrong OID\n");
								}
							} else {
								printf("Wrong varbindlist or varbind\n");
							}

						} else {
							printf("Some weird error message or something!\n");
						}

					} else {
						printf("Wrong requestID!\n");
					}
				}
			} else {
				printf("Wrong communitystring!\n");
			}

		} else {
			printf("Wrong snmp-version!\n");
		}

	} else {
		printf("Malformed snmp-packet!\n");
	}
	//printf("snmp_parse_end\n");
	//fflush(stdout); //DEBUG
}

void snmp_construct (snmpPacket * sp)
{
	//printf("snmp_construct\n");
	//fflush(stdout); //DEBUG

	//allocate 255 bytes of memory
	//sp->packet = malloc( 255*sizeof(uint8_t) );
	sp->packet = snmp_valuealloc(PACKET_SIZE);

	sp->availableBytes = PACKET_SIZE;
	sp->pCursor = sp->packet;
	sp->rSnmpMsgLen = &sp->packet[1];

	//SNMP MESSAGE

	*sp->pCursor = 0x30;
	sp->pCursor++;
	*sp->pCursor = 0x00;
	sp->pCursor++;

	//SNMPv1
	*sp->pCursor = 0x02;
	sp->pCursor++;
	*sp->pCursor = 0x01;
	sp->pCursor++;
	*sp->pCursor = 0x00;
	sp->pCursor++;

	//SNMP Community String

	*sp->pCursor = 0x04;
	sp->pCursor++;
	*sp->pCursor = 0x0A;
	sp->pCursor++;


	// B
	*sp->pCursor = 0x42;
	sp->pCursor++;

	// a
	*sp->pCursor = 0x61;
	sp->pCursor++;

	// t
	*sp->pCursor = 0x74;
	sp->pCursor++;

	// t
	*sp->pCursor = 0x74;
	sp->pCursor++;

	// l
	*sp->pCursor = 0x6c;
	sp->pCursor++;

	// e
	*sp->pCursor = 0x65;
	sp->pCursor++;

	// s
	*sp->pCursor = 0x73;
	sp->pCursor++;

	// h
	*sp->pCursor = 0x68;
	sp->pCursor++;

	// i
	*sp->pCursor = 0x69;
	sp->pCursor++;

	// p
	*sp->pCursor = 0x70;
	sp->pCursor++;

	//SNMP PDU
	*sp->pCursor = sp->command;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rSnmpPduLen = sp->pCursor;
	sp->pCursor++;

	// sp->pCursor = AsnHeaderBuild(sp->pCursor, &sp->availableBytes, , 0);

	//Request ID
	//printf("RequestID: %ld\n", sp->requestID);
	sp->pCursor = AsnUnsignedBuild(sp->pCursor, &sp->availableBytes, SNMP_INT, &sp->requestID);

	//Error
	*sp->pCursor = 0x02;
	sp->pCursor++;
	*sp->pCursor = 0x01;
	sp->pCursor++;
	*sp->pCursor = sp->error;

	//printf("%d\n", sp->error);

	sp->pCursor++;

	//Error index
	*sp->pCursor = 0x02;
	sp->pCursor++;
	*sp->pCursor = 0x01;
	sp->pCursor++;
	*sp->pCursor = 0x00;
	sp->pCursor++;

	//Varbind list
	*sp->pCursor = 0x30;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rVarbindListLen = sp->pCursor;
	sp->pCursor++;

	//Varbind
	*sp->pCursor = 0x30;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rVarbindLen = sp->pCursor;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x07;
	sp->pCursor++;

	*sp->pCursor = 0x2b;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x03;
	sp->pCursor++;

	*sp->pCursor = 0x37;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->pCursor++;

	*sp->pCursor = sp->objectID;
	sp->pCursor++;


	if( sp->valueType == SNMP_INT ) {
		sp->pCursor = AsnIntegerBuild(sp->pCursor, &sp->availableBytes, SNMP_INT, (long*)sp->value);

		// *sp->pCursor = SNMP_INT;
		// sp->pCursor++;
		//
		// *sp->pCursor = sp->valueLength;
		// sp->pCursor++;
		//
		// int i;
		// for(i=0; i< sp->valueLength; i++) {
		// 	*sp->pCursor = sp->value[i];
		// 	sp->pCursor++;
		// }
		// sp->pCursor--;


	} else if ( sp->valueType == SNMP_OCT ) {
		sp->pCursor = AsnOctetStringBuild(sp->pCursor, &sp->availableBytes, SNMP_OCT, sp->value, sp->valueLength);

		/*
		int* pTmp = sp->value;
		int i;
		*sp->pCursor = SNMP_OCT; // header
		sp->pCursor++;
		*sp->pCursor = sp->valueLength; // pituus, huom max. 255(?)
		sp->pCursor++;
		for (i = 0; i < sp->valueLength; i++)
		{
			*sp->pCursor = *pTmp;
			sp->pCursor++;
			pTmp++;
		}
		*/

	} else if ( sp->valueType == SNMP_NULL ) {
		*sp->pCursor = 0x05;
		sp->pCursor++;
		*sp->pCursor = 0x00;
		sp->pCursor++;
	}


	//aseta pituudet kohdilleen
	*sp->rSnmpMsgLen 		= sp->pCursor - sp->rSnmpMsgLen -1;
	*sp->rSnmpPduLen 		= sp->pCursor - sp->rSnmpPduLen -1;
	*sp->rVarbindListLen 	= sp->pCursor - sp->rVarbindListLen-1;
	*sp->rVarbindLen 		= sp->pCursor - sp->rVarbindLen-1;


	sp->length = sp->pCursor - sp->packet;
	//printf("snmp_construct_end\n");
	//fflush(stdout); //DEBUG
}

void snmp_trapConstruct (snmpPacket * sp)
{
	//printf("snmp_trapConstruct\n");
	//fflush(stdout); //DEBUG

	//allocate 255 bytes of memory
	//sp->packet = malloc( 255*sizeof(uint8_t) );
	sp->packet = snmp_valuealloc(PACKET_SIZE);

	sp->availableBytes = PACKET_SIZE;
	sp->pCursor = sp->packet;
	sp->rSnmpMsgLen = &sp->packet[1];

	//SNMP MESSAGE

	*sp->pCursor = 0x30;
	sp->pCursor++;
	*sp->pCursor = 0x00;
	sp->pCursor++;

	//SNMPv1
	*sp->pCursor = 0x02;
	sp->pCursor++;
	*sp->pCursor = 0x01;
	sp->pCursor++;
	*sp->pCursor = 0x00;
	sp->pCursor++;

	//SNMP Community String

	*sp->pCursor = 0x04;
	sp->pCursor++;
	*sp->pCursor = 0x0A;
	sp->pCursor++;


	// B
	*sp->pCursor = 0x42;
	sp->pCursor++;

	// a
	*sp->pCursor = 0x61;
	sp->pCursor++;

	// t
	*sp->pCursor = 0x74;
	sp->pCursor++;

	// t
	*sp->pCursor = 0x74;
	sp->pCursor++;

	// l
	*sp->pCursor = 0x6c;
	sp->pCursor++;

	// e
	*sp->pCursor = 0x65;
	sp->pCursor++;

	// s
	*sp->pCursor = 0x73;
	sp->pCursor++;

	// h
	*sp->pCursor = 0x68;
	sp->pCursor++;

	// i
	*sp->pCursor = 0x69;
	sp->pCursor++;

	// p
	*sp->pCursor = 0x70;
	sp->pCursor++;

	// PDU
	*sp->pCursor = SNMP_TRAP;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rSnmpPduLen = sp->pCursor;
	sp->pCursor++;

	// OID

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x07;
	sp->pCursor++;

	*sp->pCursor = 0x2b;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x03;
	sp->pCursor++;

	*sp->pCursor = 0x37;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->pCursor++;

	*sp->pCursor = sp->objectID;
	sp->pCursor++;

	// ADDR
	*sp->pCursor = SNMP_ADDR;
	sp->pCursor++;

	*sp->pCursor = 0x04;
	sp->pCursor++;

	*sp->pCursor = 0x10;
	sp->pCursor++;

	*sp->pCursor = 0x10;
	sp->pCursor++;

	*sp->pCursor = 0x07;
	sp->pCursor++;

	int ip;
	sscanf(inet_ntoa(confnet.cdn_ip_addr), "10.10.7.%d", &ip);

	//printf("%d, %x, %s\n", ip, ip, inet_ntoa(confnet.cdn_ip_addr));

	*sp->pCursor = ip;
	sp->pCursor++;


	// Trap types, generic = 6, specific = 0
	*sp->pCursor = 0x02;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x02;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->pCursor++;

	// Timestamp hardcoded to zero
	*sp->pCursor = 0x43;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->pCursor++;

	//Varbind list
	*sp->pCursor = 0x30;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rVarbindListLen = sp->pCursor;
	sp->pCursor++;

	//Varbind
	*sp->pCursor = 0x30;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->rVarbindLen = sp->pCursor;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x07;
	sp->pCursor++;

	*sp->pCursor = 0x2b;
	sp->pCursor++;

	*sp->pCursor = 0x06;
	sp->pCursor++;

	*sp->pCursor = 0x01;
	sp->pCursor++;

	*sp->pCursor = 0x03;
	sp->pCursor++;

	*sp->pCursor = 0x37;
	sp->pCursor++;

	*sp->pCursor = 0x00;
	sp->pCursor++;

	*sp->pCursor = sp->objectID;
	sp->pCursor++;

	sp->pCursor = AsnOctetStringBuild(sp->pCursor, &sp->availableBytes, SNMP_OCT, sp->value, sp->valueLength);

	*sp->rSnmpMsgLen 		= sp->pCursor - sp->rSnmpMsgLen -1;
	*sp->rSnmpPduLen 		= sp->pCursor - sp->rSnmpPduLen -1;
	*sp->rVarbindListLen 	= sp->pCursor - sp->rVarbindListLen-1;
	*sp->rVarbindLen 		= sp->pCursor - sp->rVarbindLen-1;

	sp->length = sp->pCursor - sp->packet;
	//printf("snmp_trapConstruct_end\n");
	//fflush(stdout); //DEBUG
}
