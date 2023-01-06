#include <stdio.h>
#include <stdbool.h>

#define SIZE 4
#define BASE 2

typedef unsigned char Board[][SIZE];
void setupNewGame(Board board);
bool checkGameOver(Board board);
void addRandomTile(Board board);
int calculateScore(Board board);
bool shiftY(Board board, bool opp);
bool mergeY(Board board, bool opp);
void moveY(Board board, bool opp);
bool shiftX(Board board, bool opp);
bool mergeX(Board board, bool opp);
void moveX(Board board, bool opp);

