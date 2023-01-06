#include "core.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

bool initializeSDL();
void cleanUp();
void drawTileText(const char *text, SDL_Rect rect);
void clearScreen();
void drawBoard(const Board board);
void drawHighScore();
void renderGame(Board board);
void gameLoop(Board board);
void handleMove(SDL_Event e, Board board);
