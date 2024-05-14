#pragma once

#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Global.hpp"
#include "Pacman.hpp"
#include "Collision.hpp"

using namespace std;
using namespace sf;

class Ghost
{
    bool game_mode;
    bool door;
    int dir;
    int ghost_mode;
    int fgamemode_timer;
    int ghost_id; // 0 - Red, 1 - Pink, 2 - Blue, 3 - Orange
    int animation_timer;
    Position ghost_home;
    Position exit_gate;
    Position position;
    Position target;

public:
    Ghost(int id);
    bool pac_ghost_collision(const Position &pacman_position);
    float target_getdistance(int dir);
    void draw(bool flash, RenderWindow &window);
    void reset(const Position &home, const Position &home_exit);
    void set_position(int x, int y);
    void switch_mode();
    void update(vector<vector<Tile>> &map, Ghost &ghost_0, Pacman &pacman, bool started);
    void update_target(int pacman_dir, const Position &ghost_0_position, const Position &pacman_position);
    Position get_position();
};

Ghost::Ghost(int id)
{
    ghost_id = id;
}

bool Ghost::pac_ghost_collision(const Position &pacman_position)
{
    if (position.x > pacman_position.x - CELL_SIZE && position.x < CELL_SIZE + pacman_position.x)
    {
        if (position.y > pacman_position.y - CELL_SIZE && position.y < CELL_SIZE + pacman_position.y)
        {
            return true;
        }
    }
    return false;
}

float Ghost::target_getdistance(int dir)
{
    int x = position.x;
    int y = position.y;

    switch (dir)
    {
    case 0:
    {
        x += GHOST_SPEED;
        break;
    }
    case 1:
    {
        y -= GHOST_SPEED;
        break;
    }
    case 2:
    {
        x -= GHOST_SPEED;
        break;
    }
    case 3:
    {
        y += GHOST_SPEED;
        break;
    }
    }
    return float(sqrt(pow(x - target.x, 2) + pow(y - target.y, 2)));
}

