#include "Player.h"


Player * P_makePlayer()
{
	Player * newPlayer = (Player*)(malloc(sizeof(Player)));;
	newPlayer->area = (Square**)(malloc(BOARD_SIZE * sizeof(Square*)));
	int i, j;
	for(i = 0; i < BOARD_SIZE; i++) {
		newPlayer->area[i] = (Square*)(malloc(BOARD_SIZE * sizeof(Square)));
		for(j = 0; j < BOARD_SIZE; j++)
		{
			newPlayer->area[i][j].shipID = EMPTY_SQUARE;
			newPlayer->area[i][j].hit = NO_HIT;
			newPlayer->area[i][j].AImark = 0;
		}
	}
	newPlayer->shipHPs = (char*) malloc(5 * sizeof(uint8_t));
	for(i = 0; i < 5; i++)
		newPlayer->shipHPs[i] = C_shipLength(i);
	newPlayer->totalHPs = 17;
	newPlayer->numShips = 5;
	newPlayer->shotsFired = 0;
	return newPlayer;
}

void P_destructPlayer(Player * player)
{
	int i;
	for(i = 0; i < BOARD_SIZE; i++)
		free(player->area[i]);
	free(player->area);
	free(player->shipHPs);
	free(player);
}

bool P_markShots(Player * player, Grids * shots, int numShots, uint8_t * results)
{
	int i, j, dir, shipID, len, x, y;
	Square * tmpSquare;
	for(i = 0; i < numShots; i++)
	{
		if(shots[i].x > BOARD_SIZE - 1 || shots[i].y > BOARD_SIZE -1)
		{
			results[i] = 0;
			continue;
		}
		tmpSquare = &player->area[shots[i].x][shots[i].y];
		results[i] = 0;
		tmpSquare->hit = MISS;
		if(tmpSquare->shipID != EMPTY_SQUARE)			//hit
		{
			results[i] = 1;
			tmpSquare->hit = HIT;
			player->shipHPs[tmpSquare->shipID]--;
			player->totalHPs--;
			if(player->shipHPs[tmpSquare->shipID] == 0) //ship was sunk
			{
				shipID = tmpSquare->shipID;
				len = C_shipLength(shipID);
				results[i] = len;

				//mark the ship sunk
				for(dir = 0; dir < 2; dir++) {
					for(j = -1; j < 2; j += 2) {
						x = shots[i].x + dir * j; y = shots[i].y + (!dir) * j;
						while(!C_outOfBoard(x, y) && player->area[x][y].shipID == shipID)
						{
							player->area[x][y].hit = SUNK;
							x += j * dir; y += j * (!dir);
						}
					}
				}
				tmpSquare->hit = SUNK;

				player->numShips--;
				if(player->numShips == 0) return true; //all the ships are sunk
			}
		}
	}
	return false;
}


bool P_NHmarkShots(Player * player, Grids * shots, int numShots, uint8_t * shotResults)
{
	int i, j, dir, x, y;
	Square * tmpSquare;
	//printf("Marking shots to virtual enemy...\n");
	for(i = 0; i < numShots; i++)
	{
		tmpSquare = &player->area[shots[i].x][shots[i].y];
		//printf("%d: %d, %d, result: %d, previoushit: %d\n", i, shots[i].x, shots[i].y, shotResults[i], tmpSquare->hit);
		if(shotResults[i] > 0 && tmpSquare->hit == GHOST_SHOT) // a ship was hit
		{
			player->totalHPs--;
			if(shotResults[i] == 1) tmpSquare->hit = HIT;
			else //a ship was sunk
			{
				tmpSquare->hit = SUNK;
				//mark the ships as "sunk" to the board
				for(dir = 0; dir < 2; dir++) {
					for(j = -1; j < 2; j += 2) {
						x = shots[i].x + dir * j; y = shots[i].y + (!dir) * j;
						while(!C_outOfBoard(x, y) && player->area[x][y].hit == HIT)
						{
							player->area[x][y].hit = SUNK;
							x += j * dir; y += j * (!dir);
						}
					}
				}
				player->numShips--;
					if(player->numShips == 0) return true; //all the ships are sunk
			}
		}
		else
			tmpSquare->hit = MISS;
	}
	return false;
}


bool P_touchesAShip(Player * player, int x, int y, int fwd)
{
	int i, dir, dx, dy;
	for(dir = 0; dir < 2; dir++) {
		for(i = -1; i < 2; i += 2) {
			if(dir == fwd && i == - 1) continue;
			dx = x + i*dir; dy = y + i*(!dir);
			if(!C_outOfBoard(dx, dy)
			&& ( player->area[dx][dy].shipID != EMPTY_SQUARE) )
				return true;
		}
	}
	return false;
}


bool P_testGhost(int shipLength, Grids * cursor, Player * player, int dir)
{
	bool ok = true;
	int i;
	for(i = 0; i < shipLength; i++ )
	{
		if( (dir*i + cursor->x >= BOARD_SIZE)	//out of board?
		|| (!dir*i + cursor->y >= BOARD_SIZE)
		|| (i > 0 && player->area[cursor->x + dir*i][cursor->y + !dir * i].shipID != EMPTY_SQUARE) )
		{
			ok = false;
			break;
		}
		if(P_touchesAShip(player, cursor->x + dir*i, cursor->y + !dir * i, 5*(i == 0) + dir) )
			ok = false;
	}
	return ok;
}

