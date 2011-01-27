/**
 * \file Player.h
 * A Documented file.
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "common.h"

/**
 * Square struct.
 * Represents the squares in the game board matrix (Player.area).
 */
typedef struct
{
	int shipID;		/**< ID of the ship that occupies this square*/
	uint8_t hit; 		/**< see common.h: shots*/
	uint8_t AImark;		/**< field used by the AI to remember previous decisions */
} Square;


/**
 * Player struct.
 * Contains the player's gameboard and needed information of the ships.
 */
typedef struct
{
	Square** area;							/**< BOARD_SIZE * BOARD_SIZE matrix */
	uint8_t numShips;						/**< number of ships left */
	char * shipHPs; 						/**< shipHitpoints: 5, 4, 3, 3, 2 */
	uint8_t totalHPs;						/**< total count of hitpoints */
	uint8_t shotsFired; 						/**< number of shots the player has fired */
} Player;


/**
* A constructor.
* Initializes and returns a Player struct.
* @return an initilized Player struct
*/
Player * P_makePlayer(void);

/**
* Destructor
*@param player
*/
void P_destructPlayer(Player * player);

/**
   * marks enemy shots to the player area, and returns true if the player was terminated
   * @param player a Player struct
   * @param shots an array of enemy shots
   * @param numShots length of the shots array
   * @param [out] results the results of the shots
   * @return true if all the players ships have sunk
   */
bool P_markShots(Player * player, Grids * shots, int numShots, uint8_t * results);

/**
   * marks our shots to virtual enemy area, and return true if the player was terminated
   * note: this is a network (NW) version
   * @param player a Player struct
   * @param shots an array of enemy shots
   * @param numShots length of the shots array
   * @param [in] results the results of our shots
   * @return true if all the players ships have sunk
   */
bool P_NHmarkShots(Player * player, Grids * shots, int numShots, uint8_t * results);

/**
   * marks enemy shots to the player area, and returns true if the player was terminated
   * @param player a Player struct
   * @param shots an array of enemy shots
   * @param numShots length of the shots array
   * @param [out] results the results of the shots
   * @return true if all the players ships have sunk
   */
bool P_markOpponentShots(Player * player, Grids * shots, uint8_t * shotResults, int numShots);

/**
   * tests if a  ship square touches another
   * @param player a Player struct
   * @param x coordinate
   * @param y coordinate
   * @param fwd integer direction
   * @return true if the player can not set a ship to this square
   */
bool P_touchesAShip(Player * player, int x, int y, int fwd);

/**
   * tests if a  ship fits into the given location
   * @param shipLength
   * @param cursor
   * @param player A Player struct
   * @param dir An integer direction
   * @return true if the ship fits here
   */
bool P_testGhost(int shipLength, Grids * cursor, Player * player, int dir);



#endif
