#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

const int PACMAN_SIZE = 16;
const float PACMAN_SPEED = 5.0f;

const int GRID_ROWS = 31;
const int GRID_COLS = 29;
const int GRID_SIZE = 20;

const int WINDOW_WIDTH = GRID_COLS * GRID_SIZE - GRID_SIZE;
const int WINDOW_HEIGHT = GRID_ROWS * GRID_SIZE - GRID_SIZE;

char grid[GRID_ROWS][GRID_COLS] =
    {
        "############################",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#o####.#####.##.#####.####o#",
        "#.####.#####.##.#####.####.#",
        "#..........................#",
        "#.####.##.########.##.####.#",
        "#.####.##.########.##.####.#",
        "#......##....##....##......#",
        "######.##### ## #####.######",
        "######.##### ## #####.######",
        "######.##          ##.######",
        "######.## ###--### ##.######",
        "######.## #      # ##.######",
        "      .   #      #   .      ",
        "######.## #      # ##.######",
        "######.## ######## ##.######",
        "######.##          ##.######",
        "######.## ######## ##.######",
        "#............##............#",
        "#.####.#####.##.#####.####.#",
        "#.####.#####.##.#####.####.#",
        "#o..##................##..o#",
        "###.##.##.########.##.##.###",
        "###.##.##.########.##.##.###",
        "#......##....##....##......#",
        "#.##########.##.##########.#",
        "#.##########.##.##########.#",
        "#..........................#",
        "############################"};

struct Pacman
{
    CircleShape shape;
    Vector2f velocity;

    Pacman()
    {
        shape.setRadius(PACMAN_SIZE / 2);
        shape.setFillColor(Color::Yellow);
        shape.setOrigin(PACMAN_SIZE / 2, PACMAN_SIZE / 2);
        shape.setPosition(130, 110);
    }

    void update(Keyboard::Key keyPressed)
    {
        // Predict the next position based on current position and velocity
        Vector2f nextPosition = shape.getPosition() + velocity;

        // Calculate the cell indices of the next position
        int cellX = static_cast<int>((nextPosition.x + PACMAN_SIZE / 2) / GRID_SIZE);
        int cellY = static_cast<int>((nextPosition.y + PACMAN_SIZE / 2) / GRID_SIZE);

        // Check for collisions with walls at the next position
        if (grid[cellY][cellX] != '#')
        {
            // If no collision, update position to the next position
            shape.setPosition(nextPosition); // Update shape position to the new position
        }
        else
        {
            // If collision, reset velocity to stop movement
            velocity = Vector2f(0.f, 0.f);
        }
        //cout<<shape.getPosition().x<<" "<<shape.getPosition().y<<endl;
    }
};

void drawPacmanMaze(sf::RenderWindow &window)
{
    for (int i = 0; i < GRID_ROWS; ++i)
    {
        for (int j = 0; j < GRID_COLS; ++j)
        {
            sf::RectangleShape cell(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            cell.setPosition(j * GRID_SIZE, i * GRID_SIZE);
            if (grid[i][j] == '#')
            {
                cell.setFillColor(sf::Color::Blue); // Color for walls
            }
            else
            {
                cell.setFillColor(sf::Color::Black); // Color for paths
            }
            window.draw(cell);
        }
    }
}

int main()
{
    RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pac-Man");
    window.setFramerateLimit(60);

    Pacman pacman;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Handle input
        pacman.velocity = Vector2f(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Up))
            pacman.velocity.y -= PACMAN_SPEED;
        if (Keyboard::isKeyPressed(Keyboard::Down))
            pacman.velocity.y += PACMAN_SPEED;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            pacman.velocity.x -= PACMAN_SPEED;
        if (Keyboard::isKeyPressed(Keyboard::Right))
            pacman.velocity.x += PACMAN_SPEED;

        // Update
        pacman.update(event.key.code);

        // Render
        window.clear();
        drawPacmanMaze(window);
        window.draw(pacman.shape); // Draw Pac-Man
        window.display();
    }
    return 0;
}
