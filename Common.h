/**
 * \file Common.h
 * A Documented file.
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <stdint.h>
#define min(X, Y)  ((X) < (Y) ? (X) : (Y))

#define BOARD_SIZE 10
#define EMPTY_SQUARE -1

#define TOLERANCE 2
#define MAX_OPPONENTS 20

void setLedOff(int led);
void setLedOn(int led);
#define _LED_1  0
#define _LED_2  1
#define _LED_3  2
#define _LED_4  22

#define lan

#ifdef lan

#define BCAST "10.10.255.255"
#define NMASK "255.255.0.0"

#else

#define BCAST "10.10.7.255"
#define NMASK "255.255.255.0"

#endif

//typedef unsigned char u_char;
short * Sinewave;
short * Arccos;
void assignSinewave(void);
/**
 * List of different forms of shots.
 * Note that the values are ASCII character codes that are used by the terminal UI to demonstrate hits and misses.
 */
enum shots
{
	NO_HIT = 46, 		//char "."
	HIT = 120,			//char "x"
	MISS = 111,			//char "o"
	GHOST_SHOT = 63,	//char "?"
	SUNK = 35			//char "#"
};

/**
 * Coordinates.
 */
typedef struct
{
	int x;
	int y;
} Grids;

/**
 * Command queue.
 */
typedef struct
{
	short cmd;
	void * next;
} Command;


int sqrt_int(int x);
int logTwo(int n);
short cosine(int rad);
short arcuscos(short div);
int absolute(int real, int imag);


/**
 * \fn C_shipLength
 * Determines the length of a ship, judging by the ship index.
 * 0->5, 1->4, 2->3, 3->3, 4->2.
 * @param shipIndex
 * @return ship length
 */
int C_shipLength(int shipIndex);


/**
   * Determines if the given coordinates are within the game board.
   * @param x
   * @param y
   * @return true if the given coordinates area out of the board.
   */
bool C_outOfBoard(int x, int y);


#endif
