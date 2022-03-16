#include "core.h"
#include <iostream>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH  600

void render_and_update_snakes(sf::RenderWindow& window, Game* game, unsigned int cell_width)
{
    static sf::RectangleShape cell;
    for (unsigned int i = 0; i < game->snakes_count; ++i) {
        Snake* snake = &game->snakes[i];

        cell.setOutlineColor(sf::Color::Green);
        cell.setFillColor(sf::Color::Green);
        cell.setPosition(snake->start.y * cell_width, snake->start.x * cell_width);
        cell.setSize(sf::Vector2f(cell_width, cell_width));
        window.draw(cell);

        cell.setOutlineColor(sf::Color::Green);
        cell.setFillColor(sf::Color::Green);
        cell.setPosition(snake->end.y * cell_width, snake->end.x * cell_width);
        cell.setSize(sf::Vector2f(cell_width, cell_width));
        window.draw(cell);


        for (unsigned int i = 0; i < snake->cells_count; ++i) {
            cell.setOutlineColor(sf::Color::Green);
            cell.setFillColor(sf::Color::Green);
            cell.setPosition(snake->cells[0].y * cell_width, snake->cells[0].x * cell_width);
            cell.setSize(sf::Vector2f(cell_width, cell_width));
            window.draw(cell);
        }
        SnakeMove(snake);
    }
}

void render_and_update_priest(sf::RenderWindow& window, Game* game, unsigned int cell_width)
{
    static sf::RectangleShape cell;
    cell.setOutlineColor(sf::Color::Red);
    cell.setFillColor(sf::Color::Red);
    cell.setPosition(game->priest.pos.y * cell_width, game->priest.pos.x * cell_width);
    cell.setSize(sf::Vector2f(cell_width, cell_width));
    window.draw(cell);
    GameUpdatePriest(game);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        log_error("Usage: game-sfml <input-file>");
        exit(1);
    }

    FILE* input_file = fopen(argv[1], "r");
    if (! input_file) {
        log_error("Couldn' open file: %s", argv[0]);
        exit(1);
    }

    Game game;
    if (InitGameFromFile(&game, input_file)) {
        fclose(input_file);
        exit(1);
    }
    unsigned int cell_width = SCREEN_HEIGHT / game.boardSize;

    sf::RenderWindow window { sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Snake and a priest", sf::Style::Titlebar };
    sf::Event ev;
    window.clear(sf::Color::Black);

    sf::Clock clock;
    render_and_update_snakes(window, &game, cell_width);
    render_and_update_priest(window, &game, cell_width);
    window.display();

    bool isQuit = false;
    const Snake* BittenBy;

    while (! isQuit) {
        while (window.pollEvent(ev)) {
            switch (ev.type) {
                case sf::Event::Closed: {
                    isQuit = true;
                }
                break;

                case sf::Event::KeyPressed: {
                    if (ev.key.code == sf::Keyboard::Escape) {
                        isQuit = true;
                    }
                }
                break;
            }
        }

        if (GameIsPriestAtWinningPosition(&game)) {
            std::cout << "NIRVANAN";
            sf::sleep(sf::milliseconds(10000));
            isQuit = true;
        }

        BittenBy = GameIsPriestCollision(&game);
        if (BittenBy) {
            printf("%s %d, %d\n", BittenBy->name, game.priest.pos.x, game.priest.pos.y);
            sf::sleep(sf::milliseconds(1000));
            isQuit = true;
        }

        if (clock.getElapsedTime() >= sf::microseconds(1000000)) {
            render_and_update_snakes(window, &game, cell_width);
            render_and_update_priest(window, &game, cell_width);
            clock.restart();
            window.display();
        }

        window.clear();
    }

    window.close();
    return 0;
}
