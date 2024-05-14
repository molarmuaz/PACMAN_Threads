#pragma once
#include <vector>
#include <ctime> // Include ctime for time-related functions
#include <SFML/Graphics.hpp>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "Global.hpp"
#include "Pacman.hpp"
#include "Ghost.hpp"
#include "ManageGhost.hpp"
#include "UpdateMap.hpp"
#include "Draw.hpp"
#include "Collision.hpp"
using namespace std;

int lives = 3;

void* game(void* arg)
{
    bool win = false; // Detects when the game has finished
	bool started = false;

    // Used to make the game framerate-independent.
    int lag = 0;

    // Similar to lag, used to make the game framerate-independent.
    clock_t previous_time = clock(); // Use clock_t for timing

    // It's not exactly like the map from the original Pac-Man game, but it's close enough.
    vector<string> map_drawn = {
        " ################### ",
        " #........#........# ",
        " #o##.###.#.###.##o# ",
        " #.................# ",
        " #.##.#.#####.#.##.# ",
        " #....#...#...#....# ",
        " ####.### # ###.#### ",
        "    #.#   0   #.#    ",
        "#####.# ##=## #.#####",
        "     .  #123#  .     ",
        "#####.# ##### #.#####",
        "    #.#       #.#    ",
        " ####.# ##### #.#### ",
        " #........#........# ",
        " #.##.###.#.###.##.# ",
        " #o.#.....P.....#.o# ",
        " ##.#.#.#####.#.#.## ",
        " #....#...#...#....# ",
        " #.######.#.######.# ",
        " #.................# ",
        " ################### "
    };

    vector<vector<Tile>> map(MAP_WIDTH, vector<Tile>(MAP_HEIGHT));

    // Initial ghost positions.
    vector<Position> ghost_positions(4);

    // SFML thing. Stores events, I think.
    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "PACMAN BY MUAZ AND HUSSAIN", sf::Style::Close);
    // Resizing the window.
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    ManageGhost mgr;

    Pacman pacman;

    srand((time(0)));

	int total_score = 146;
	int score = 0;
	int powerupctr = 0;
    map = update_map(map_drawn, ghost_positions, pacman);

    mgr.reset(ghost_positions);

    while (window.isOpen())
    {
		if(!started)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				started = true;
		}
        // Here we're calculating the lag.
        clock_t current_time = clock();
        int delta_time = ((current_time - previous_time) * 2000000 / CLOCKS_PER_SEC); // Convert to milliseconds

        lag += delta_time;

        previous_time = current_time;

        // While the lag exceeds the maximum allowed frame duration.
        while (FRAME_DURATION <= lag)
        {
            // We decrease the lag.
            lag -= FRAME_DURATION;

            while(window.pollEvent(event))
            {
				if(event.type == sf::Event::Closed)
				{
					window.close();
				}
            }

            if (!win && !pacman.get_dead())
            {
                win = true;

                pacman.update(map,started);

                mgr.update(map, pacman,started);

                // We're checking every cell in the map.
				int scr = 0;
				int powerup = 0;
                for (const vector<Tile> &column : map)
                {
                    for (const Tile &cell : column)
                    {
                        if (cell == Tile::Pellet) // And if at least one of them has a pellet.
                        {
                            win = false; // The game is not yet won.
                            scr++;
                        }
						if(Tile::Energizer == cell)
						{
							powerup++;
						}
                    }
                }
				score = total_score-scr;
				powerupctr = powerup;
				

                if (win)
                {
                    pacman.set_animation_frames(0);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) // Restarting the game.
            {
                win = false;

				lives = 3;

                map = update_map(map_drawn, ghost_positions, pacman);

                mgr.reset(ghost_positions);

                pacman.reset();
            }

            // I don't think anything needs to be explained here.
            if (FRAME_DURATION > lag)
            {
                window.clear();

                if (!win && !pacman.get_dead())
                {
                    draw_map(map, window);

                    mgr.draw(GHOST_FLASH_START >= pacman.get_powerup_frames(), window);

                    draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, " LIVES: " + to_string(lives) + "		SCORE: " + to_string(score) + "			POWERUP: "+to_string(powerupctr), window);
                }

                pacman.draw(win, window);

                if (pacman.get_end())
                {
                    if (win)
                    {
                        draw_text(1, 0, 0, "WINNER!", window);
						draw_text(0, 133, 180, "SCORE: " + to_string(score), window);
                    }
                    else
                    {
                        if (lives == 1)
                        {
                            draw_text(1, 0, 0, "GAME OVER", window);
							draw_text(0, 133, 180, "SCORE:" + to_string(score), window);
                        }
                        else
                        {
                            lives--;
							mgr.reset(ghost_positions);
                            pacman.reset();
                        }
                    }
                }

                window.display();
            }
        }
    }
    return NULL;
}
