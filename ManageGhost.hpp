#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Pacman.hpp"
#include "Ghost.hpp"

using namespace std;
using namespace sf;

class ManageGhost
{
    int curr_wave;
    int w_time;
    vector<Ghost> ghosts;

public:
    ManageGhost();
    void draw(bool flash, RenderWindow& window);
    void reset(const vector<Position>& ghostPos);
    void update(vector<vector<Tile>>& map, Pacman& pac, bool started);
};

ManageGhost::ManageGhost() : curr_wave(0), w_time(LONG_SCATTER_DURATION)
{
    for (int i = 0; i < 4; ++i) {
        ghosts.emplace_back(i);
    }
}

void ManageGhost::draw(bool flash, RenderWindow& window)
{
    for (int i = 0; i < 4; ++i)
    {
        ghosts[i].draw(flash, window);
    }
}

void ManageGhost::reset(const vector<Position>& ghostPos)
{
    curr_wave = 0;
    w_time = LONG_SCATTER_DURATION;

    for (size_t i = 0; i < ghosts.size(); ++i)
    {
        ghosts[i].set_position(ghostPos[i].x, ghostPos[i].y);
    }
    //for (Ghost& ghost : ghosts)
    for(size_t i = 0; i < ghosts.size(); ++i)
    {
        ghosts[i].reset(ghosts[2].get_position(), ghosts[0].get_position());
    }
}

void ManageGhost::update(vector<vector<Tile>>& map, Pacman& pac, bool started)
{
    if (!pac.get_powerup_frames()) //dont update wave time
    {
        if (0 == w_time)
        {
            if (7 > curr_wave)
            {
                curr_wave++;
                for (Ghost& ghost : ghosts)
                {
                    ghost.switch_mode();
                }
            }

            if (1 == curr_wave % 2)
            {
                w_time = CHASE_DURATION;
            }
            else if (2 == curr_wave)
            {
                w_time = LONG_SCATTER_DURATION;
            }
            else
            {
                w_time = SHORT_SCATTER_DURATION;
            }
        }
        else
        {
            w_time--;
        }
    }

    for (int i=0; i<4; i++)
    {
        ghosts[i].update(map, ghosts[0], pac,started);
    }
}
