#ifndef __core_h__
#define __core_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

//
// Helper macros
#define ABS(X)                  (((X) < 0) ? (-(X)) : (X))
#define CharIsDigit(X)          ((X) >= '0' && (X) <= '9')
#define CompareString(X, Y, Z)  (strncmp(X, Y, Z) == 0)

void log_error(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fputs("error: ", stderr);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

// ====----------------------------------------------------------------------------------------
//
//
//
// Direction
//
//
//
// ----------------------------------------------------------------------------------------====

typedef enum
{
    DIRECTION_up,
    DIRECTION_down,
    DIRECTION_right,
    DIRECTION_left,
} DIRECTION;

static const char* DIRECTION_to_cstr(DIRECTION direction)
{
    switch (direction)
    {
    case DIRECTION_up: return "Up";
    case DIRECTION_down: return "down";
    case DIRECTION_left: return "left";
    case DIRECTION_right: return "right";
    }
    return NULL;
}

// ====----------------------------------------------------------------------------------------
//
//
//
// Position
//
//
//
// ----------------------------------------------------------------------------------------====

typedef struct
{
    unsigned int x,
                 y,
                 boundary;
} Position;

int PositionIsEq(const Position* first, const Position* second)
{
    return ((first->x == second->x) && (first->y == second->y)) ? 1 : 0;
}

Position PositionMake(unsigned int x, unsigned int y, unsigned int boundary)
{
    return (Position) {
        .x        = x,
        .y        = y,
        .boundary = boundary,
    };
}

void PositionInc(Position* pos, int inc)
{
    pos->x += inc;
    pos->y += inc;
}

void PositionMoveUp(Position* pos)
{
    if (pos->x <= 0) {
        pos->x = pos->boundary;
    } else {
        pos->x -= 1;
    }
}

void PositionMoveDown(Position* pos)
{
    if (pos->x >= pos->boundary) {
        pos->x = 0;
    } else {
        pos->x += 1;
    }
}

void PositionMoveLeft(Position* pos)
{
    if (pos->y <= 0) {
        pos->y = pos->boundary;
    } else {
        pos->y -= 1;
    }
}

void PositionMoveRight(Position* pos)
{
    if (pos->y >= pos->boundary) {
        pos->y = 0;
    } else {
        pos->y += 1;
    }
}

// ====----------------------------------------------------------------------------------------
//
//
//
// Snake
//
//
//
// ----------------------------------------------------------------------------------------====

#ifndef SNAKE_MAX_CELLS
#define SNAKE_MAX_CELLS 32
#endif

typedef struct
{
    char           name[32];
    unsigned int   name_length;

    Position       start, end;
    Position       cells[SNAKE_MAX_CELLS];
    unsigned int   cells_count;
    DIRECTION      direction;
    unsigned int   boundary;
} Snake;

void SnakeInit(Snake* snake, char* name, unsigned int name_length, unsigned int boundary, Position start, Position end)
{
    strncpy(snake->name, name, name_length);
    snake->boundary = boundary;
    snake->cells_count = 0;
    snake->start = start;
    snake->end = end;

    //
    // Init direction
    if (snake->start.x == snake->end.x) {
        if (snake->start.y > snake->end.y)
            snake->direction = DIRECTION_right;
        else
            snake->direction = DIRECTION_left;
    } else if (snake->start.y == snake->end.y) {
        if (snake->start.x > snake->end.x)
            snake->direction = DIRECTION_down;
        else
            snake->direction = DIRECTION_up;
    } else {
        log_error("SnakeInit: start and end must be on the same row or coloumn: (%d, %d), (%d, %d)", start.x, start.y, end.x, end.y);
        assert(0 && "unreachable");
    }


    //
    // Init cells
#define FILL_CELLS(count, X, Y)\
    for (unsigned int i = 1; i < count; ++i) {\
        snake->cells[snake->cells_count++] = (Position) {\
            .x = (X),\
            .y = (Y),\
            .boundary = snake->boundary\
        };\
    }

    static unsigned int count;
    switch (snake->direction) {
    case DIRECTION_up:
    {
        if (snake->start.x > snake->end.x) {
            count = ABS(snake->start.x - snake->end.x - snake->boundary);
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, (snake->start.x + i) % snake->boundary, snake->start.y);
        } else {
            count = snake->end.x - snake->start.x;
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, (snake->start.x + i), snake->start.y);
        }
    }
    break;

    case DIRECTION_down:
    {
        if (snake->start.x < snake->end.x) {
            count = ABS(snake->end.x - snake->start.x - snake->boundary);
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, (snake->end.x + i) % snake->boundary, snake->start.x);
        } else {
            count = snake->start.x - snake->end.x;
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, snake->end.x + i, snake->start.y);
        }
    }
    break;

    case DIRECTION_right:
    {
        if (snake->start.y < snake->end.y) {
            count = ABS(snake->end.y - snake->end.y - snake->boundary);
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, snake->start.x, (snake->start.y + i) % snake->boundary);
        } else {
            count = snake->start.x - snake->end.x;
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, snake->start.x, (snake->start.y + i));
        }
    }
    break;

    case DIRECTION_left:
    {
        if (snake->start.y > snake->end.y) {
            count = ABS(snake->start.y - snake->end.y - snake->boundary);
            assert(count < SNAKE_MAX_CELLS);

            FILL_CELLS(count, snake->start.x, (snake->start.y + i) % snake->boundary);
        } else {
            count = snake->end.y - snake->start.y;
            assert(count < SNAKE_MAX_CELLS);
            FILL_CELLS(count, snake->start.x, (snake->start.y + i));
        }
    }
    break;

    default: assert(0 && "unreachable");
    }
}

