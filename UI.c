#include "UI.h"

const char * msgHeader = "sanot:\n";


//UI messages
const char * CHOOSE_DIRECTION = "Valitse suunta nuolinäppäimillä\n";
const char * INCORRECT_SHIP_POSITION = "Laiva ei sovi tähän.\nKäännä laivaa tai palaa koordinaattien valintaan.\n";
const char * SHIP_TOUCH = "Laiva ei saa koskettaa toista laivaa.\nKäännä laivaa tai palaa koordinaattien valintaan.\n";


UI * UI_construct(void)
{
	UI * ui = (UI*)malloc(sizeof(UI));
	ui->message = malloc(sizeof(uint8_t)*200);
	ui->frontCommand = NULL;
	ui->backCommand = NULL;
	ui->CQsize = 0;
	ui->cursorTable = HOME;
	ui->visibleTable = MENU;
	ui->menuCursor = MENU_SINGLE;
	ui->cursor.x = 0; ui->cursor.y = 0;

	ui->terminalInput = true; //TODO: these values should be optional for the user
	ui->speechInput = true;
	ui->IOboardInput = false;

	ui->CQmutex = (MUTEX*)malloc(sizeof(MUTEX));
	NutMutexInit(ui->CQmutex);

	ui->commandPool = malloc(CP_SIZE * sizeof(Command));
	ui->CPindex = 0;

	//launch the event loop
	#ifdef __i386
	pthread_t  terminalInput;
	int rc = pthread_create(&terminalInput, NULL, TUI_listener, (void *)(ui));

	if (rc)
	{
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        //exit(-1);
    }
	#endif

	#ifdef __arm__
	NutThreadCreate("EventLoop", TUI_listener, (void *)(ui), 1024);
	#endif


	return ui;
}

void UI_closeInput(UI * ui)
{
	ui->terminalInput = false;
	ui->IOboardInput = false;
	ui->speechInput = false;
	while(ui->frontCommand != NULL)
	{
		free(ui->frontCommand);
		ui->frontCommand = (Command*)ui->frontCommand->next;
	}
	free(ui->message);
	free(ui);
}


bool UI_selectUsingCursor(UI * ui)
{
	short command = 0;
	while(1 == 1)
	{
		TUI_printArea(ui);			//TODO: general updateView, if another screen needs to be updated
		command = UI_getInput(ui, 0);
		if(command == ARROW_UP) {
			if(ui->cursor.y > 0) ui->cursor.y --;
		}
		if(command == ARROW_DOWN) {
			if(ui->cursor.y < BOARD_SIZE - 1) ui->cursor.y ++;
		}
		if(command == ARROW_LEFT) {
			if(ui->cursor.x > 0) ui->cursor.x --;
		}
		if(command == ARROW_RIGHT) {
			if(ui->cursor.x < BOARD_SIZE - 1) ui->cursor.x ++;
		}
		if(command == ENTER_BUTTON) return true;
		if(command == UNDO_BUTTON || command == EXIT_BUTTON) return false;

		if ( command >= 3840 ) { // Command is type 1111 x-coordinant y-coordinant
			ui->cursor.x = ((command-3840)>>4);
			ui->cursor.y = (command-3840-(ui->cursor.x<<4));
			TUI_printArea(ui);
			return true;
		}
	}
}

void UI_eraseGhost(UI * ui, int dir)
{
	int i;
	for(i = 0; i < 5; i++ )
	{
		if( (i * dir + ui->cursor.x >= BOARD_SIZE)
		|| (i *(!dir) + ui->cursor.y >= BOARD_SIZE)
		|| (ui->player->area[ui->cursor.x + dir * i][ui->cursor.y + (!dir) * i].shipID != GHOST_SHIP) )
			break;
		ui->player->area[ui->cursor.x + dir * i][ui->cursor.y + (!dir) * i].shipID = EMPTY_SQUARE;
	}
}




