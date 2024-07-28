/*
 * game.c
 *
 * Functionality related to the game state and features.
 *
 * Author: Jarrod Bennett, Cody Burnett
 */ 

#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "display.h"
#include "ledmatrix.h"
#include "terminalio.h"
#include "buttons.h"



static const uint8_t track[TRACK_LENGTH] = {0x00,
	0x00, 0x00, 0x08, 0x08, 0x08, 0x80, 0x04, 0x02,
	0x04, 0x40, 0x08, 0x80, 0x00, 0x00, 0x04, 0x02,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x02, 0x20, 0x01,
	0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x80, 0x04, 0x40, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x40, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x02, 0x20, 0x01,
	0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x08, 0x08, 0x08, 0x80, 0x04, 0x02,
	0x04, 0x40, 0x02, 0x08, 0x80, 0x00, 0x02, 0x01,
	0x04, 0x40, 0x08, 0x80, 0x04, 0x02, 0x20, 0x01,
	0x10, 0x10, 0x12, 0x20, 0x00, 0x00, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x40, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x40, 0x02, 0x20,
	0x04, 0x40, 0x08, 0x04, 0x40, 0x40, 0x02, 0x20,
	0x01, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00};

//create a variable to check whether the note is green or not
uint8_t note_isnt_green = 1;

uint16_t beat;
//define the scoring region columns:
uint8_t scoring_row_1 = 0, scoring_row_2 = 1, scoring_row_3 = 2, scoring_row_4 = 3, scoring_row_5 = 4;

//define the index of the note (index = which exact note is currently being played) and
//set it to some invalid value:
uint8_t global_note_index = -1; //This value represents which beat (or note) is in the scoring area


// Initialize the game by resetting the grid and beat
void initialise_game(void)
{
	// initialize the display we are using.
	default_grid(); //located in display.c
	beat = 0;
}


//create update_game_score() function to be called in play_note() - prints out statement with 
//score
//in play_score() function - check which column the note is in - call update_game_score() function 
//and update the number, note the print statement

int update_game_score(uint8_t scoring_column_row)
{

	
			//define what number to add to the score based on future
			if ((scoring_column_row == 0)||(scoring_column_row==5))
			{
				global_score_variable++;
			}
			
			else if ((scoring_column_row == 1)||(scoring_column_row == 4))
			{
				global_score_variable = global_score_variable + 2;
			}
			
			else if (scoring_column_row == 3)
			{
				global_score_variable = global_score_variable + 3;
			}
			
			else 
			{
				global_score_variable = global_score_variable - 1;
			}
			
		
			
	return global_score_variable;
	
}
	



