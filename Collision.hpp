#pragma once
#include <vector>
#include <cmath>

#include "Global.hpp"

using namespace std;

bool collide(bool point_collect, bool use_tp, int x, int y, vector<vector<Tile>>& map)
{
    bool output = false;

    // Getting the exact position.
    float cell_x = x / float(CELL_SIZE);
    float cell_y = y / float(CELL_SIZE);

    // A ghost/Pacman can intersect 4 cells at most.
    for (char a = 0; a < 4; a++)
    {
        int x = 0;
        int y = 0;

        switch (a)
        {
        case 0: // Top left cell
            x = (floor(cell_x));
            y = (floor(cell_y));
            break;
        case 1: // Top right cell
            x = (ceil(cell_x));
            y = (floor(cell_y));
            break;
        case 2: // Bottom left cell
            x = (floor(cell_x));
            y = (ceil(cell_y));
            break;
        case 3: // Bottom right cell
            x = (ceil(cell_x));
            y = (ceil(cell_y));
            break;
        }

        // Making sure that the position is inside the map.
        if (0 <= x && 0 <= y && map.size() > static_cast<size_t>(x) && map[0].size() > static_cast<size_t>(y))
        {
            if (!point_collect) // Here we only care about the walls.
            {
                if (map[x][y] == Tile::Wall|| (!use_tp && map[x][y] == Tile::Door))
                {
                    output = true;
                }
            }
            else // Here we only care about the collectables.
            {
                if (map[x][y] == Tile::Energizer)
                {
                    output = true;
                    map[x][y] = Tile::Empty;
                }
                else if (map[x][y] == Tile::Pellet)
                {
                    map[x][y] = Tile::Empty;
                }
            }
        }
    }

    return output;
}
