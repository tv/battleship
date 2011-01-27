#include "AI.h"

void swap(uint8_t * first, uint8_t * second)
{
	uint8_t tmp = *first;
	*first = *second;
	*second = tmp;
}

AI * AI_construct(Player * cpuPlayer, Player * human)
{
	AI * ai = (AI*)malloc(sizeof(AI));
	
	ai->cpu = cpuPlayer;
	ai->human = human;
	
	uint8_t b = BOARD_SIZE/2;
	uint8_t i = 0;
	
	ai->rowOrder = (uint8_t*)malloc(BOARD_SIZE * sizeof(uint8_t));
	ai->colOrder = (uint8_t*)malloc( BOARD_SIZE/2 * sizeof(uint8_t));
	ai->lastShots = (Grids*)malloc(5 * sizeof(Grids));
	ai->foundShips = (AIShip*)malloc(5 * sizeof(AIShip));
	
	ai->rowPtr = 0; ai->colOffset = 0; ai->shipCount = 0;
	
	//init
	for(i = 0; i < b; i++) ai->colOrder[i] = i;
	for(i = 0; i < BOARD_SIZE; i++) ai->rowOrder[i] = i;
	
	//shuffle
	for(i = 0; i < BOARD_SIZE; i++) swap(&ai->rowOrder[i], &ai->rowOrder[rand() % BOARD_SIZE]);
	for(i = 0; i < b; i++) swap(&ai->colOrder[i], &ai->colOrder[rand() % b]);
	
	return ai;
}

void AI_destruct(AI * ai)
{
	free(ai->rowOrder);
	free(ai->colOrder);
	free(ai->lastShots);
	free(ai->foundShips);
}

void AI_setShips(AI * ai)
{
	bool retry;
	Grids cursor;
	int i, j, k, dir, shipLength;
	for(i = 0; i < 5; i++)
	{
		retry = false;
		
		//set the ship randomly
		cursor.x = rand() % BOARD_SIZE; 				//0...9
		cursor.y = rand() % BOARD_SIZE;					//0..9
		dir = rand() % 2;								//0 or 1
		shipLength = C_shipLength(i);
		
		//test if the ship fits here
		for(k = 0; k < 2; k++)
		{
			dir = !dir;
			if( !P_testGhost( shipLength, &cursor, ai->cpu, dir) ) retry = true;
			else { retry = false; break; }
		}
		
		//retry if the position failed
		if(retry)
		{ i--; continue; }
		
		//set ship
		for(j = 0; j < shipLength; j++)
			ai->cpu->area[cursor.x + dir * j][cursor.y + (!dir) * j].shipID = i; 
	}
}

