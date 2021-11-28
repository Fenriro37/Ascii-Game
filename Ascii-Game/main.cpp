#include <conio.h>
#include "Game.hpp"

int main() {
    srand(time(0));
    game x;
    while (1) {
        x.logic();    
    }
    return 0;
} 