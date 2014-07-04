# A SetCover Solver with the greedy algorithm  #

This version of the greedy set cover is a well optimized linear version. 
This is a C++ implementation of greedy SetCover solver, which is the best-possible polynomial time approximation algorithm for set cover.

## How to use ##

    git clone https://github.com/bangxiang/SetCover
    cd SetCover
    make
    ./TestSetCover.exe -h


Example output  
    $ ./TestSetCover.exe -func
    
    1 2 3
    2 4
    3 4
    4 5
    min_sets: 0 3
    
    10 10 5
    5 7 8 9 10
    5 7
    1 3 6 10
    4 6 7 9 10
    1 2 8 10
    min_sets: 1 3 4 5