void SnakeMove(Snake* s)
{

#define MOVE_CELLS(Movement)\
    for (unsigned int i = 0; i < s->cells_count; ++i) {\
        Movement(&s->cells[i]);\
    }

    switch (s->direction) {
    case DIRECTION_up:
    {
        PositionMoveUp(&s->start);
        PositionMoveUp(&s->end);
        MOVE_CELLS(PositionMoveUp);
    }
    break;

    case DIRECTION_down:
    {
        PositionMoveDown(&s->start);
        PositionMoveDown(&s->end);
        MOVE_CELLS(PositionMoveDown);
    }
    break;

    case DIRECTION_left:
    {
        PositionMoveLeft(&s->start);
        PositionMoveLeft(&s->end);
        MOVE_CELLS(PositionMoveLeft);
    }
    break;

    case DIRECTION_right:
    {
        PositionMoveRight(&s->start);
        PositionMoveRight(&s->end);
        MOVE_CELLS(PositionMoveRight);
    }
    break;

    default: assert(0 && "unreachable");
    }
}

int SnakeIsCollisionWith(const Snake* snake, const Position* pos)
{
    if (PositionIsEq(&snake->start, pos) || PositionIsEq(&snake->end, pos))
        return 1;

    for (unsigned int i = 0; i < snake->cells_count; ++i) {
        if (PositionIsEq(&snake->cells[i], pos))
                return 1;
    }

    return 0;
}

void SnakeDebugPrint(const Snake* snake)
{
    printf("Snake: %s\n", snake->name);
    printf("  dir: %s\n", DIRECTION_to_cstr(snake->direction));
    printf("  start: (%d, %d)\n", snake->start.x, snake->start.y);
    printf("  end:   (%d, %d)\n", snake->end.x, snake->end.y);
    printf("  cells:\n");
    for (unsigned int i = 0; i < snake->cells_count; ++i) {
        printf("         (%d, %d)\n", snake->cells[i].x, snake->cells[i].y);
    }
}

// ====----------------------------------------------------------------------------------------
//
//
//
// Priest
//
//
//
// ----------------------------------------------------------------------------------------====

typedef struct
{
    Position  pos;
    DIRECTION direction;
} Priest;

void PriestInit(Priest* priest, Position pos, DIRECTION direction)
{
    priest->pos = pos;
    priest->direction = direction;
}

