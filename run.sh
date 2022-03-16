#!/usr/bin/env sh

set -x

if [ "$1" = "test" ]
then
    gcc -x c src/core.h -DTEST_CORE -ggdb -fsanitize=address -o test && ./test
    exit 0
fi

if [ "$1" = "solution" ]
then
    gcc src/solution.c src/core.h -O3 -o solution && ./solution
    exit 0
fi

if [ "$1" = "game" ]
then
    g++ -std=c++17 -Wno-switch src/game-sfml.cc -lsfml-graphics -lsfml-window -lsfml-system -o game-sfml && ./game-sfml
    exit 0
fi

exit 1
