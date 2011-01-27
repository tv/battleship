#include "GameLogic.h"

#ifdef __arm__
#include <cfg/os.h>
#include <io.h>
#include <dev/board.h>

#include <sys/version.h>
#include <sys/thread.h>
#include <sys/timer.h>
#include <sys/event.h>

#endif

extern const char * CHOOSE_DIRECTION;

GameLogic * GL_makeGame(UI * ui)
{
	GameLogic * game = (GameLogic*)malloc(sizeof(GameLogic));
	game->player = P_makePlayer();
	game->enemy = P_makePlayer();
	game->ui = ui;
	ui->player = game->player;
	ui->enemy = game->enemy;
	ui->cursorTable = HOME;
	ui->visibleTable = HOME;
	return game;
}

void GL_destructGame(GameLogic * game)
{
	P_destructPlayer(game->player);
	P_destructPlayer(game->enemy);
	free(game);
}


bool GL_setShip(int shipIndex, GameLogic * game)
{
	int shipLength, dir, i;
	shipLength = C_shipLength(shipIndex);
	bool success = false;
	dir = 0;
	while(!success)
	{
		sprintf(game->ui->message, "Aseta laivasi nro %d (pituus %d).\n", shipIndex + 1, shipLength); //TODO: static strings
		if(!UI_selectUsingCursor(game->ui)
		|| game->player->area[game->ui->cursor.x][game->ui->cursor.y].shipID != EMPTY_SQUARE)
		{
			if(game->ui->visibleTable == MENU) return false;
			continue;
		}
		sprintf(game->ui->message, CHOOSE_DIRECTION );
		success = UI_readDirection(game->ui, shipLength, &dir);
		if(success)
		{
			for(i = 0; i < shipLength; i++ )
				game->player->area[game->ui->cursor.x + dir * i][game->ui->cursor.y + (!dir) * i].shipID = shipIndex;
			TUI_printArea(game->ui);
		}
		else if(game->ui->visibleTable == MENU) return false;
	}
	return true;
}

bool GL_getShots(GameLogic * game, Grids * myShots)
{
	game->ui->visibleTable = 0; game->ui->cursorTable = 0; //away
	int i = 0;
	bool success;
	while(i < game->player->numShips)
	{
		success = false;
		while(!success)
		{
			sprintf(game->ui->message, "Valitse ruudut, johon ammut (%d/%d)\n", i + 1, game->player->numShips);
			if(!UI_selectUsingCursor(game->ui))
			{
				if(game->ui->visibleTable == MENU) return false;	//user wants to return to main menu
				if(i > 0) //undo
				{
					game->enemy->area[myShots[i-1].x][myShots[i-1].y].hit = NO_HIT;
					i -= 2;
					break;
				}
				continue;
			}

			if(game->enemy->area[game->ui->cursor.x][game->ui->cursor.y].hit != NO_HIT) continue;	//invalid coordinates
			else //succesful choice
			{
				success = true;
				game->enemy->area[game->ui->cursor.x][game->ui->cursor.y].hit = GHOST_SHOT;
				myShots[i].x = game->ui->cursor.x; myShots[i].y = game->ui->cursor.y;
			}
		}
		i++;
		if(i + game->player->shotsFired == BOARD_SIZE * BOARD_SIZE) return false;	//all possible shots has been made
	}
	game->player->shotsFired += game->player->numShips;
	return true;
}


void GL_singlePlayerGame(UI * ui)
{
	GameLogic * game = GL_makeGame(ui);

	AI * ai = AI_construct(game->enemy, game->player);

	//setp ships
	int i;
	for(i = 0; i < 5; i++)
	{
		if( !GL_setShip(i, game) )
		{
			GL_destructGame(game);
			AI_destruct(ai);
			return;
		}
	}

	AI_setShips(ai);
	TUI_showMessage(ui, "Laivat on asetettu. Oletko valmis? Peli alkaa napin painalluksesta.");

	//start shooting
	bool victory, loss;
	Grids myShots[5];
	Grids enemyShots[5];
	uint8_t shotResults[5];
	uint8_t iEnemyShots, iShots;

	do
	{
		iEnemyShots = game->enemy->numShips;
		iShots = game->player->numShips;
		if(!GL_getShots(game, myShots)) break; 											//get the shots from user

		AI_getShots(ai, enemyShots, iEnemyShots);										//get the shots from AI

		TUI_shotAnimation(ui);		//NOTE: not necessary at all

		victory = P_markShots(game->enemy, myShots, iShots, shotResults);
		loss = P_markShots(game->player, enemyShots, iEnemyShots, shotResults);
		AI_setResults(ai, shotResults, iEnemyShots);

	} while(!victory && !loss);

	//inform of the winner
	if(victory || loss)
	{
		if(victory && loss)
			sprintf(ui->message, "Peli päättyi tasapeliin!\n\nEnterillä valikkoon.\n");
		else if(victory)
			sprintf(ui->message, "Hienoa, voitit!\n\nEnterillä valikkoon.\n");
		else
			sprintf(ui->message, "\nAijai, hävisit tekoälylle.\n\nEnterillä valikkoon.\n");
		TUI_printArea(ui);
		TUI_getEnter(ui);
	}
	ui->visibleTable = MENU;
	//destructor calls
	GL_destructGame(game);
	AI_destruct(ai);

}