// ====----------------------------------------------------------------------------------------
//
//
//
// Read input
//
//
//
// ----------------------------------------------------------------------------------------====

#ifndef READ_MAX_SNAKES
#define READ_MAX_SNAKES 32
#endif

typedef struct
{
    int      boardSize;
    Snake    snakes[READ_MAX_SNAKES];
    int      snakes_count;
    Priest   priest;
    Position priest_win_position;
} Game;

int InitGameFromFile(Game* info, FILE* fp)
{
    static char buffer[256];
    static unsigned int curr;

    //
    // Read board size
    if (! fgets(buffer, 256, fp)) {
        log_error("input: failed to read boardSize");
        return -1;
    }

    curr = 0;
    while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
        info->boardSize = info->boardSize* 10 + (buffer[curr] - '0');
        curr += 1;
    }
    memset(buffer, 0, 256);

    //
    // Read snakes count
    if (! fgets(buffer, 256, fp)) {
        log_error("input: failed to read snakes count");
        return -1;
    }

    curr = 0;
    while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
        info->snakes_count = info->snakes_count* 10 + (buffer[curr] - '0');
        curr += 1;
    }
    assert(info->snakes_count < READ_MAX_SNAKES);
    memset(buffer, 0, 256);

    // --------------------------------------------------------------------------------------------
    // Init snakes
    static Snake* snake;

    for (unsigned int i = 0; i < info->snakes_count; ++i) {
        if (! fgets(buffer, 256, fp)) {
            log_error("input: failed to read snake line");
            return -1;
        }

        Position start = {0};
        Position end   = {0};
        snake = &info->snakes[i];

        // NOTE(madflash) - Set start and end's boundary
        // Since the SnakeInit accpets position's with their boundary set
        start.boundary = end.boundary = info->boardSize - 1;

        // NOTE(madflash) - 
        // Format: [snake_name][ ][start.x][,][start.y][space][end.x][,][end.y]

        //
        // Read snake line
        curr = 0;
        while (buffer[curr] != '\0' && buffer[curr]!= ' ') curr += 1;
        char* name =  buffer;
        unsigned int name_length = curr;

        //
        // Skip spaces
        while (buffer[curr] != '\0' && buffer[curr] == ' ') curr += 1;

        // 
        // Read Position start
        while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
            start.x = start.x* 10 + (buffer[curr] - '0');
            curr += 1;
        }

        if (buffer[curr] != ',') {
            log_error("input: Expected ',' after start.x position, got '%c'", buffer[curr]);
            return -1;
        }
        curr += 1;

        while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
            start.y = start.y* 10 + (buffer[curr] - '0');
            curr += 1;
        }

        //
        // Skip spaces
        while (buffer[curr] != '\0' && buffer[curr] == ' ') curr += 1;

        // Read Position end
        while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
            end.x = end.x* 10 + (buffer[curr] - '0');
            curr += 1;
        }

        if (buffer[curr] != ',') {
            log_error("input: expected comma after end.x position");
            return -1;
        }

        curr += 1;
        while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
            end.y = end.y* 10 + (buffer[curr] - '0');
            curr += 1;
        }

        // NOTE(madflash) - Minus 1 from start and end, to make it compatible with zero-based indexing
        PositionInc(&start, -1);
        PositionInc(&end, -1);
        SnakeInit(snake, name, name_length, info->boardSize - 1, start, end);

        // NOTE(madflash) - Clear buffer
        memset(buffer, 0, 256);
    }

    // --------------------------------------------------------------------------------------------
    // Init priest
    if (! fgets(buffer, 256, fp))
        return -1;

    unsigned int start = 0;
    curr = 1;
    while (buffer[curr] != '\0' && CharIsDigit(buffer[curr])) {
        start += start * 10 + (buffer[curr] - '0');
        curr += 1;
    }

    switch (buffer[0]) {
    case 'W':
    {
        info->priest.direction = DIRECTION_right;
        assert(start == 2);
        info->priest.pos = PositionMake(start - 1, 0, info->boardSize - 1);
        info->priest_win_position = PositionMake(start - 1, info->boardSize - 1, info->boardSize - 1);
    }
    break;

    case 'E':
    {
        info->priest.direction = DIRECTION_left;
        info->priest.pos = PositionMake(start - 1, 0, info->boardSize - 1);
        info->priest_win_position = PositionMake(start - 1, 0, info->boardSize - 1);
    }
    break;

    case 'N':
    {
        info->priest.direction = DIRECTION_down;
        info->priest.pos = PositionMake(0, start - 1, info->boardSize - 1);
        info->priest_win_position = PositionMake(info->boardSize - 1, start - 1, info->boardSize - 1);
    }
    break;

    case 'S':
    {
        info->priest.direction = DIRECTION_up;
        info->priest.pos = PositionMake(info->boardSize - 1, start - 1, info->boardSize - 1);
        info->priest_win_position = PositionMake(0, start - 1, info->boardSize - 1);
    }
    break;

    default:
    {
        log_error("Unsupported direction: %c", buffer[0]);
        return -1;
    }
    }

    return 0;
}

