#pragma once

//I won't explain this.
const char CELL_SIZE = 16;
//This too.
const char FONT_HEIGHT = 16;
//Okay, I'll explain this.
//I start counting everything from 0, so this is actually the second ghost.
//The website used smaller cells, so I'm setting smaller values.
//Which cell in front of Pacman the pink ghost will target.
const char GHOST_1_CHASE = 2;
//Which cell in front of Pacman the blue ghost will use to calculate the target.
const char GHOST_2_CHASE = 1;
//If the distance between the orange ghost and Pacman is less than this value, the orange ghost will go into the scatter mode.
const char GHOST_3_CHASE = 4;
//How many frames are in the ghost body animation
const char GHOST_ANIMATION_FRAMES = 6;
//What do you think?
const char GHOST_ANIMATION_SPEED = 4;
//The speed of the ghost after the Pacman touches it while being energized.
const char GHOST_ESCAPE_SPEED = 4;
//Since the normal speed of the ghost is 1, and I didn't like the idea of using floating numbers, I decided to move the ghost after this number of frames.
//So the higher the value, the slower the ghost.
const char GHOST_FRIGHTENED_SPEED = 3;
//I won't explain the rest. Bite me!
const char GHOST_SPEED = 1;
const char MAP_HEIGHT = 21;
const char MAP_WIDTH = 21;
const char PACMAN_ANIMATION_FRAMES = 6;
const char PACMAN_ANIMATION_SPEED = 4;
const char PACMAN_DEATH_FRAMES = 12;
const char PACMAN_SPEED = 2;
const char SCREEN_RESIZE = 2;

//This is in frames. So don't be surprised if the numbers are too big.
const int CHASE_DURATION = 1024;
const int ENERGIZER_DURATION = 200;
const int FRAME_DURATION = 16667;
const int GHOST_FLASH_START = 64;
const int LONG_SCATTER_DURATION = 512;
const int SHORT_SCATTER_DURATION = 256;


enum Tile 
{
    Wall,
    Door,
    Pellet,
    Energizer,
    Empty
};

struct Position
{
	int x;
	int y;

	bool operator==(const Position& a)
	{
		return this->x == a.x && this->y == a.y;
	}
};