// Play a note in the given lane
void play_note(uint8_t lane){
	
	//create a flag to check whether the note isn't in the scoring area
	//initial flag state = 1, note hasn't arrived in scoring area
	uint8_t note_is_not_in_scoring_area = 1;
	
	
	//note: don't increment the beat in this function. This is because the beat will be changed by this increment every time the play_note() function is called
	//in project.c, which will mess up the coloring of the notes (e.g. every time a button is pressed, the LED color will be changed to the color of all active
	//notes on the screen)
	
	//create an array to store the indexes of notes in the 4 lanes if they are in the scoring area
	uint16_t note_indices_in_scoring_area[4] = {};
	
	
	

		
	//loop between the first and max note columns to draw the new notes:	
	for(uint8_t current_note_column = 0; current_note_column < MATRIX_NUM_COLUMNS; current_note_column++)
	{
		
	//calculate the value of the future column to so the index of the note can be calculated later in this function and so
	//the column of the note can be kept track of to see if it has entered the scoring region
	uint8_t future_column = MATRIX_NUM_COLUMNS - 1 - current_note_column; 
	
	//only generate notes every 5 columns
	if ((future_column+beat)%5)
	{
		continue;
	}


	
	//calculate the index position of the note using future_column and the beat
	uint8_t index_position = (future_column+beat)/5;
	
	//doesn't allow note generation if the note indexes exceed the track length
	if (index_position >= TRACK_LENGTH)
	{
		continue;
	}

	//check to see if the column of the note is in between the scoring columns on the LED (i.e. columns 0-5)
	if ((future_column>=scoring_row_1)&&(future_column<=scoring_row_5)){
		
	//if it is, store the index of the note that was just calculated as the "lane"'th index in the array
	//that stores the indices of the notes when they are in the scoring columns
	note_indices_in_scoring_area[lane]= index_position;

	
	}


	//checks to see if there is a note along the specific lane inputted into the function:
	if ((track[index_position])&(1<<lane))
	{
		
		
	//if there is, check in the lane'th index of the array to see if the note index of the note that lane is 
	//the same as the scoring column index, indicating that the note along "lane" is in the scoring section
	if (index_position==note_indices_in_scoring_area[lane])
	{
		//if so, store the index value of the note to be used in advance_note() and immediately set the note color to green:
		global_note_index = index_position;
		ledmatrix_update_pixel(current_note_column, 2*lane, COLOUR_GREEN);
		ledmatrix_update_pixel(current_note_column, 2*lane+1, COLOUR_GREEN);
		note_isnt_green = 0;
	
	}
	
	
	
	//scoring_row_1 = 0 (bottom row), scoring_row_2 = 1, scoring_row_3 = 2, scoring_row_4 = 3, 
	//scoring_row_5 = 4 (top scoring row)



if ((future_column == scoring_row_1)||(future_column==scoring_row_5))
{
	//the note is in the scoring area, so update the not in scoring area flag to 0
	note_is_not_in_scoring_area = 0;
	
	
	//updates the score to +1
	printf("Game score: %d\n",  update_game_score(0));
	
	
	
}


else if ((future_column == scoring_row_2)||(future_column==scoring_row_4))
{
	note_is_not_in_scoring_area  = 0;
	
	
	//updates the score to +2
	printf("Game score: %d\n",  update_game_score(1));
	

	

}

else if ((future_column == scoring_row_3))
{
	note_is_not_in_scoring_area = 0;
	

//updates the score to +3
printf("Game score: %d\n", update_game_score(3));

}




	
	//if the future column of the note is less than 0 (i.e. the note has exited the LED board), clear the note index
	if (future_column == 0)
	{
		global_note_index = 0;
		
	}



//closes lane check if statement
}


//closes for loop
}


//if the note isn't in the scoring area and the play_note() function has being called, deduct a point
if ((note_is_not_in_scoring_area))
{
	printf("Game score: %d\n", update_game_score(7));
	
}














//closes function
}
	
