import enum
from dataclasses import dataclass

@dataclass
class Position:
    x: int
    y: int

    def MoveUp(self, limit):
        self.x -= 1
        if self.x < 0:
            self.x = limit - 1

    def MoveDown(self, limit):
        self.x += 1
        if self.x >= limit:
            self.x = 0

    def MoveLeft(self, limit):
        self.y -= 1
        if self.y < 0:
            self.y = limit - 1

    def MoveRight(self, limit):
        self.y += 1
        if self.y > limit:
            self.y = 0

    def __gt__(self, other):
        if self.x == other.x:
            return self.y > other.y
        else:
            return self.x > other.x

    def __eq__(self, other):
        if self.x == other.x and self.y == other.y:
            return True
        else:
            return False
    
class Direction(enum.Enum):
    Right = 1
    Left = 2
    Up = 3
    Down = 4

class Snake:
    def __init__(self, name, start, end):
        self.name: str = name
        self.start: Position = start
        self.end: Position = end
        self.cells = []

        self.direction: Direction
        if self.start.x == self.end.x:
            if self.start.y > self.end.y:
                self.direction = Direction.Right
            else:
                self.direction = Direction.Left
        else:
            if self.start.x > self.end.x:
                self.direction = Direction.Down
            else:
                self.direction = Direction.Up

        self.init_cells()

    def init_cells(self) -> None:
        if self.direction == Direction.Up:
            if self.start.x > self.end.x:
                count = abs((self.start.x - self.end.x) - board_size)
                for i in range(1, count):
                    new_x = (self.start.x + i) % board_size
                    self.cells.append(Position(new_x, self.start.y))
            else:
                count = self.end.x - self.start.x
                for i in range(1, count):
                    new_x = (self.start.x + i)
                    self.cells.append(Position(new_x, self.start.y))

        elif self.direction == Direction.Down:
            if self.start.x < self.end.x:
                count = abs((self.end.x - self.start.x) - board_size)
                for i in range(1, count):
                    new_x = (self.end.x + i) % board_size
                    self.cells.append(Position(new_x, self.start.x))
            else:
                count = self.start.x - self.end.x
                for i in range(1, count):
                    new_x = (self.end.x + i)
                    self.cells.append(Position(new_x, self.start.y))

        elif self.direction == Direction.Right:
            if self.start.y < self.end.y:
                count = abs((self.start.y - self.end.y) - board_size)
                for i in range(1, count):
                    new_y = (self.start.y + i) % board_size
                    self.cells.append(Position(self.start.x, new_y))
            else:
                count = self.start.y - self.end.y
                for i in range(1, count):
                    new_y = (self.start.y + i)
                    self.cells.append(Position(self.start.x, new_y))

        elif self.direction == Direction.Left:
            if self.start.y > self.end.y:
                count = abs((self.start.y - self.end.y) - board_size)
                for i in range(1, count):
                    new_y = (self.start.y + i) % board_size
                    self.cells.append(Position(self.start.x, new_y))
            else:
                count = self.end.y - self.start.y
                for i in range(1, count):
                    new_y = (self.start.y + i)
                    self.cells.append(Position(self.start.x, new_y))
        
    def __str__(self):
        return f"""
    {self.name}
        start: ({self.start.x} {self.start.y})
        end: ({self.end.x}, {self.end.y})
        Direction: {self.direction}
        Cells: {self.cells}
    """


class Priest:
    def __init__(self, pos, direction):
        self.pos: Position = pos
        self.direction: Direction = direction

def init_board():
    size = int(input())
    return size * [size * [0]]

def init_snakes():
    snakes = []
    snakes_count = int(input())

    def getPos(lst):
        return list(map(lambda x: x - 1, map(int, lst)))

    for _ in range(snakes_count):
        s = input().split(' ')
        start_pos = getPos(s[1].split(','))
        end_pos = getPos(s[2].split(','))

        snakes.append(Snake(s[0],
            Position(start_pos[0], start_pos[1]),
            Position(end_pos[0], end_pos[1]),
        ))

    return snakes

def print_snakes(snakes) -> None:
    for i in snakes:
        print(i)

def init_priest(board_size) -> (Priest, Position):
    pos: Position
    respective_end: Position
    direction: Position

    data = input()
    d = data[0]
    start = int(data[1])

    if d == 'W':
        pos = Position(start - 1, 0)
        direction = Direction.Right
        respective_end = Position(start - 1, board_size - 1)
    elif d == 'E':
        pos = Position(start - 1, 0)
        direction = Direction.Left
        respective_end = Position(start - 1, 0)
    elif d == 'N':
        pos = Position(0, start - 1)
        direction = Direction.Down
        respective_end = Position(board_size - 1, start - 1)
    elif d == 'S':
        pos = Position(board_size - 1, start - 1)
        direction = Direction.Up
        respective_end = Position(0, start - 1)

    return (Priest(pos, direction), respective_end)

def update_snakes(snakes, board_size):
    for i in snakes:
        if i.direction == Direction.Up:
            i.start.MoveUp(board_size)
            i.end.MoveUp(board_size)
            
            for i in i.cells:
                i.MoveUp(board_size);

        elif i.direction == Direction.Down:
            i.start.MoveDown(board_size)
            i.end.MoveDown(board_size)

            for i in i.cells:
                i.MoveDown(board_size);

        elif i.direction == Direction.Left:
            i.start.MoveLeft(board_size)
            i.end.MoveLeft(board_size)

            for i in i.cells:
                i.MoveLeft(board_size);

        elif i.direction == Direction.Right:
            i.start.MoveRight(board_size)
            i.end.MoveRight(board_size)

            for i in i.cells:
                i.MoveRight(board_size);

    return snakes

def update_priest(priest):
    if priest.direction == Direction.Up:
        priest.pos.x -= 1
    elif priest.direction == Direction.Down:
        priest.pos.x += 1
    elif priest.direction == Direction.Right:
        priest.pos.y += 1
    elif priest.direction == Direction.Left:
        priest.pos.y -= 1

    return priest

def main():
    board = init_board()

    board_size = len(board)
    snakes = init_snakes()
    priest, respective_end = init_priest(len(board))

    bitten = False

    # print("Initial")
    # print_snakes(snakes)
    # for i in range(7):
        # print("Cycle: ", i)
        # snakes = update_snakes(snakes, len(board))
        # print_snakes(snakes)


    while priest.pos != respective_end and not bitten:
        snakes = update_snakes(snakes, board_size)
        update_priest(priest)

        for snake in snakes:
            if priest.pos == snake.start or priest.pos == snake.end or priest.pos in snake.cells:
                print(f"{snake.name} {priest.pos.x},{priest.pos.y}")
                bitten = True

    if not bitten:
        print("NIRVANAN")

if __name__ == '__main__':
    main()