bool UI_drawGhost(int shipLength, UI * ui, int dir)
{
	bool ok = true;
	int i;
	for(i = 0; i < shipLength; i++ )
	{
		if( (dir*i + ui->cursor.x >= BOARD_SIZE)	//out of board?
		|| (!dir*i + ui->cursor.y >= BOARD_SIZE)
		|| (i > 0 && ui->player->area[ui->cursor.x + dir*i][ui->cursor.y + !dir * i].shipID != EMPTY_SQUARE) )
		{
			ok = false;
			sprintf(ui->message, INCORRECT_SHIP_POSITION);
			break;
		}
		if(P_touchesAShip(ui->player, ui->cursor.x + dir*i, ui->cursor.y + !dir * i, 5*(i == 0) + dir) )
		{
			ok = false;
			sprintf(ui->message, SHIP_TOUCH);
		}
		ui->player->area[ui->cursor.x + dir * i][ui->cursor.y + !dir * i].shipID = GHOST_SHIP; //GHOST_SHIP is a joker
	}
	if(ok) sprintf(ui->message, CHOOSE_DIRECTION);
	return ok;
}



bool UI_readDirection(UI * ui, int shipLength, int * direction) //TODO: cursor "test"
{
	bool ok = false; bool start = true;
	char command;
	int dir = 1;
	while(1 == 1)
	{
		TUI_printArea(ui); //TODO: general printing function UI_print needed?
		if(start) command = ARROW_RIGHT;
		else command = UI_getInput(ui, 0);
		if(command == ARROW_UP || command == ARROW_RIGHT)
		{
			if(dir == 0 || start)
			{
				start = false;
				dir = 1;
				//set the ghost ship
				UI_eraseGhost(ui, !dir);
				ok = UI_drawGhost(shipLength, ui, dir);
			}
		}
		else if(command == ARROW_DOWN || command == ARROW_LEFT)
		{
			if(dir == 1)
			{
				dir = 0;
				UI_eraseGhost(ui, !dir);
				ok = UI_drawGhost(shipLength, ui, dir);
			}
		}
		else if(command == ENTER_BUTTON)
		{
			if(ok) //set the ship
			{
				*direction = dir;
				return true;
			}
		}
		else if(command == UNDO_BUTTON || command == EXIT_BUTTON)
		{
			UI_eraseGhost(ui, dir);
			return false;
		}
		//default:
	}
	if(!ok) UI_eraseGhost(ui, dir); //erase the ghost ship
	return false;
}


bool UI_commonAction(UI * ui, uint8_t command)
{
	if (command == CHAT_BUTTON && ui->visibleTable != STATS && ui->visibleTable != QUERY)
	{
		//if(ui->visibleTable != CHAT) UI_chatView(ui);
		return false;
	}
	if(ui->visibleTable == MENU || ui->visibleTable == OPPONENT_MENU || ui->visibleTable == QUERY || ui->visibleTable == CHAT)
		return false;

	if(command == ITOA_OFFSET + 1 || command == ITOA_OFFSET + 2) //user wants to switch between view (HOME / AWAY)
	{
		ui->visibleTable = !(command - (ITOA_OFFSET + 1));
		TUI_printArea(ui);
		return true;
	}
	else if(command == STATS_BUTTON)
	{
		if(ui->visibleTable == STATS)
		{
			ui->visibleTable = ui->cursorTable;
			TUI_printArea(ui);	//TODO: test if this is not needed
		}
		else TUI_showStats(ui);
		return true;
	}
	/*else if (command == CHAT_BUTTON && ui->visibleTable != STATS)
		UI_chatView(ui);*/
	else if( command == EXIT_BUTTON )
	{
		if( TUI_booleanRequest(ui, "Haluatko todella lopettaa pelin ja palata valikkoon (k/e)?\n") )
			ui->visibleTable = MENU;
		else TUI_printArea(ui);
	}
	return false;
}


