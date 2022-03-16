#include "core.h"
#include <stdio.h>

void update_snakes(Game* game)
{
    for (unsigned int i = 0; i < game->snakes_count; ++i) {
        SnakeMove(&game->snakes[i]);
    }
}

void update_priest(Game* game)
{
    switch (game->priest.direction) {
    case DIRECTION_up:
    PositionMoveUp(&game->priest.pos);
    break;

    case DIRECTION_down:
    PositionMoveDown(&game->priest.pos);
    break;

    case DIRECTION_left:
    PositionMoveLeft(&game->priest.pos);
    break;

    case DIRECTION_right:
    PositionMoveRight(&game->priest.pos);
    break;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        log_error("Usage: solution <input-file>");
        exit(1);
    }

    FILE* input_file = fopen(argv[1], "r");
    if (! input_file) {
        log_error("Couldn' open file: %s", argv[0]);
        exit(1);
    }

    Game game = {0};
    if (InitGameFromFile(&game, input_file) < 0) {
        fclose(input_file);
        exit(1);
    }

    int isBitten = 0;
    const Snake* BittenBy;

    while (1) {
        update_snakes(&game);
        update_priest(&game);

        BittenBy = GameIsPriestCollision(&game);
        if (BittenBy) {
            printf("%s %d, %d\n", BittenBy->name, game.priest.pos.x, game.priest.pos.y);
            isBitten = 1;
            break;
        }

        if (GameIsPriestAtWinningPosition(&game)) {
            printf("NIRVANA");
            break;
        }
    }

    fclose(input_file);
    return 0;
}
