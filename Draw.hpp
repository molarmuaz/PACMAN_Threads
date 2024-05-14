#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Global.hpp"

using namespace std;
using namespace sf;

void draw_map(const vector<vector<Tile>>& map, RenderWindow& window)
{
    sf::Sprite sprite;

    sf::Texture texture;
    texture.loadFromFile("Resources/Images/Map" + std::to_string(CELL_SIZE) + ".png");

    sprite.setTexture(texture);

    for (char a = 0; a < MAP_WIDTH; a++)
    {
        for (char b = 0; b < MAP_HEIGHT; b++)
        {
            sprite.setPosition(float(CELL_SIZE * a), float(CELL_SIZE * b));

            switch (map[a][b])
            {
                case Tile::Door:
                {
                    sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

                    window.draw(sprite);

                    break;
                }
                case Tile::Energizer:
                {
                    sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

                    window.draw(sprite);

                    break;
                }
                case Tile::Pellet:
                {
                    sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));

                    window.draw(sprite);

                    break;
                }
                case Tile::Wall:
                {
                    bool down = 0;
                    bool left = 0;
                    bool right = 0;
                    bool up = 0;

                    if (b < MAP_HEIGHT - 1)
                    {
                        if (map[a][1 + b] == Tile::Wall)
                        {
                            down = 1;
                        }
                    }

                    // Since we have warp tunnels, we need to draw them as if they're connected.
                    if (0 < a)
                    {
                        if (map[a - 1][b] == Tile::Wall)
                        {
                            left = 1;
                        }
                    }
                    else
                    {
                        left = 1;
                    }

                    if (a < MAP_WIDTH - 1)
                    {
                        if (map[1 + a][b] == Tile::Wall)
                        {
                            right = 1;
                        }
                    }
                    else
                    {
                        right = 1;
                    }

                    if (0 < b)
                    {
                        if (map[a][b - 1] == Tile::Wall)
                        {
                            up = 1;
                        }
                    }

                    //--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
                    sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

                    window.draw(sprite);
                }
            }
        }
    }
}

void draw_text(bool center, int x, int y, const string& text, RenderWindow& window)
{
	int char_x = x;
	int char_y = y;
	int char_width;
	Sprite char_sprite;
	Texture char_texture;
	char_texture.loadFromFile("Resources/Images/Font.png");
	char_width = char_texture.getSize().x / 96;       //96 char
	char_sprite.setTexture(char_texture);

	if (center)
	{
		char_x = round(0.5f * (CELL_SIZE * MAP_WIDTH - char_width * text.substr(0, text.find_first_of('\n')).size()));
		char_y = round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + count(text.begin(), text.end(), '\n'))));
	}

	for (string::const_iterator a = text.begin(); a != text.end(); a++)
	{
		if (*a == '\n')
		{
			if (center)
			{
				char_x = round(0.5f * (CELL_SIZE * MAP_WIDTH - char_width * text.substr(1 + a - text.begin(), text.find_first_of('\n', 1 + a - text.begin()) - (1 + a - text.begin())).size()));
			}
			else
			{
				char_x = x;
			}
			char_y += FONT_HEIGHT;
			continue;
		}

		char_sprite.setPosition(char_x, char_y);
		char_sprite.setTextureRect(IntRect(char_width * (*a - 32), 0, char_width, FONT_HEIGHT));//32 is space
		char_x += char_width;
		window.draw(char_sprite);
	}
}