short UI_getInput(UI * ui, int maxWait) //"CQpop"
{
	bool timeCheck = (maxWait != 0);
	short result;
	bool commonAction;
	do
	{
		while(ui->frontCommand == NULL) {
			#ifndef __arm__
			usleep(70000);
			#else
			NutSleep(LOOP_WAIT);
			#endif
			if(timeCheck) maxWait -= LOOP_WAIT;
			if(maxWait < 0) return TIME_OUT;
		}
		result = UI_CQpop(ui);

		commonAction = UI_commonAction(ui, result);
	} while(commonAction);
	return result;
}

void UI_CQpush(UI * ui, uint8_t cmd)
{
	if(ui->CQsize == CP_SIZE) return; //command queue is full

	NutMutexLock(ui->CQmutex);

	Command * command = &ui->commandPool[ui->CPindex]; //take a member from the pool
	command->next = NULL;
	command->cmd = cmd;

	ui->CPindex++;	//update the pool pointer
	if(ui->CPindex == CP_SIZE) ui->CPindex = 0;

	ui->CQsize++;	//update the size of the queue

	if(ui->frontCommand == NULL)		//the queue is initially empty
	{
		ui->frontCommand = command;
		ui->backCommand = command;
	}
	else
	{
		ui->backCommand->next = command; //add a command to the end of the queue
		ui->backCommand = command;
	}

	NutMutexUnlock(ui->CQmutex);
}

short UI_CQpop(UI * ui)
{
	if(ui->CQsize == 0) return 99; //error (should ever occur, though)
	NutMutexLock (ui->CQmutex);
	Command * tmp = ui->frontCommand;
	ui->frontCommand = tmp->next;
	//free(tmp);
	ui->CQsize--;
	if(ui->frontCommand == NULL)
		ui->backCommand = NULL;

	NutMutexUnlock(ui->CQmutex);
	return tmp->cmd;
}

void UI_menuSelect(UI * ui)
{
	bool selected = false;
	short command;
	while(!selected)
	{
		TUI_printMenu(ui);
		command = UI_getInput(ui, 0);
		if(command == ARROW_UP && ui->menuCursor > MENU_SINGLE)
			ui->menuCursor--;
		else if(command == ARROW_DOWN && ui->menuCursor < MENU_QUIT)
			ui->menuCursor++;
		else if(command == ENTER_BUTTON) selected = true;
	}
}


uint8_t UI_opponentSelect(UI * ui, uint32_t* opponentList, int numOpponents, int * cursor)
{
	uint8_t command;
	bool selected = false;
	while(!selected)
	{
		command = UI_getInput(ui, 300);
		if(command == TIME_OUT) return TIME_OUT;
		sprintf(ui->message, " "); //clear the message
		if(command == ARROW_UP && *cursor > -2)
			(*cursor)--;
		else if(command == ARROW_DOWN && *cursor < numOpponents - 1)
			(*cursor)++;
		else if(command == ENTER_BUTTON) selected = true;
		TUI_printOpponentMenu(ui, opponentList, numOpponents, *cursor);
	}
	return 1;
}
/*
void UI_chatView(UI * ui)
{
	int previousTable = ui->visibleTable;
	ui->visibleTable = CHAT;
	uint8_t* msg = ui->nh->chatMsgs[ui->nh->emptyMsg];

	TUI_printChat(ui);

	uint8_t ch = UI_getInput(ui, 0);
	int ptr = 0;
	msg[ptr] = '\0';
	while(ch != CHAT_BUTTON)
	{
		if(ch == ENTER_BUTTON)	//user wants to send the message
		{
			if (ptr > 0)
			{
				msg[ptr] = '\0';							//end the message
				NH_sendChatMessage(ui->nh);					//send it
				msg = ui->nh->chatMsgs[ui->nh->emptyMsg];	//load new message
				msg[0] = '\0';

				ptr = 0;
				TUI_printChat(ui);							//update screen
				ch = UI_getInput(ui, 0);					//read next char
				continue;
			}
			else {
				ch = UI_getInput(ui, 0);
				continue;
			}


		}
		else if(ch == UNDO_BUTTON && ptr > 0) 			//backspace - user wants to erase the previous char
		{
			ptr--;
			msg[ptr] = '\0';
			TUI_printChat(ui);
			ch = UI_getInput(ui, 0);
			continue;
		}
		else if(ptr < MAX_MSG_LENGTH - 2)
		{
			//user input is printed
			printf("%c", ch);
			msg[ptr] = ch;
			ptr++;
			//msg[ptr] = '\0';
			ch = UI_getInput(ui, 0);
		}
	}

	if(ch == CHAT_BUTTON) msg[0] = '\0';

	ui->visibleTable = previousTable;
	if(ui->visibleTable < 2) TUI_printArea(ui);
	return;
}*/

