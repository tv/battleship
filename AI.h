/**
 * \file AI.h
 * A Documented file.
 */
#ifndef AI_H
#define AI_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>	//DEBUG
#include <unistd.h> //DEBUG
#include <sys/types.h>

#include "Common.h"
#include "Player.h"


/**
 * /struct AIShip
 * A sub struct of AI: information of an unfinished ship. Works as a linked list.
 */
typedef struct
{
	uint8_t shipID;
	Grids firstHit;
	uint8_t direction;
} AIShip;


/**
 * AI struct.
 * Contains the queue of unfinished ships.
 */
typedef struct
{
	//array of found ships
	AIShip * foundShips;
	uint8_t shipCount;

	//players
	Player * cpu;
	Player * human;

	//shooting plan
	uint8_t * rowOrder;
	uint8_t * colOrder;
	uint8_t rowPtr;
	uint8_t colOffset;

	Grids * lastShots;
} AI;

/**
* Constructor.
* @param cpuPlayer
* @param humanPlayer
* @return the constructed AI
*/
AI * AI_construct(Player * cpuPlayer, Player * humanPlayer);

/**
* Destructor.
* @param ai the AI struct
*/
void AI_destruct(AI * ai);

/**
* Commands the cpu to place ships.
* @param ai the AI struct
*/
void AI_setShips(AI * ai);

/**
* Commands the cpu to shoot.
* @param ai the AI struct
* @param [out] shots a Grids struct, where AI saves the shots it makes
* @param numShots the number of shots the AI has
* @return true on success
*/
bool AI_getShots(AI * ai, Grids * shots, uint8_t numShots);

/**
* Commands the cpu to shoot.
* @param ai the AI struct
* @param [out] shotResults an array of shot results
* @param numShots the number of shots the AI had
* @return true on success
*/
void AI_setResults(AI * ai, uint8_t * shotResults, uint8_t numShots);


#endif
