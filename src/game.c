#include "game.h"
#include <stdlib.h>

// set window dimesions
#define WINDOW_WIDTH 500 // SCREEN_WIDTH
#define WINDOW_HEIGHT 600 // SCREEN_HEIGHT
#define WINDOW_PAD 10

// font size
#define CELL_FONT_SIZE 40

struct COLOR
{
    char r;
    char g;
    char b;
    char a;
};

struct App {
	SDL_Renderer *renderer;
	SDL_Window *window;
} app;

// application background color
struct COLOR bg = {41, 50, 65, 1};

// dark text color
struct COLOR fg = {80, 80, 80, 255};

// colors used for tiles
struct COLOR colors[] = {
    {230, 227, 232, 255},
    {169, 214, 229, 255},
    {137, 194, 217, 255},
    {97, 165, 194, 255},
    {70, 143, 175, 255},
    {44, 125, 160, 255},
    {42, 111, 151, 255},
    {1, 79, 134, 255},
    {1, 73, 124, 255},
    {1, 58, 99, 255},
    {1, 42, 74, 255},
    {1, 20, 50, 255},
    {1, 14, 30, 255},
    {1, 7, 15, 255},
    {1, 1, 1, 255},
    {1, 1, 1, 255},
    {1, 1, 1, 255},
    {1, 1, 1, 255},
    {1, 1, 1, 255},
    {1, 1, 1, 255}};

bool initializeSDL(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow("2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	
	if (!app.window)
	{
		printf("Failed to open %d x %d window: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());
		exit(1);
	}

	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	if (!app.renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL_TTF: %s\n", SDL_GetError());
		exit(1);
	}

	return true;
}

// design the tiles
void drawTileText(const char *text, SDL_Rect rect)
{
	// get font
	TTF_Font *font = TTF_OpenFont("SansSemiBold.ttf", CELL_FONT_SIZE);
	if (font == NULL)
	{
		printf("Font not found: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Color color = {255, 255, 255};
	SDL_Surface *surfaceMessage = TTF_RenderText_Blended(font, text, color); 
	SDL_Texture *texture = SDL_CreateTextureFromSurface(app.renderer, surfaceMessage);
	SDL_Rect message_rect;

	TTF_SizeText(font, text, &message_rect.w, &message_rect.h);
	// center message rect in tile
	message_rect.x = rect.x + rect.w / 2 - message_rect.w / 2;
	message_rect.y = rect.y + rect.h / 2 - message_rect.h / 2;

	SDL_RenderCopy(app.renderer, texture, NULL, &message_rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surfaceMessage); 
}


void cleanUp(void)
{
	SDL_DestroyWindow(app.window);
	app.window = NULL;
	TTF_Quit();
	SDL_Quit();
}

void clearScreen()
{
	SDL_SetRenderDrawColor(app.renderer, bg.r, bg.g, bg.b, bg.a);
	SDL_RenderClear(app.renderer);
}

void drawBoard(const Board board)
{
	int squareSize = (WINDOW_WIDTH - 2 * WINDOW_PAD) / SIZE - WINDOW_PAD;

	for (int x = 0; x < SIZE; x++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			SDL_Rect fillRect = {WINDOW_PAD + x * (squareSize + WINDOW_PAD), WINDOW_PAD + y * (squareSize + WINDOW_PAD), squareSize, squareSize};
			struct COLOR s = colors[board[y][x]];
			SDL_SetRenderDrawColor(app.renderer, s.r, s.g, s.b, s.a);
			SDL_RenderFillRect(app.renderer, &fillRect);
			char str[14];
			sprintf(str, "%lu", 1 << board[y][x]);

			if (board[y][x] == 0)
			{
				str[0] = ' ';
			}
			drawTileText(str, fillRect);
		}
	}
}

int readHighScore(){
	// Open the file for reading
    FILE* fp = fopen("../../res/highscore.txt", "r");

    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("Error opening file!\n");
    }

    // Read the number from the file
    int number;
    fscanf(fp, "%d", &number);

    // Close the file
    fclose(fp);

	return number;
}

void writeHighScore(Board board){
	// Open a file for writing
    FILE* fp = fopen("../../res/highscore.txt", "w");

    // Check if the file was opened successfully
    if (fp == NULL)
    {
        printf("Error opening file!\n");
    }

    // Write the highscore to the file
    int score = calculateScore(board);
	int highscore = readHighScore();

	if(score > highscore){
		fprintf(fp, "%d", score);
	}

    // Close the file
    fclose(fp);
}

void drawHighScore()
{
    char highscore[15];
	char highscoreText[30] = "Best: ";
    sprintf(highscore, "%d", readHighScore());
    strncat(highscoreText, highscore, 15);
    SDL_Rect fillRect = {   WINDOW_WIDTH / 2 + 5,
                            WINDOW_WIDTH + WINDOW_PAD,
                            WINDOW_WIDTH / 2 - 2 * WINDOW_PAD,
                            WINDOW_HEIGHT - WINDOW_WIDTH - 2 * WINDOW_PAD };
    SDL_SetRenderDrawColor( app.renderer, 1, 1, 1, 1 );		
    SDL_RenderFillRect( app.renderer, &fillRect );
    drawTileText(highscoreText, fillRect);           	
}

void drawScore(Board board)
{
    char score[15];
	char scoreText[30] = "Score: ";
    sprintf(score, "%d", calculateScore(board));
    strncat(scoreText, score, 15);
    SDL_Rect fillRect = {   WINDOW_PAD,
                            WINDOW_WIDTH + WINDOW_PAD,
                            WINDOW_WIDTH / 2 - 2 * WINDOW_PAD,
                            WINDOW_HEIGHT - WINDOW_WIDTH - 2 * WINDOW_PAD };
    SDL_SetRenderDrawColor( app.renderer, 1, 1, 1, 1 );		
    SDL_RenderFillRect( app.renderer, &fillRect );
    drawTileText(scoreText, fillRect);           	
}

void handleMove(SDL_Event e, Board board)
{
	if(checkGameOver(board))
    {
		writeHighScore(board);
        setupNewGame(board);
        addRandomTile(board);
        return;
    }

	switch (e.key.keysym.sym)
	{
	case SDLK_UP:
		moveY(board, 0);
		break;
	case SDLK_DOWN:
		moveY(board, 1);
		break;
	case SDLK_LEFT:
		moveX(board, 0);
		break;
	case SDLK_RIGHT:
		moveX(board, 1);
		break;
	default:;
	}
}

void renderGame(Board board)
{
	clearScreen();
	drawBoard(board);
	drawHighScore();
	drawScore(board);
	SDL_RenderPresent(app.renderer);
}

void gameLoop(Board board)
{
	renderGame(board);

	SDL_Event e; // used to store events (like keyboard or mouse press)
	while (true) 
	{
		while (SDL_PollEvent(&e))
		{
			// user quits game with the 'x' button
			if (e.type == SDL_QUIT)
			{
				writeHighScore(board);
				return;
			}
			else if (e.type == SDL_KEYUP)
			{
				handleMove(e, board);
				renderGame(board);
			}
		}
	}
}


int main(int argc, char **argv)
{
	if (!initializeSDL())
		exit(EXIT_FAILURE);

	unsigned char board[SIZE][SIZE];
	setupNewGame(board);
	gameLoop(board);
	cleanUp();

	return EXIT_SUCCESS;
}