void 			UI_settings(UI * ui)
{
	//TODO
}

void 			UI_manual(UI * ui)
{
	//TODO
}





//-----------------------------terminal UI methods------------------------------------------//






int TUI_getch(void) {
	#ifdef __i386
	struct termios oldt,
	               newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	return ch;
	#endif

	#ifdef __arm__
	int ch;
	ch = fgetc(stdin);
	return ch;
	#endif
}

void TUI_clear(void) //TODO: arm implementation?
{
	printf("\x1b[2J\x1b[1;1H");
    fflush(stdout);
}

void TUI_showMessage(UI * ui, const char * message)
{
	TUI_clear();
	printf("%s\n", message);
	UI_getInput(ui, 0);
}

void TUI_shotAnimation(UI * ui)
{
	sprintf(ui->message, "LAUKAUS!\n");
	TUI_printArea(ui);
	NutSleep(300); //0.4s
}


void TUI_printArea(UI * ui)
{
	NutThreadYield();
	if(ui->visibleTable > HOME) return;
	TUI_clear();
	char id, cursorMark;
	cursorMark = ' ';
	int row, col;
	Player * player = ui->player;
	if(!ui->visibleTable)
	{
		player = ui->enemy;
		printf("1|2|");
	}
	else printf("|1|2");
	for(row = -1; row < BOARD_SIZE; row ++) {
		for(col = -1; col < BOARD_SIZE; col++) {
			cursorMark = ' ';
			if(col == ui->cursor.x && row == ui->cursor.y && ui->cursorTable == ui->visibleTable)
				cursorMark = '|';
			if(row == -1 && col == -1) continue; //upper corner is handled earlier
			else if(row == -1) printf(" %c ", col + ARROW_UP);
			else if(col == -1) printf("  %d ", row );

			else
			{
				id = player->area[col][row].hit;
				if(id == NO_HIT && ui->visibleTable) 	//home view
				{
					if(player->area[col][row].shipID != EMPTY_SQUARE)
					{
						if(player->area[col][row].shipID == GHOST_SHIP) id = '?';
						else id = ITOA_OFFSET + C_shipLength(player->area[col][row].shipID);
					}
				}
				printf("%c%c%c", cursorMark, id, cursorMark);
			}
		}
		printf("\n\n");
	}
	printf("%s", ui->message);
	fflush(stdout);
}


bool TUI_booleanRequest(UI * ui, const char * message)
{
	char ans = 0;
	TUI_clear();
	int table = ui->visibleTable; ui->visibleTable = QUERY;
	bool response = false;
	printf("%s", message);
	while(ans != YES_BUTTON && ans != NO_BUTTON)
		ans = UI_getInput(ui, 0);
	if(ans == YES_BUTTON)
		response = true;
	ui->visibleTable = table;
	return response;
}

void TUI_printMenu(UI * ui)
{
	TUI_clear();
	//printf("    VALIKKO\n\n\n");
	printf("IP: %s\n\n\n", inet_ntoa(confnet.cdn_ip_addr));
	int i;
	for(i = MENU_SINGLE; i <= MENU_QUIT; i++)
	{
		if(i == ui->menuCursor) printf(" -> ");
		else printf("    ");
		switch(i)
		{
			case MENU_SINGLE: 	printf("yksinpeli\n"); break;
			case MENU_MULTI: 	printf("kaksinpeli\n"); break;
			case MENU_SETTINGS:	printf("asetukset\n"); break;
			case MENU_MANUAL:	printf("ohjeet\n"); break;
			#ifndef __arm__
			case MENU_QUIT:		printf("lopeta\n");
			#endif
		}
	}
}

