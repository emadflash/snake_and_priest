#!/usr/bin/env sh

if [ "$1" = "test" ]
then
    gcc -x c src/core.h -DTEST_CORE -ggdb -fsanitize=address -o test && ./test
    exit
fi

if [ "$1" = "solution" ]
then
    gcc src/solution.c src/core.h -O3 -o solution && ./solution
    exit
fi
