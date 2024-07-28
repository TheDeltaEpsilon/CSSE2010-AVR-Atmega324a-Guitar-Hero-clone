/*
 * game.h
 *
 * Author: Jarrod Bennett, Cody Burnett
 *
 * Function prototypes for game functions available externally. You may wish
 * to add extra function prototypes here to make other functions available to
 * other files.
 */


#ifndef GAME_H_
#define GAME_H_

#include <stdint.h>

#define TRACK_LENGTH 129

//define a global score variable to be accessed across all files
int global_score_variable;

uint8_t global_game_over_variable;

// Initialise the game by resetting the grid and beat
void initialise_game(void);

// Advance the notes one row down the display
void advance_note(void);

// Returns 1 if the game is over, 0 otherwise.
uint8_t is_game_over(void);

#endif