void TUI_printOpponentMenu(UI* ui, uint32_t* opponentList, int numOpponents, int cursor)
{
	TUI_clear();
	printf("    ETEISHUONE (%d vastustajaa)\n\n\n", numOpponents);
	int i;
	for(i = -2; i < numOpponents; i++)
	{
		if(i == cursor) printf(" -> ");
		else printf("    ");
		if(i == -2) printf("Takaisin päävalikkoon\n");
		else if(i== -1) printf("Päivitä luettelo\n");
		else printf("%s\n", inet_ntoa(opponentList[i]));
	}
	printf("\n\n%s\n", ui->message);
	fflush(stdout);
}


void TUI_showStats(UI * ui) //TODO: in multiplayer game, we dont know the enemies ships, but we do know which ships has been sunk.
{
	TUI_clear();
	ui->visibleTable = STATS;
	printf("Omat laivat: %d kelluvaa\n\n", ui->player->numShips);
	int i;
	for(i = 0; i < 5; i++)
		printf("Laiva %d: %d/%d\n", i + 1, ui->player->shipHPs[i], C_shipLength(i));

	printf("Yhteensä: %d/17", ui->player->totalHPs);
	printf("\n\nVastustajan laivat: %d kelluvaa\n", ui->enemy->numShips);
	printf("Osumapisteitä jäljellä: %d/17\n", ui->enemy->totalHPs);

	//TODO: at which point is the visible table turned back?
}

void TUI_printChat(UI * ui)
{
	TUI_clear();
	printf("CHAT (%d viestiä, empty msg: %d)\n\n", ui->nh->numMsgs, ui->nh->emptyMsg);
	fflush(stdout);

	int i, k = ui->nh->emptyMsg;
	for (i = 0; i < MAX_MESSAGES; i++) {
		k++;
		if (k >= MAX_MESSAGES)
			k = 0;
		if(ui->nh->chatMsgs[k][0] != '\0')
		{
			if(i < MAX_MESSAGES - 1)
				printf("%s\n\n", ui->nh->chatMsgs[k]); //print message
			else printf("%s", ui->nh->chatMsgs[k]);
		}

	}

	/*
	k = 0;
	l = ui->nh->numMsgs;
	if(ui->nh->numMsgs == MAX_MESSAGES) //consider rotation
	{
		k = ui->nh->emptyMsg + 1;
		if(k == MAX_MESSAGES) k = 0;
		l = MAX_MESSAGES;
	}
	for(i = 0; i < l; i++)
	{
		printf("%d: %s\n\n", k, ui->nh->chatMsgs[k]); //print message
		k++;
		if(k == MAX_MESSAGES) k = 0;
	}
	printf("%s", ui->nh->chatMsgs[ui->nh->emptyMsg]);
	*/
}


void TUI_getEnter(UI * ui)
{
	TUI_printArea(ui);
	uint8_t enter = 0;
	while(enter != ENTER_BUTTON && ui->visibleTable != MENU)
		enter = UI_getInput(ui, 0);
}


#ifdef __i386
void *TUI_listener(void * ui)
#endif

#ifdef __arm__
THREAD(TUI_listener, ui)
#endif
{
	uint8_t cmd;
	UI * tui = (UI*)ui;
	while(tui->terminalInput)
	{
		//wait input from user
		cmd = TUI_getch();
		if(cmd == ARROW_FIRST && TUI_getch() == ARROW_SECOND )
			cmd = TUI_getch();
		//add a member to the end of the queue (push)
		UI_CQpush(tui, cmd);
	}
	#ifdef __i386
	pthread_exit(NULL);
	return NULL; //does nothing but keeps the compiler happy (no warings)
	#endif
}
