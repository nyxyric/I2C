#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "core.h"

void setupNewGame(Board board)
{
	// use time(NULL) to generate a random number 
	srand(time(NULL));

	for (unsigned int x = 0; x < SIZE; x++)
	{
		for (unsigned int y = 0; y < SIZE; y++)
		{
			// set all tile numbers to zero
			board[x][y] = 0;
		}
	}
	// add one random tile at the begining of the game
	addRandomTile(board);
}

bool checkGameOver(Board board)
{
    for (unsigned int x = 0; x < SIZE - 1; x++)
    {
        for (unsigned int y = 0; y < SIZE - 1; y++)
        {
            if (  board[x][y] == board[x][y + 1] ||
                  board[x][y] == board[x + 1][y] ||
                  board[x][y] == 0)
            {return 0;}
        }
        if( board[x][SIZE - 1] == board[x + 1][SIZE - 1] || 
            board[x][SIZE - 1] == 0) return 0;
        if( board[SIZE - 1][x] == board[SIZE - 1][x + 1] ||
            board[SIZE - 1][x] == 0) return 0;
    }
    return 1;
}

void addRandomTile(Board board)
{
	// count of tiles = 16
	unsigned int count[SIZE * SIZE];
	unsigned int len = 0;

	for (unsigned int x = 0; x < SIZE; x++)
	{
		for (unsigned int y = 0; y < SIZE; y++)
		{
			// add random tile just if current value is zero
			if (board[x][y] == 0)
			{
				count[len] = x * SIZE + y;
				len++;
			}
		}
	}	
	// create a random nummber between 0 and the number of tiles with value 0
	unsigned int index = rand() % len;

	// set random tile value to 1
	// get ranom tile by dividing integer with size (Integers are always rounded towards zero)
	board[count[index] / SIZE][count[index] % SIZE] = 1;
}

// calculates the current score of the game session
int calculateScore(Board board)
{
    int score = 0;
    for (unsigned int x = 0; x < SIZE; x++)
    {
        for (unsigned int y = 0; y < SIZE; y++)
        {
            if(board[x][y] != 0)
            {
            	score += pow(BASE, board[x][y]);
            }
        }
    }
    return score;
}

// merge the tiles, if they have the same number
bool mergeY(Board board, bool opp)
{
	bool merged = false;
	int start = 0;
	int end = SIZE - 1;
	int increment = 1;

	// enter if input is: down
	if (opp)
	{
		start = SIZE - 1;
		end = 0;
		increment = -1;
	}
	for (int y = 0; y < SIZE; y++)
	{
		// index defines if more than two tiles have to be merged
		int index = start;
		for (int x = start; x != end; x += increment)
		{
			// merge if tile value is not 0
			if (board[x][y] != 0)
			{
				// just merge if the value of the two tiles is the same
				if (board[x][y] == board[x + increment][y])
				{
					// add 1 to the tile value
					board[index][y] = board[x][y] + 1;

					// set the value of the old coordinates to zero
					board[x + increment][y] = 0;
					if (index != x)
						board[x][y] = 0;
					index += increment;
					merged = true;
				}
				else
				{
					// just move without merging
					board[index][y] = board[x][y];

					// if tiles were moved, their old coordinates value have to be set to zero
					if (index != x)
						board[x][y] = 0;
					index += increment;
				}
			}
		}

		if (board[end][y] != 0)
		{
			board[index][y] = board[end][y];
			if (index != end)
				// set the value of the old coordinates to zero
				board[end][y] = 0;
		}
	}
	return merged;
}


// just move the tiles before trying to merge them
bool shiftY(Board board, bool opp)
{
	bool moved = false;
	int start = 0;
	int end = SIZE;
	int increment = 1;

	// enter if input is: down
	if (opp)
	{
		start = SIZE - 1;
		end = -1;
		increment = -1;
	}
	for (int y = 0; y < SIZE; y++)
	{
		int index = start;
		for (int x = start; x != end; x += increment)
		{
			// shift if tile value is not 0
			if (board[x][y] != 0)
			{
				// shift tile in y-direction up or down, so it is connected to the window border, or the other tiles
				board[index][y] = board[x][y];
				if (index != x)
				{
					// set the value of the old coordinates to zero
					board[x][y] = 0;
					moved = true;
				}
				index += increment;
			}
		}
	}
	return moved;
}

// function for up and down movements
inline void moveY(Board board, bool opp)
{
	// first shift and after that merge the tiles if possible
	bool a = shiftY(board, opp);
	bool b = mergeY(board, opp);
	
	// add random tile after user input, but just if it was possible to move and/or merge a tile
	if (a || b)
		addRandomTile(board);
}

bool shiftX(Board board, bool opp)
{
	bool moved = false;
    int start = 0;
	int end = SIZE;
	int increment = 1;

	// enter if input is: down
    if (opp)
    {
        start = SIZE - 1;
        end = - 1;
        increment = - 1;
    }
    for (int x = 0; x < SIZE; x++)
    {
        int index = start;
        for(int y = start; y != end; y += increment)
        {
            if (board[x][y] != 0)
            {
                board[x][index] = board[x][y];
                if(index != y) {
					// set the value of the old coordinates to zero
                    board[x][y] = 0;
                    moved = 1;
                }
                index += increment;
            }
        }
    }
    return moved;
}

bool mergeX(Board board, bool opp)
{
	bool merged = false;
    int start = 0;
	int end = SIZE - 1;
	int increment = 1;

	// enter if input is: down
    if (opp)
    {
        start = SIZE - 1;
        end = 0;
        increment = -1;
    }
    for (int x = 0; x < SIZE; x++)
    {
		// index defines if more than two tiles have to be merged
        int index = start;
        for(int y = start; y != end; y += increment)
        {
			// merge if tile value is not 0
            if(board[x][y] != 0)
            {
				// just merge if the value of the two tiles is the same
                if(board[x][y] == board[x][y + increment])
                {
					// add 1 to the tile value
                    board[x][index] = board[x][y] + 1;

					// set the value of the old coordinates to zero
                    board[x][y + increment] = 0;
                    if(index != y) 
						board[x][y] = 0;
                    merged = 1;
                    index += increment;
                }
                else
                {
					// just move without merging
                    board[x][index] = board[x][y];

					// if tiles were moved, their old coordinates value have to be set to zero
                    if(index != y) 
						board[x][y] = 0;
                    index += increment;
                }
            }
        }
        
        if(board[x][end] != 0)
        {
            board[x][index] = board[x][end];
            if(index != end)
				// set the value of the old coordinates to zero 
				board[x][end] = 0;
        }
    }
    return merged;
}

inline void moveX(Board board, bool opp)
{
	// first shift and after that merge the tiles if possible
	bool a = shiftX(board, opp);
	bool b = mergeX(board, opp);

	// add random tile after user input, but just if it was possible to move and/or merge a tile
    if( a || b)
		addRandomTile(board);
}