// Advance the notes one row down the display
void advance_note(void)
{
	

		
	// remove all the current notes; reverse of below
	for (uint8_t col=0; col<MATRIX_NUM_COLUMNS; col++)
	{
		uint8_t future = MATRIX_NUM_COLUMNS - 1 - col; //defines row number (vertical location) of notes
		uint8_t index = (future + beat) / 5;  
		if (index >= TRACK_LENGTH)
		{
			break;
		}
		if ((future+beat) % 5)
		{
			continue;
		}
		for (uint8_t lane = 0; lane < 4; lane++)
		{
			if (track[index] & (1<<lane))
			{
				PixelColour colour;
				// yellows in the scoring area
				if (col==11 || col == 15)
				{
					colour = COLOUR_QUART_YELLOW;
				}
				else if (col==12 || col == 14)
				{
					colour = COLOUR_HALF_YELLOW;
				}
				else if (col==13)
				{
					colour = COLOUR_YELLOW;
				}
				else
				{
					colour = COLOUR_BLACK;
				}
				ledmatrix_update_pixel(col, 2*lane, colour);
				ledmatrix_update_pixel(col, 2*lane+1, colour);
			}
		}
	}
	
	// increment the beat
	beat++;
	
	//FUTURE NOTES SECTION
	
	
	uint8_t future_column = 15; //set the future column to 15 (top of LED)

	
	
	//loop between all the columns of the LED
	for (uint8_t col = 0; col < MATRIX_NUM_COLUMNS; col++)
	{
		
		
		//notes in this game are generated every 5 columns 
		//(i.e. 1st note slides down 5 columns on LED-then 2nd note appears on LED etc.)
		if ((future_column+beat)%5)
		{
			
			//increment future_column = 15 upwards by 1 every time a new note is generated 
			future_column = future_column + 1;
			continue;
		}
		
		//generate the index of the note every 5 columns from beat and future = 15 (set to the top column of the LED)
		uint8_t index = (future_column + beat)/5;

		
		//AND mask the index'th note in the track array with 0x0F = 00001111 to check if there are any future notes in specific lanes 
		uint8_t next_note = track[index]&0x0F;
		
		//check readme.txt for my logic for this step. 
		
		
			
			//if there is a next/future note (i.e. next_note != 0 = not false)
			if(next_note)		
			{
				//loop over the 4 lanes	
						for(uint8_t lane = 0; lane<4; lane++)
						{
				//check to see for next note none-zero note in any of the 4 lanes
							
						if (next_note&(1<<lane))
						{
							//if there is, turn top of column dark red
							
								ledmatrix_update_pixel(0, 2*lane, COLOUR_DARK_RED);
								ledmatrix_update_pixel(0, 2*lane+1, COLOUR_DARK_RED);
								
							
						}
							
						}
						//the note has being found, so break out of the for loop
						break;
					}
					}
					
					
		
	// draw the new notes
	for (uint8_t col=0; col<MATRIX_NUM_COLUMNS; col++)
	{
		// col counts from one end, future from the other
		uint8_t future = MATRIX_NUM_COLUMNS-1-col;
		// notes are only drawn every five columns
		if ((future+beat)%5)
		{
			continue;
		}
		
		// index of which note in the track to play
		uint8_t index = (future+beat)/5;
		
		// if the index is beyond the end of the track,
		// no note can be drawn
		if (index >= TRACK_LENGTH)
		{
			continue;
		}
		
		
		
		
		// iterate over the four paths/lanes that the notes might be taking
				for (uint8_t lane=0; lane<4; lane++)
				{
					// check if there's a note in the specific path
					if ((track[index]) & (1<<lane))
					{
					
					PixelColour note_color;
					
						//if there's a note in the lane (i.e. (track[index]) & (1<<lane) is not 0), check whether
						//the index of the note matches the index of the note in the scoring area
						if(index == global_note_index){
							
							//if there is, change color of the note to green
						note_color = COLOUR_GREEN;
						note_isnt_green = 0;
						
						if (future <= 0)
						{
							note_isnt_green = 1;
						}
						}
						
						
						
					else {
						//otherwise, keep the note color red
						note_color = COLOUR_RED;
						
						
						if ((future <= scoring_row_1)&&(index != global_note_index))
						{
							printf("Game score: %d\n", update_game_score(7));
							note_isnt_green = 1;
							
						}
						
						
					}
					
				
								
				//if there is a along the specific lane note, color it the appropriate color
				ledmatrix_update_pixel(col, 2*lane, note_color);
				ledmatrix_update_pixel(col, 2*lane+1, note_color);		
				
				
		
				
				
}					

}
}

 }


uint8_t is_game_over(void)
{
	// YOUR CODE HERE
	// Detect if the game is over i.e. if a player has won.
	//TRACK-LENGTH = 129
	
	
	//check if the track length multiplied by 5 is less than or equal to the beat - checks if the end of the song has reached 
	//note - song loops over twice - so must wait twice before game over screen shows up

	
	if ((TRACK_LENGTH*5)<=beat)
	{
		//if the game ends, set the global game over variable to True and reset the score:
		global_game_over_variable = 1;
		global_score_variable = 0;
	}

	


	
	return 0;
}