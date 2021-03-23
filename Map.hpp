#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <iostream>
#include "Hero.hpp"
#include "Map.hpp"
using namespace std;

   const int width = 20;
   const int height = 20;
   hero Bugo(1,1,1,1,1);

//class map{
  //  private:
        char level[height][width];
    


void initializeArray(); 
void move(char mossa);

//};
#endif