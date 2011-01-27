/**
 * \file UI.h
 * A Documented file.
 */
#ifndef UI_H
#define UI_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "Network.h"
#include "NetHandler.h"

#ifdef __i386
#include <termios.h>
#include <pthread.h>
#endif

#ifdef __arm__
#include <sys/mutex.h>
#include <sys/thread.h>
#include <sys/timer.h>
#endif

#include "Player.h"
#include "Common.h"

#define ITOA_OFFSET 48
#define GHOST_SHIP 9
#define TIME_OUT 0
#define LOOP_WAIT 60
#define CP_SIZE 20


/**
 * An enum.
 * ASCII codes of used keys.
 */
enum buttons
{
	ARROW_FIRST = 27, /**< arrow keys send three values: ARROW_FIRST, ARROW_SECOND and ARROW_DIR*/
	ARROW_SECOND = 91,

	ARROW_UP = 65, 		//the same as uppercase A
	ARROW_DOWN = 66,
	ARROW_LEFT = 68,
	ARROW_RIGHT = 67,
	ENTER_BUTTON = 10,
	UNDO_BUTTON = 8,	//backspace
	STATS_BUTTON = 9,	//tab
	EXIT_BUTTON = 113,	//q
	YES_BUTTON = 'k',	//TODO: find the real numbers
	NO_BUTTON = 'e',
	CHAT_BUTTON = '§'
};


/**
 * Declarations of different views.
 */
enum views
{
	AWAY = 0,
	HOME = 1,
	MENU = 2,
	STATS = 3,
	OPPONENT_MENU = 4,
	CHAT = 5,
	QUERY = 6
	//more needed? SETTINGS, MANUAL , MENU_QUERY?
};


/**
 * Main menu members.
 */
enum menuItems
{
	MENU_SINGLE = 0,
	MENU_MULTI = 1,
	MENU_SETTINGS = 2,
	MENU_MANUAL = 3,
#ifdef __arm__
	MENU_QUIT = 3
#else
	MENU_QUIT = 4
#endif
};


/**
 * UI struct.
 * Contains the data that UI needs to communicate with the user.
 */
typedef struct
{
	Player * player;		/**< local player*/
	Player * enemy;			/**< AI player or the enemy-over-net*/

	Grids cursor;			/**< cursor location*/
	uint8_t cursorTable; 	/**< the table the cursor is on (home / away)*/
	uint8_t menuCursor;		/**< menu cursor location*/

	char * message;			/**< message to the user */
	uint8_t visibleTable;	/**< the table that is currently shown, @see enum views*/

	//command queue
	Command * frontCommand;	/**< the next command to be served (front member of the command queue)*/
	Command * backCommand;	/**< back member of the command queue */
	uint8_t   CQsize;		/**< number of commands in the command queue */

	Command * commandPool;	/**< a pool of empty commands */
	uint8_t CPindex;		/**< commandPool index */


	//input flags:
	bool terminalInput;		/**< flag: do we listen terminal input? */
	bool IOboardInput;		/**< flag: do we listen IO-board input? */
	bool speechInput;		/**< flag: do we listen voice commands? */

	NH * nh;				/**< pointer to the nethandler */

	MUTEX * CQmutex;		/**< command queue mutex */
} UI;


//---- constructor & destructor ----//

/**
* Initializes the user interface.
* @return a new UI
*/
UI *			UI_construct(void);

/**
* Deallocates allocated resources.
* @param ui the ui to be destructed
*/
void			UI_closeInput(UI * ui);

//---- user input querys ----

/**
* Enables the user to select a square by moving the cursor.
* @param ui an UI struct
* @return true if enter was pressed, false if undo
*/
bool 			UI_selectUsingCursor(UI * ui);

/**
* Enables the user to select a direction where the next ship will point at.
* @param ui an UI struct
* @param shipLength the length of the ship
* @param [out] direction the direction selected by the user
* @return true if the selected direction is valid
*/
bool 			UI_readDirection(UI * ui, int shipLength, int * direction);

/**
* Pops a member from the command queue. If queue is empty, waits until a command arrives,
* or the the maxWait time.
* @param ui an UI struct
* @param maxWait timeout time (in milliseconds)
* @return The command as unsigned char, or TIME_OUT
*/
short		 	UI_getInput(UI * ui, int maxWait);

void 			UI_CQpush(UI * ui, uint8_t command);

short			UI_CQpop(UI * ui);

/**
* Checks the newly popped command, and serves it if it is a common command.
* @param ui an UI struct
* @param command as uint8_t
* @return true if the command was a common command
* @note common commands are "go to menu", "switch to home/away view", "show stats", "type message", ...
*/
bool 			UI_commonAction(UI * ui, uint8_t command);


//--- drawing ----


/**
* Draws a ghost ship to the board.
* @param shipLength the length of the ship
* @param ui an UI struct
* @param dir the orientation of the ship
* @return true if the ship fits into the given location
*/
bool 			UI_drawGhost(int shipLength, UI * ui, int dir);

/**
* Erases a ghost ship.
* @param ui an UI struct
* @param dir the orientation of the ship
*/
void 			UI_eraseGhost(UI * ui, int dir);

/**
* Main menu look & feel.
*/
void			UI_menuSelect(UI * ui);

/**
* Settings menu.
* @param ui an UI struct
*/
void 			UI_settings(UI * ui);

/**
* Manual page.
* @param ui an UI struct
*/
void			UI_manual(UI * ui);

/**
* Opponent menu look & feel
* @param ui an UI struct
* @param opponentList an array of ip addresses
* @param numOpponents number of opponents available
* @param cursor pointer to the cursor of the opponentList
*/
uint8_t 		UI_opponentSelect(UI * ui, uint32_t* opponentList, int numOpponents, int * cursor);

/**
* Chat look & feel
* @param ui an UI struct
*/
void 			UI_chatView(UI * ui);




//---- TUI methods----




/**
* Clear screen.
*/
void 			TUI_clear(void);

/**
* Update screen.
* @param ui an UI struct.
*/
void 			TUI_printArea(UI * ui);

/**
* Print the main menu.
* @param ui the user interface struct
*/
void			TUI_printMenu(UI * ui);

/**
* Print the opponent menu
* @param ui an UI struct
* @param opponentList an array of ip addresses in char * format
* @param numOpponents number of opponents available
* @param cursor
*/
void 			TUI_printOpponentMenu(UI* ui, uint32_t* opponentList, int numOpponents, int cursor);

/**
* Print the chat screen
* @param ui the user interface struct
* @param message user written message
*/
void 			TUI_printChat(UI * ui);


/**
* Show stats during the game.
* @param ui the user interface struct
*/
void			TUI_showStats(UI * ui);

/**
* Get char.
* @return the key that was pressed
*/
int 			TUI_getch(void);

/**
* Event loop (terminal /UART).
* @param ui an UI struct.
*/
#ifdef __i386
void *TUI_listener(void * ui);
#endif

#ifdef __arm__
THREAD(TUI_listener, arg);
#endif


/**
* Shows the user a message
* @param ui an UI struct.
* @param message the message test
*/
void			TUI_showMessage(UI * ui, const char * message);

/**
* Ask the user a yes/no question
* @param ui an UI struct
* @param message
*/
bool 			TUI_booleanRequest(UI * ui, const char * message);


/**
* Shot animation
* @param ui an UI struct.
*/
void			TUI_shotAnimation(UI* ui);

/**
* Get enter.
* @param ui an UI struct.
*/
void 			TUI_getEnter(UI * ui);




#endif
