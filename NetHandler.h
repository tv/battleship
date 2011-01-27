/**
 * \file NetHandler.h
 * NetHandler is used for ouputting data through ethernet
 */
#ifndef NETHANDLER_H
#define NETHANDLER_H

#define MAX_MESSAGES 5
#define MAX_MSG_LENGTH 100

#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "snmp.h"
#include "common.h"

typedef struct
{
	uint32_t* 		opponentList; 	//list of opponents
	uint32_t   		opponent;		//current opponent
	int				numOpponents;	//number of available opponents
	int				currentOID;		//the protocol phase
	bool			readyToPlay;	//flag: auto response to GET-REQUEST messages
	bool			openRequest;	//flag: is there an open (unanswered) SET-REQUEST? NOTE: this flag is set by the network listener
	bool			challengeSend;
	bool			challengeAccepted;
	bool			getOpponentList;
	bool			enemyStartedTurn;
	bool			enemyOK;
	bool			enemySalvoReceived;
	bool			resultsQueryReceived;
	bool			salvoResultsReceived;

	Grids*			lastEnemySalvo;
	uint8_t*		salvoResults;		// results of our shots
	uint8_t* 		enemyShotResults;	// results of enemy shots
	uint8_t*		chatMsgs[MAX_MESSAGES];		//chat messages
	uint8_t*		trapMessage;
	int				numMsgs;		//number of chat messages
	int				emptyMsg;		//index of the empty message

	//snmpPacket *	lastPacketFromOpponent;
	bool			lastPacketParsed;
	long			requestID;


	UDPSOCKET 		*udpSock;
	UDPSOCKET 		*udpTrapSock;


} NH;

/**
* Constructor
*/
NH * NH_construct(void);

/**
 * Destructor
 */
void NH_destruct(NH * nh);

/**
* Initializes all the flags and variables in the given NH struct
* @param nh pointer to NetHandler
*/
void NH_init(NH * nh);


/**
 * Sends snmp packet to address and port
 * @param nh 		pointer to NetHandler
 * @param addr 		address of the receiver, from inet_addr()
 * @param port 		where to send the data
 * @param command 	snmp command type
 * @param valuetype 	type of the value
 * @param value 		pointer to the value to be send
 * @param oid		oid of the packet
 * @param reqid		Request ID of the packet, 0 to randomize
 * @return reqid		Request id of the send packet
 */
long NH_sendSnmpPacket(NH *nh, uint32_t addr, uint8_t port, uint8_t command, uint8_t valuetype, void *value,  uint8_t oid, long reqid);

long NH_sendSnmpPacketOCT(NH *nh, uint32_t addr, uint8_t port, uint8_t command, uint8_t valuetype, void *value, size_t valueLength, uint8_t oid, long reqid);
/**
 * Sends generic snmp trap packet to opponent
 * @param nh 		pointer to NetHandler
 * @param msg 		quit message
 */
void NH_sendTrap(NH *nh, char * msg);

long NH_sendSnmpError(NH *nh, uint32_t addr, uint8_t port, int command, int oid, int reqid);


/**
* Sends SET-REQUEST to chosen opponent
* @param nh a NetHandler (NH) struct
* @param opponentID the index of the opponent in our opponentlist
*/
void NH_sendChallenge(NH * nh, int opponentID);


/**
* Returns the opponent list
* @param nh a NetHandler (NH) struct
* @param [out]  opponentList
* @return number of opponents
*/
int NH_getOpponentList(NH * nh);


/**
* @param nh a NetHandler (NH) struct
* @param oid oid of the package
* @param userResponse as boolean value
* @return true if there is an unaswered SET REQUEST
* @note the NH input handler -thread must set nh->opponent and nh->lastPacketFrom opponent when a SET REQUEST is received
*/
void NH_sendBooleanResponse(NH * nh, int oid, bool userResponse);

/**
* Sends a SET REQUEST message with an OID 2, value being the current turn
* @param nh a NetHandler (NH) struct
* @param turn the number of the current round
*/
void NH_sendStartTurn(NH * nh, int turn);

/**
* Sends the shots to opponent as a SET-REQUEST message (SALVO)
* @param nh a NetHandler (NH) struct
* @param myShots
* @param iShots number of shots to fire
*/
void NH_sendShots(NH * nh, Grids * myShots, int iShots);


bool NH_getShotResults(NH * nh, uint8_t * myShotResults);

void NH_rotateMessages(NH * nh);


#endif
