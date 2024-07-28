FUTURE NOTES EXPLANATION


line: 	"uint8_t next_note = track[index]&0x0F;"
		
0x0F = 00001111 checks the "four least significant bits [of the track array] indicating if a note is present in [a] lane" (idea taken from Edstem post#680),
e.g. if this AND mask return "1" on the LSB, there is going to be a note in lane 0 due to the properties of bitwise AND (1 AND X = X, 0 AND X = 0, 1 indicates note is present in lane, 0 indicates note not present in lane), if it returns "1" in 2nd least significant bit, there is note in lane 1 etc.
		
		
The index for the future note is calculated from future = 15 and above (i.e. the rows equal to and above to top of the LED matrix). Thus,  track[index] as index increments upwards (due to future_column being incremented upwards) are tracking elements in "track" that represent notes that are off the LED screen (i.e. notes in the future) as the song progresses
		
Thus, AND masking track[index] with 0x0F = 00001111 checks the lower 4 bits of the track array element corresponding to the note in the track that is just off the LED screen, i.e. a future note.