void GL_multiPlayerGame(UI * ui, NH * nh)
{
	GameLogic * game = GL_makeGame(ui);
	ui->visibleTable = OPPONENT_MENU;
	sprintf(ui->message, " ");
	int numOpponents, i, cursor, input, turn;
	char tmpStr[60];
	cursor = -2; //"back to menu"
	turn = 1;
	bool quit = false;
	bool userResponse, master;
	master = false;

	NH_init(nh);
	nh->readyToPlay = true; //set "GET-REQUEST" response to 1: we are ready to play
	nh->currentOID = 2;		//note: this does not include the ready-to-play messages!

	//opponent selection
	numOpponents = NH_getOpponentList(nh); //create opponent list
	TUI_printOpponentMenu(ui, nh->opponentList, numOpponents, cursor);
	while(ui->visibleTable == OPPONENT_MENU)
	{
		if(nh->openRequest == true) //some opponent has challenged us to a match
		{
			sprintf(tmpStr, "%s haastaa sinut. Hyväksytkö haasteen (k/e)?\n", inet_ntoa(nh->opponent));
			userResponse = TUI_booleanRequest(ui, tmpStr); //ask the user if he wants to play against this opponent
			master = false;
			NH_sendBooleanResponse(nh, nh->currentOID, userResponse); //send response to the opponent
			nh->openRequest = false;
			if(userResponse) //user wishes to play
			{
				if(nh->opponent != 0) break; 	//ok, game starts
				else 							//opponent has canceled the challenge
					TUI_showMessage(ui, "Vastustajasi ehti perua haasteen.");
			}
			else
			{
				nh->opponent = 0;				//user doesn't want to play
				TUI_printOpponentMenu(ui, nh->opponentList, numOpponents, cursor);
			}
		}
		input = UI_opponentSelect(ui, nh->opponentList, numOpponents, &cursor);
		if( input == TIME_OUT || cursor == -1)
		{
			if(input != TIME_OUT)
			{
				numOpponents = NH_getOpponentList(nh);	//update opponent list
				TUI_printOpponentMenu(ui, nh->opponentList, nh->numOpponents, cursor);
			}
			continue;
		}
		if(cursor == -2) //user wants to return to the main menu
		{
			quit = true;
			break;
		}
		else //user wants to challenge an opponent
		{
			nh->opponent = nh->opponentList[cursor]; //set the opponent
			nh->openRequest = true;
			NH_sendChallenge(nh, cursor);
			master = true;
			sprintf(ui->message, "Odotetaan vastustajan vastausta.\nVoit perua haasteen undo-näppäimellä.");
			TUI_printOpponentMenu(ui, nh->opponentList, nh->numOpponents, cursor);
			userResponse = true;
			for(;;) //wait for opponent response
			{
				if( (ui->frontCommand != NULL && UI_getInput(ui, 0) == UNDO_BUTTON) || (nh->opponent == 0) )
				{
					userResponse = false;
					if(nh->opponent != 0) //user wants to cancel the challenge
					{
						nh->challengeSend = false;
						NH_sendTrap(nh, "Cancel the challenge");
						sprintf(ui->message, " ");
					}
					else
						sprintf(ui->message, "Vastustaja hylkäsi haasteen");
					TUI_printOpponentMenu(ui, nh->opponentList, nh->numOpponents, cursor);
					nh->opponent = 0;
					break;
				}
				else if(nh->challengeAccepted) break;
				NutSleep(300);	//sleep ~0.3s
			}
			nh->openRequest = false;
			if(userResponse) break;
		}
	}
	nh->readyToPlay = false;	//we are no longer available

	if(quit) 					//the user wants to return to main menu
	{ GL_destructGame(game); return; }

	//set ships
	ui->visibleTable = HOME;
	nh->currentOID = 2;
	for(i = 0; i < 5; i++)
	{
		if( !GL_setShip(i, game) ) //user wants to exit the  game
		{ quit = true; break; }
	}

	if(quit)
	{ GL_leaveGame(nh, ui, game); return; }

	sprintf(ui->message, "Odotetaan vastustajaa...");
	TUI_printArea(ui);

	//start game
	bool victory, loss;
	Grids myShots[5];
	uint8_t iEnemyShots, iShots;
	int waitResult = WAIT_TIMEOUT;

	//salvo loop
	do
	{
		nh->currentOID = 2;
		iEnemyShots = game->enemy->numShips;
		iShots = game->player->numShips;

		//START-TURN
		//printf("GL: Round %d is about to begin\n", turn);
		if(GL_startTurn(nh, ui, turn, master) ) //startTurn number 1
		{ GL_leaveGame(nh, ui, game); return; }

		//SALVO
		nh->currentOID = 3;

		if(!GL_getShots(game, myShots) || nh->opponent == 0) 	//get the shots from user
		{
			GL_leaveGame(nh, ui, game);
			//printf("Karmeat epäilykset toteutuivat!");
			return;
		}

		//send shots and wait for confirmation
		//printf("GL: send shots and wait for confirmation...\n");
		waitResult = WAIT_TIMEOUT;
		while(waitResult == WAIT_TIMEOUT) {
			NH_sendShots(nh, myShots, iShots);
			waitResult = GL_waitForFlag(ui, nh, &nh->enemyOK, 1000 * 10); //10 s interval
			//printf("Waiting for enemyOK (OID 3)\n");
		}
		if(WAIT_QUIT == waitResult) { GL_leaveGame(nh, ui, game); return; }

		//wait for enemy salvo
		//printf("GL: wait for enemy salvo, already received: %d\n", nh->enemySalvoReceived);
		sprintf(ui->message, "Odotetaan vastustajan laukauksia...\n");
		TUI_printArea(ui);

		if(WAIT_QUIT == GL_waitForFlag(ui, nh, &nh->enemySalvoReceived, 0))
		{ GL_leaveGame(nh, ui, game); return; }
		//NutSleep(100); //wait a while before asking for results

		//mark enemy shots
		loss = P_markShots(game->player, nh->lastEnemySalvo, iEnemyShots, nh->enemyShotResults);	//mark the enemy shots on our board

		//SALVO-RESULTS
		nh->currentOID = 4;

		//send salvo results query
		//printf("GL: send salvo results query\n");
		NH_sendSnmpPacket(nh, nh->opponent, 161, SNMP_GET_REQ, SNMP_NULL, NULL, nh->currentOID, 0);
		NutSleep(100);

		//wait for enemy query
		//printf("GL: wait for enemy query, already received: %d\n", nh->resultsQueryReceived);
		if(WAIT_QUIT == GL_waitForFlag(ui, nh, &nh->resultsQueryReceived, 0))
		{ GL_leaveGame(nh, ui, game); return; }
		//send salvo results
		//printf("GL: send salvo results\n");
		NH_sendSnmpPacketOCT(nh, nh->opponent, 161, SNMP_GET_RESP, SNMP_OCT, (void *)nh->enemyShotResults, 5, 4, 0);

		//wait for results, NOTE: results are stored in nh->salvoResults
		//printf("GL: wait for salvo results\n");
		waitResult = WAIT_TIMEOUT;
		while(waitResult == WAIT_TIMEOUT) {
			waitResult = GL_waitForFlag(ui, nh, &nh->salvoResultsReceived, 1000 * 5); //every 5 seconds, resend query
			if(waitResult == WAIT_TIMEOUT) //send new query
				NH_sendSnmpPacket(nh, nh->opponent, 161, SNMP_GET_REQ, SNMP_NULL, NULL, nh->currentOID, 0);
		}
		if(WAIT_QUIT == waitResult) { GL_leaveGame(nh, ui, game); return; }


		//printf("GL: ok, turn is done\n");

		//mark our shots to our local / virtual enemy
		victory = P_NHmarkShots(game->enemy, myShots, iShots, nh->salvoResults);
		turn++;
	} while(!victory && !loss);

	if(victory || loss)
	{
		//printf("\nDEBUG: Voitin tai hävisin!\n"); //DEBUG
		if(victory && loss)
		{
			sprintf(ui->message, "Peli päättyi tasapeliin!\n\nEnterillä valikkoon.\n");
			NH_sendTrap(nh, "Draw");
		}
		else if(victory)
		{
			sprintf(ui->message, "Hienoa, voitit!\n\nEnterillä valikkoon.\n");
			NH_sendTrap(nh, "I won");
		}
		else
		{
			sprintf(ui->message, "\nAijai, hävisit.\n\nEnterillä valikkoon.\n");
			NH_sendTrap(nh, "I lost");
		}
		TUI_printArea(ui);
		TUI_getEnter(ui);
	}
	nh->opponent = 0;
	ui->visibleTable = MENU;
	GL_destructGame(game); //destructor call
}


