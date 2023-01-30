# I2C

# Project Description #

This project is part of the I2C module. The goal of the project is the implementation of the 2048 game. Besides the game logic a score and highscore mechanism was implemented.

# Setup #

The project is implemented on a Kali Linux VM. Visual Studio Code is used as editor. The SDL2 library is used for the user interface. 

## Compiling ##

Compiling the code works on all Debian based Linux distributions. Just do the following:
1. Create a build folder in the 0x2048 folder. 
2. Issue the command `cmake` in the build folder. 
3. Issue the command `make` in the build folder.
3. The executable should be generated in the bin folder.


# Game Logic #

The goal of the game is not to fill the playing field by skillfully moving the number blocks. To do this, the player has the ability to move up, down, right and left using the arrow keys. For each movement the player receives two points. Once the playing field is full and the player cannot make any more movements, the game is over. 