bool AI_getShots(AI * ai, Grids * shots, uint8_t numShots)
{
	AIShip * ship;
	int i, j, k, x, y, tx, ty, sign, badSide, dir, tmp, iShip;
	char m[2] = {2,2}; 
	
	i = 0;
	iShip = 0;
	
	//first shoot at the know ships
	while(iShip < ai->shipCount && i < numShots)
	{	
		ship = &ai->foundShips[iShip];
		x = ship->firstHit.x;
		y = ship->firstHit.y;
		if(ai->human->area[x][y].hit == SUNK) 
		{ 	iShip++; continue; }												
		
		if(ship->direction == 2)	//previously we have found an enemy ship, but we dont know the orientation of the ship
		{
			//printf("\nHaarukoidaan kohdan %d, %d laiva\n", x, y);	//DEBUG
			//sleep(2);												//DEBUG
			
			for(dir = 0; dir < 2; dir++)
			{
				m[dir] = 2;
				for(k = -1; k < 2; k += 2)
				{
					tx = x + dir * k; ty = y + (!dir) * k;
					if(C_outOfBoard(tx, ty)
					|| ai->human->area[tx][ty].hit != NO_HIT)
					{
						m[dir]--;
						continue;
					}
					else if(i < numShots && ai->human->area[tx][ty].hit == NO_HIT)
					{
						ai->human->area[tx][ty].AImark = 2;
						shots[i].x = tx;
						shots[i].y = ty;
						i++;
					}
				}
			}
			for(dir = 0; dir < 2; dir++)
			{ if(m[dir] == 0) ship->direction = !dir; }
		}
		if(ship->direction < 2) //previously we have shot this ship and we do know the orientation of the ship
		{
			//printf("\nUpotetaan kohdan %d, %d laiva\n", x, y);	//DEBUG
			//sleep(2);
			
			dir = ship->direction;
			k = 1;
			badSide = -1;
			while(i < numShots && badSide < 2 && k < 5) // k-loop
			{
				sign = 1;
				for(j = 0; j < 2; j ++)
				{
					sign *= -1;
					tx = x + sign  * dir * k;
					ty = y + sign * (!dir) * k;
					if(C_outOfBoard(tx, ty) || j == badSide || badSide > 2) 
						continue;
										
					tmp = ai->human->area[tx][ty].hit;
					if(tmp == MISS || tmp == SUNK)
					{
						if(badSide == -1) badSide = j;
						else badSide = 10;	//BOTH
					}
					else if(tmp == NO_HIT && ai->human->area[tx][ty].AImark != 2)
					{
						shots[i].x = tx;	
						shots[i].y = ty;
						i++;
					}
				}
				k++;
			}
		}
		iShip++;
	}
	
	//shoot rest of the shots randomly
	while(i < numShots)
	{
		//shoot according to the plan
		tmp = ai->rowPtr + ai->colOffset;
		while(tmp >= BOARD_SIZE / 2) tmp -= BOARD_SIZE / 2;
		y = ai->rowOrder[ai->rowPtr];
		x = ai->colOrder[tmp] * 2 + y % 2;
		
		if(ai->human->area[x][y].hit == NO_HIT)
		{
			ai->human->area[x][y].AImark = 1;
			shots[i].x = x;
			shots[i].y = y;
			i++;
		}
		
		//cyclic convolution
		ai->rowPtr++;
		if(ai->rowPtr == BOARD_SIZE)
		{
			ai->rowPtr = 0;
			ai->colOffset++;
			if(ai->colOffset >= BOARD_SIZE / 2)	//no more squares to be shot at...
			{
				while(i < numShots)
				{
					if(ai->human->area[x][y].hit == NO_HIT)
					{ shots[i].x = i; shots[i].y = i; i++; }
				}
			}
		}
	}
	//save the fired shots
	for(i = 0; i < numShots; i++) ai->lastShots[i] = shots[i];
	return true;
}


void AI_setResults(AI * ai, uint8_t * shotResults, uint8_t numShots)
{
	int i, x, y, k, n, id, dir, duplicate;
	for(i = 0; i < numShots; i++)
	{
		id = ai->human->area[ai->lastShots[i].x][ai->lastShots[i].y].shipID;
		if(shotResults[i] == 1) //a hit
		{	
			if(ai->human->area[ai->lastShots[i].x][ai->lastShots[i].y].AImark == 1) //we have found a new ship with a random shot
			{
				duplicate = 0;
				for(k = 0; k < ai->shipCount; k++)
				{ if(ai->foundShips[k].shipID == id) duplicate = 1; }
				if(!duplicate)
				{
					ai->shipCount++;
					ai->foundShips[k].firstHit.x = ai->lastShots[i].x; 
					ai->foundShips[k].firstHit.y = ai->lastShots[i].y; 
					ai->foundShips[k].direction = 2;
					ai->foundShips[k].shipID = id;
				}
			}
			else //we have hit a previously found ship
			{
				//find out the orientation of the found ship
				for(dir = 0; dir < 2; dir++) 
				{
					for(k = -1; k < 2; k += 2)
					{
						x = ai->lastShots[i].x + dir * k;
						y = ai->lastShots[i].y + (!dir) * k;
						if( C_outOfBoard(x,y) ) continue;
						if(ai->human->area[x][y].hit == HIT)
						{
							//find out which ship was hit
							for(n = 0; n < ai->shipCount; n++)
							{
								if(ai->foundShips[n].shipID == id) //found the target 
									ai->foundShips[n].direction = dir;
							}
						}
					}
				}
			}
		}
	}
}