void Ghost::draw(bool flash, RenderWindow &window)
{
    int body_frame = (floor(animation_timer / float(GHOST_ANIMATION_SPEED)));
    Sprite body;
    Sprite face;
    Texture texture;
    texture.loadFromFile("Resources/Images/Ghost" + to_string(CELL_SIZE) + ".png");
    body.setTexture(texture);
    body.setPosition(position.x, position.y);
    body.setTextureRect(IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));
    face.setTexture(texture);
    face.setPosition(position.x, position.y);

    if (!ghost_mode)
    {
        switch (ghost_id)
        {
        case 0://red
        {
            body.setColor(sf::Color(255, 0, 0));
            break;
        }
        case 1://pink
        {
            body.setColor(sf::Color(255, 182, 255));
            break;
        }
        case 2://blue
        {
            body.setColor(sf::Color(0, 255, 255));
            break;
        }
        case 3://orange
        {
            body.setColor(sf::Color(255, 182, 85));
        }
        }

        face.setTextureRect(sf::IntRect(CELL_SIZE * dir, CELL_SIZE, CELL_SIZE, CELL_SIZE));

        window.draw(body);
    }
    else if (ghost_mode == 1)//frightened
    {
        body.setColor(sf::Color(36, 36, 255));
        face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

        if (flash && body_frame % 2 == 0)
        {
            body.setColor(sf::Color(255, 255, 255));
            face.setColor(sf::Color(255, 0, 0));
        }
        else
        {
            body.setColor(sf::Color(36, 36, 255));
            face.setColor(sf::Color(255, 255, 255));
        }
        window.draw(body);
    }
    else//very frightened
    {
        face.setTextureRect(sf::IntRect(CELL_SIZE * dir, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
    }

    face.setTextureRect(IntRect(CELL_SIZE * dir, CELL_SIZE, CELL_SIZE, CELL_SIZE));
    window.draw(body);
    window.draw(face);
    animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void Ghost::reset(const Position &home, const Position &home_exit)
{
    game_mode = false;
    door = ghost_id > 0;
    dir = 0;
    ghost_mode = 0;
    fgamemode_timer = 0;
    animation_timer = 0;
    ghost_home = home;
    exit_gate = home_exit;
    target = home_exit;
}

void Ghost::set_position(int x, int y)
{
    position = {x, y};
}

void Ghost::switch_mode()
{
    game_mode = !game_mode;
}

void Ghost::update(vector<vector<Tile>> &map, Ghost &ghost_0, Pacman &pacman, bool started)
{
    if (started)
    {
        bool move = false;
        int available_ways = 0;
        int speed = GHOST_SPEED;
        vector<bool> walls(4, false);

        if (!ghost_mode && pacman.get_powerup_frames() == ENERGIZER_DURATION)
        {
            fgamemode_timer = GHOST_FRIGHTENED_SPEED;
            ghost_mode = 1;
        }
        else if (!pacman.get_powerup_frames() && ghost_mode == 1)
        {
            ghost_mode = 0;
        }

        if (ghost_mode == 2 && position.x % GHOST_ESCAPE_SPEED == 0 && position.y % GHOST_ESCAPE_SPEED == 0)
        {
            speed = GHOST_ESCAPE_SPEED;
        }

        update_target(pacman.get_direction(), ghost_0.get_position(), pacman.get_position());

        walls[0] = collide(0, door, speed + position.x, position.y, map);
        walls[1] = collide(0, door, position.x, position.y - speed, map);
        walls[2] = collide(0, door, position.x - speed, position.y, map);
        walls[3] = collide(0, door, position.x, speed + position.y, map);

        if (ghost_mode != 1)
        {
            int optimal_dir = 4;
            move = true;
            for (int a = 0; a < 4; a++)
            {
                if (a == (2 + dir) % 4)
                {
                    continue;
                }
                else if (!walls[a])
                {
                    if (optimal_dir == 4)
                    {
                        optimal_dir = a;
                    }
                    available_ways++;
                    if (target_getdistance(a) < target_getdistance(optimal_dir))
                    {
                        optimal_dir = a;
                    }
                }
            }
            if (available_ways > 1)
            {
                dir = optimal_dir;
            }
            else
            {
                if (optimal_dir == 4)
                {
                    dir = (2 + dir) % 4;
                }
                else
                {
                    dir = optimal_dir;
                }
            }
        }
        else
        {
            int random_dir = rand() % 4;
            if (fgamemode_timer == 0)
            {
                move = true;
                fgamemode_timer = GHOST_FRIGHTENED_SPEED;
                for (int a = 0; a < 4; a++)
                {
                    if (a == (2 + dir) % 4) // cant turn back
                    {
                        continue;
                    }
                    else if (!walls[a])
                    {
                        available_ways++;
                    }
                }
                if (available_ways > 0)
                {
                    while (walls[random_dir] || random_dir == (2 + dir) % 4)
                    {
                        random_dir = rand() % 4;
                    }
                    dir = random_dir;
                }
                else
                {
                    dir = (2 + dir) % 4; // turn back
                }
            }
            else
            {
                fgamemode_timer--;
            }
        }
        if (move)
        {
            switch (dir)
            {
            case 0:
            {
                position.x += speed;
                break;
            }
            case 1:
            {
                position.y -= speed;
                break;
            }
            case 2:
            {
                position.x -= speed;
                break;
            }
            case 3:
            {
                position.y += speed;
                break;
            }
            }
            if (-CELL_SIZE >= position.x) // ghost stays in map
            {
                position.x = CELL_SIZE * MAP_WIDTH - speed;
            }
            else if (position.x >= CELL_SIZE * MAP_WIDTH)
            {
                position.x = speed - CELL_SIZE;
            }
        }
        if (pac_ghost_collision(pacman.get_position()))
        {
            if (ghost_mode == 0)
            {
                pacman.set_dead(true);
            }
            else
            {
                door = true;
                ghost_mode = 2;
                target = ghost_home;
            }
        }
    }
}

void Ghost::update_target(int pacman_dir, const Position &ghost_0_position, const Position &pacman_position)
{
    if (door)
    {
        if (position == target)
        {
            if (exit_gate == target)
            {
                door = false;
            }
            else if (ghost_home == target)
            {
                ghost_mode = 0;
                target = exit_gate;
            }
        }
    }
    else
    {
        if (!game_mode) // scatter
        {
            switch (ghost_id)
            {
            case 0:
            {
                target = {CELL_SIZE * (MAP_WIDTH - 1), 0};
                break;
            }
            case 1:
            {
                target = {0, 0};
                break;
            }
            case 2:
            {
                target = {CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1)};
                break;
            }
            case 3:
            {
                target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
            }
            }
        }
        else // chase
        {
            target = pacman_position;
        }
    }
}

Position Ghost::get_position()
{
    return position;
}