int GameIsPriestCollision(Game* game)
{
    for (unsigned int i = 0; i < game->snakes_count; ++i) {
        if (SnakeIsCollisionWith(&game->snakes[i], &game->priest.pos))
            return 1;
    }

    return 0;
}

int GameIsPriestAtWinningPosition(Game* game)
{
    return (PositionIsEq(&game->priest.pos, &game->priest_win_position)) ? 1 : 0;
}

#ifdef TEST_CORE
#include <stdio.h>
int main(void)
{
    Snake s;
    SnakeInit(&s, "lucy", 4, 7, PositionMake(1, 2, 7), PositionMake(1, 5, 7));
    SnakeDebugPrint(&s);

    assert(s.cells_count == 2);
    assert(s.direction == DIRECTION_left);
    assert(s.cells[0].x == 1 && s.cells[0].y == 3);
    assert(s.cells[1].x == 1 && s.cells[1].y == 4);

    SnakeMove(&s);
    SnakeMove(&s);
    SnakeMove(&s);
    SnakeDebugPrint(&s);

    assert(s.start.x == 1 && s.start.y == 7);
    assert(s.end.x == 1 && s.end.y == 2);
    assert(s.cells[0].x == 1 && s.cells[0].y == 0);
    assert(s.cells[1].x == 1 && s.cells[1].y == 1);

    //
    // Test InitGameFromFile function
    FILE* fp = fopen("examples/input-0.txt", "r");
    assert(fp);

    Game info = {0};
    assert(InitGameFromFile(&info, fp) >= 0);

    assert(info.boardSize == 10);
    assert(info.snakes_count == 4);

    assert(CompareString(info.snakes[0].name, "Snake1", info.snakes[0].name_length));
    assert(CompareString(info.snakes[1].name, "Snake2", info.snakes[1].name_length));
    assert(CompareString(info.snakes[2].name, "Snake3", info.snakes[2].name_length));
    assert(CompareString(info.snakes[3].name, "Snake4", info.snakes[3].name_length));
    
    assert(info.snakes[0].start.x == 1 && info.snakes[0].start.y == 5);
    assert(info.snakes[1].start.x == 7 && info.snakes[1].start.y == 4);
    assert(info.snakes[2].start.x == 2 && info.snakes[2].start.y == 10);
    assert(info.snakes[3].start.x == 4 && info.snakes[3].start.y == 2);

    assert(info.snakes[0].end.x == 1 && info.snakes[0].end.y == 8);
    assert(info.snakes[1].end.x == 7 && info.snakes[1].end.y == 7);
    assert(info.snakes[2].end.x == 4 && info.snakes[2].end.y == 10);
    assert(info.snakes[3].end.x == 2 && info.snakes[3].end.y == 2);

    assert(info.priest.direction == DIRECTION_right);
    assert(info.priest.pos.x == 1 && info.priest.pos.y == 0);
    assert(info.priest_win_position.x == info.priest.pos.x && info.priest_win_position.y == info.boardSize - 1);

    fclose(fp);
    return 0;
}
#endif

#endif  // __core_h__