//return 1  if either the user or the enemy wants to quit
//return 0  if wait is successful
//return -1 on timeout
int GL_waitForFlag(UI * ui, NH * nh, bool * flag, int timeout)
{
	bool quit = false;
	bool countDown = true;
	if(timeout == 0) countDown = false;
	else timeout--;
	//wait until enemy responds or the user wants to quit (or timeout occurs)
	while( !(*flag) && ( nh->opponent != 0 ) && timeout >= 0)
	{
		UI_getInput(ui, 200);
		if(ui->visibleTable == MENU) { quit = true; break; }	//user wants to quit
		if(countDown) timeout -= 200;
	}
	if( timeout < 0 && !(*flag) ) return WAIT_TIMEOUT;
	*flag = false; //put down the flag
	return (quit || (nh->opponent == 0 ));	//return true if either the user or the enemy wants to quit
}


bool GL_startTurn(NH * nh, UI * ui, int turn, bool master)
{
	int quit = WAIT_TIMEOUT;
	sprintf(ui->message, "Odotetaan vastustajaa...\n");

	while(quit == WAIT_TIMEOUT) //resend every 2 seconds
	{
		if(master)
		{
			NH_sendStartTurn(nh, turn);
			quit = GL_waitForFlag(ui, nh, &nh->enemyOK, 1000 * 2); //TODO: the turn must be right!
			//printf("GL_startTurn (master): waiting for opponent...\n");
		}
		else
		{
			quit = GL_waitForFlag(ui, nh, &nh->enemyStartedTurn, 1000 * 2);
			if(quit == WAIT_OK) NH_sendBooleanResponse(nh, nh->currentOID, true);;
		}
		if(nh->resultsQueryReceived)
		{
			NH_sendSnmpPacketOCT(nh, nh->opponent, 161, SNMP_GET_RESP, SNMP_OCT, (void *)nh->enemyShotResults, 5, 4, 0);
			nh->resultsQueryReceived = false;
		}
	}
	sprintf(ui->message, "Vuoro alkaa.\n");
	//nh->currentOID++;
	return (quit == 1); //note: returns true if the game has ended due to premature quit
}

