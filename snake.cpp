#include <iostream>
#include <conio.h>
#include <windows.h>
#include<time.h>

using namespace std;
bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;

void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}
void Draw()
{
    
    system("cls");
    for (int i = 0; i < width+1; i++)
        cout << "#";
    cout << endl;
 
    for (int i = 1; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0 || j == width - 1)
                cout << "#";

            if (i == y && j == x)
                cout << "O";

            else if (i == fruitY && j == fruitX)
                cout << "F";

            else 
                cout<<" ";
        }
        cout << endl;
    }
  
    for (int i = 0; i < width+1; i++)
        cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}

void Input()
{
  if (kbhit())
  {
      switch (getch())
      {
      case 'a':
        dir = LEFT;
        break;
      case 'd':
        dir = RIGHT;
        break;        
      case 'w':
        dir = UP;
        break; 
      case 's':
        dir = DOWN;
        break;
       case 'x':              
        gameOver = true;
          break;
      }
  }  
}
void Logic()
{
    switch(dir)
    {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case DOWN:
            y++;
            break;
        case UP:
            y--;
            break;
    }

    if(x==fruitX && y==fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height; 
    }
}

int main()
{
    srand(time(0));
    Setup();
    
    while (!gameOver){
        Draw();
        Input();
        Logic();
        Sleep(100); //sleep(10);
    }
    return 0;
}