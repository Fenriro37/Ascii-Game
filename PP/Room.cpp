#include "Room.hpp"
    
void room::encounter(){
        if(hero.XPos==enemy.XPos && hero.YPos==enemy.YPos){
            hero.life -= enemy.damage;
            enemy.XPos = -1;
            enemy.YPos = -1;
        }
    }
void room::initializeView() 
    {
        if(hero.life<=0)
            cout<<"GameOver";  
        //ciclo sulle righe   
        else{ 
            for (int row = 0; row < height; row++)
            {   
                //ciclo sulle colonne
                for (int col = 0; col < width; col++)
                {   
                    //caso muri sinistro e destro
                    if( col == 0 || col == width-1)
                    {
                        view[row][col]='#';
                        cout << view[row][col];
                    }   
                    //caso tetto e pavimento
                    else if(row == 0 || row == height-1 )
                    {
                        view[row][col] = '#';
                        cout<< view[row][col];
                    }
                    //player
                    else if(row==hero.YPos && col==hero.XPos)
                    {
                        view[row][col] = '$';
                        cout<<view[row][col];
                    }
                    //enemy
                    else if(row==enemy.YPos && col==enemy.XPos)
                    {
                        view[row][col] = 'M';
                        cout<<view[row][col];
                    }
                    //resto
                    else
                    {
                        view[row][col]=' ';
                        cout << view[row][col];
                    }
                }
                cout << "\n";   
            }
        cout<<"Life : "<<hero.life;
        }
    }

    void room::move(char mossa){
        switch(mossa){
            case 'W': //Se premi W il codice parte da qua e scende finchè non viene gestito
            case 'w':{ //in questo caso anche se hai premuto W eseguirai il codice sottostante
                if(view[hero.YPos-1][hero.XPos]!='#')
                hero.YPos--; //in un array bidimensionale il movimento verticale è invertito 
                            //(per andare in alto decremento)
                break; //perchè vogliamo che un solo caso gestisca il problema
            }
            case 'S':
            case 's':{ 
                if(view[hero.YPos+1][hero.XPos]!='#')
                hero.YPos++;
                break;
            }
            case 'A':
            case 'a':{ 
                if(view[hero.YPos][hero.XPos-1]!='#')
                hero.XPos--;
                break;
            }
            case 'D':
            case 'd':{ 
                if(view[hero.YPos][hero.XPos+1]!='#')
                hero.XPos++;
                break;
            }
        }
       encounter();
                
    }