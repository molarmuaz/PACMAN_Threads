#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Global.hpp"
#include "Collision.hpp"

using namespace std;

class Pacman
{
    bool end;
    bool dead;
    char dir;
    int animation_frames;
    int powerup_frames;
    Position position;

public:
    Pacman() : end(false), dead(false), dir(0), powerup_frames(0), position({0, 0}) {}

    bool get_end() { return end; }

    bool get_dead() { return dead; }

    char get_direction() { return dir; }

    int get_powerup_frames() { return powerup_frames; }

    void draw(bool win, sf::RenderWindow& window)
    {
        char frame = (floor(animation_frames / float(PACMAN_ANIMATION_SPEED)));

        sf::Sprite sprite;
        sf::Texture texture;

        sprite.setPosition(position.x, position.y);

        if (dead || win)
        {
            if (animation_frames < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
            {
                animation_frames++;

                texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

                sprite.setTexture(texture);
                sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

                window.draw(sprite);
            }
            else
            {
                end = true;
            }
        }
        else
        {
            texture.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png");

            sprite.setTexture(texture);
            sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * dir, CELL_SIZE, CELL_SIZE));

            window.draw(sprite);

            animation_frames = (1 + animation_frames) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
        }
    }

    void reset()
    {
        end = false;
        dead = false;
        dir = 0;
        animation_frames = 0;
        powerup_frames = 0;
        position.x = 160;
        position.y = 240;
    }

    void set_animation_frames(int a) { animation_frames = a; }

    void set_dead(bool a)
    {
        dead = a;

        if (dead)
        {             
            animation_frames = 0;
        }
    }

    void set_position(int x, int y) 
    { 
        position = {x, y}; 
    }

    void update(vector<vector<Tile>>& map, bool started)
    {
        if(started)
        {
            vector<bool> wall(4);
            wall[0] = collide(0, 0, PACMAN_SPEED + position.x, position.y, map);
            wall[1] = collide(0, 0, position.x, position.y - PACMAN_SPEED, map);
            wall[2] = collide(0, 0, position.x - PACMAN_SPEED, position.y, map);
            wall[3] = collide(0, 0, position.x, PACMAN_SPEED + position.y, map);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (!wall[0]) // You can't turn in this direction if there's a wall there.
                {
                    dir = 0;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (!wall[1])
                {
                    dir = 1;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (!wall[2])
                {
                    dir = 2;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (!wall[3])
                {
                    dir = 3;
                }
            }

            if (!wall[dir])
            {
                switch (dir)
                {
                case 0:
                    position.x += PACMAN_SPEED;
                    break;
                case 1:
                    position.y -= PACMAN_SPEED;
                    break;
                case 2:
                    position.x -= PACMAN_SPEED;
                    break;
                case 3:
                    position.y += PACMAN_SPEED;
                    break;
                }
            }

            if (-CELL_SIZE >= position.x)
            {
                position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
            }
            else if (CELL_SIZE * MAP_WIDTH <= position.x)
            {
                position.x = PACMAN_SPEED - CELL_SIZE;
            }

            if (collide(1, 0, position.x, position.y, map)) // When Pacman eats an energizer...
            {
                // He becomes energized!
                powerup_frames = (ENERGIZER_DURATION);
            }
            else
            {
                powerup_frames = std::max(0, (powerup_frames) - 1);
            }

        }
    }

    Position get_position() { return position; }
};