void GL_leaveGame(NH * nh, UI * ui, GameLogic * game)
{
	if(nh->opponent != 0)
	{
		//printf("Lopetan pelin, koska käyttäjä niin komensi.\n"); //DEBUG
		NH_sendTrap(nh, "I quit"); //user wants to quit
		nh->opponent = 0;
	}
	else
	{
		sprintf(ui->message, "Vastustaja lopetti pelin viestillä:\n\"%s\"\nEnterillä valikkoon.\n", nh->trapMessage);
		TUI_getEnter(ui);
		ui->visibleTable = MENU;
		TUI_printArea(ui);
	}
	GL_destructGame(game);
	return;
}


THREAD(mainFunc, arg)
{
	#ifdef __arm__
	u_long baud = 115200;

	#ifdef NUT_DEBUG
    NutRegisterDevice(&DEV_DEBUG, 0, 0);
	freopen(DEV_DEBUG_NAME, "r+", stdout);
	freopen(DEV_DEBUG_NAME, "r+", stdin);
	#else
	NutRegisterDevice(&DEV_UART, 0, 0);
	freopen(DEV_UART0_NAME, "r+", stdout);
	freopen(DEV_UART0_NAME, "r+", stdin);
	#endif

	_ioctl(_fileno(stdout), UART_SETSPEED, &baud);
	_ioctl(_fileno(stdin), UART_SETSPEED, &baud);

	#endif
	assignSinewave();
	//printf("BattleShip started\n");
	//fflush(stdout);

	//init the user interface
	UI * ui = UI_construct();


	//TUI_showMessage(ui, "Paina mitä tahansa näppäintä");
	printf("Paina mitä tahansa näppäintä\n");
	short command = UI_getInput(ui, 0);

	srand(NutGetTickCount() ); //init random seed

	if (ui->speechInput)
		Sound_init(ui, command == EXIT_BUTTON);

	printf("Sound initialized!\n");

	//init the netHandler
	NH * nh = NH_construct();
	Network_init(nh);
	ui->nh = nh;



	//main menu
	bool run = true;
	while(run)
	{
		UI_menuSelect(ui);
			 if(ui->menuCursor == MENU_SINGLE)   GL_singlePlayerGame(ui);
		else if(ui->menuCursor == MENU_MULTI) 	 GL_multiPlayerGame(ui, nh);
		else if(ui->menuCursor == MENU_SETTINGS) UI_settings(ui);
		else if(ui->menuCursor == MENU_MANUAL) 	 UI_manual(ui);
		else if(ui->menuCursor == MENU_QUIT) 	 run = false;
	}
	//destructor calls
	UI_closeInput(ui); //TODO: actually kill the threads? thread handling?
	NH_destruct(nh);
	//return 0;
}

int main(void)
{
	NutThreadCreate("MainThread", mainFunc, NULL, 16384);
	for(;;)
		NutThreadYield();
}

