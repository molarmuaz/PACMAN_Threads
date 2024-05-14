#pragma once
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

#include "Global.hpp"
#include "Pacman.hpp"

using namespace std;

vector<vector<Tile>> update_map(const vector<string>& map, vector<Position>& pos, Pacman& pacman)
{
    vector<vector<Tile>> result(MAP_WIDTH, vector<Tile>(MAP_HEIGHT, Tile::Empty));

    for (char a = 0; a < MAP_HEIGHT; a++)
    {
        for (char b = 0; b < MAP_WIDTH; b++)
        {
            // By default, every cell is empty.
            result[b][a] = Tile::Empty;

            switch (map[a][b])
            {
                //#wall #obstacle #youcantgothroughme
                case '#':
                {
                    result[b][a] = Tile::Wall;
                    break;
                }
                case '=':
                {
                    result[b][a] = Tile::Door;
                    break;
                }
                case '.':
                {
                    result[b][a] = Tile::Pellet;
                    break;
                }
                // Red ghost
                case '0':
                {
                    pos[0].x = CELL_SIZE * b;
                    pos[0].y = CELL_SIZE * a;

                    break;
                }
                // Pink ghost
                case '1':
                {
                    pos[1].x = CELL_SIZE * b;
                    pos[1].y = CELL_SIZE * a;

                    break;
                }
                // Blue (cyan) ghost
                case '2':
                {
                    pos[2].x = CELL_SIZE * b;
                    pos[2].y = CELL_SIZE * a;

                    break;
                }
                // Orange ghost
                case '3':
                {
                    pos[3].x = CELL_SIZE * b;
                    pos[3].y = CELL_SIZE * a;

                    break;
                }
                // Pacman!
                case 'P':
                {
                    pacman.set_position(CELL_SIZE * b, CELL_SIZE * a);

                    break;
                }
                // This looks like a surprised face.
                case 'o':
                {
                    result[b][a] = Tile::Energizer;
                }
            }
        }
    }

    return result;
}
