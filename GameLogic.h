/**
 * \file GameLogic.h
 * A Documented file.
 */
#ifndef GAMELOGIC_H
#define GAMELOGIC_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "UI.h"
//#include "Common.h"
#include "AI.h"
#include "Sound.h"
#include "Network.h"
#include "NetHandler.h"

#define WAIT_TIMEOUT 	-1
#define WAIT_OK 		0
#define WAIT_QUIT		1

/**
 * Control struct.
 * Contains the players and the user interface.
 */
typedef struct
{
	Player * player;
	Player * enemy;
	UI * ui;
} GameLogic;

/**
* Constructor
*@param ui the user interface
*@return a GameLogic instance
*/
GameLogic *	GL_makeGame(UI * ui);

/**
* Destructor
*@param game
*/
void GL_destructGame(GameLogic * game);

/**
* Enables the user to place a ship.
* @param shipIndex
* @param game a GameLogic struct
* @return false if the user fails to set his ships and wants to return to the main menu
*/
bool 		GL_setShip(int shipIndex, GameLogic * game);

/**
* Controls the sequence of getting shot coordinates from the user.
* @param game a GameLogic struct
* @param [out] myShots the required shots
* @return false if all the possible shots has been fired
*/
bool		GL_getShots(GameLogic * game, Grids * myShots);

/**
* Executes the single player game.
* @param ui the user interface
*/
void 		GL_singlePlayerGame(UI * ui);

/**
* Executes the multiplayer game.
* @param ui the user interface
* @param nh a nethandler object
*/
void		GL_multiPlayerGame(UI * ui, NH * nh);


/**
 * Waits for a given flag.
 * Also listens to the user: does he want to quit?
 * @param ui the user interface
 * @param nh a nethandler object
 * @param flag a pointer to the flag to listen
 */
int GL_waitForFlag(UI * ui, NH * nh, bool * flag, int timeout);

void GL_leaveGame(NH * nh, UI * ui, GameLogic * game);

bool GL_startTurn(NH * nh, UI * ui, int turn, bool master);

THREAD(mainFunc, arg);

#endif
