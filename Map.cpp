#include "Map.hpp"
   const int width = 40; //scambio 
   const int height = 20; // scambio
   hero Bugo(3,1,1,1,1);
   enemy Morgan(1,5,1,7,7);
   char level[width][height];

    bool gameover(){
        bool f = false;
        if (Bugo.life <= 0){
            f= true;            
        }
        return f;
    }

    void encounter(){
        if(Bugo.Xhero==Morgan.Xenemy && Bugo.Yhero==Morgan.Yenemy){
            Bugo.life -= Morgan.damage;
            Morgan.Xenemy = -1;
            Morgan.Yenemy = -1;
        }
    }

    void initializeArray() {
        if(Bugo.life<=0)
            cout<<"GamoeOver";  
        //ciclo sulle righe   
        else{ 
            for (int row = 0; row < width; row++)
            {   
                //ciclo sulle colonne
                for (int col = 0; col < height; col++)
                {   
                    //caso muri sinistro e destro
                    if( col == 0 || col == width-1)
                    {
                        level[row][col]='#';
                        cout << level[row][col];
                    }   
                    //caso tetto e pavimento
                    else if(row == 0 || row == width-1 )
                    {
                        level[row][col] = '#';
                        cout<< level[row][col];
                    }
                    //player
                    else if(row==Bugo.Yhero && col==Bugo.Xhero)
                    {
                        level[row][col] = '$';
                        cout<<level[row][col];
                    }
                    //enemy
                    else if(row==Morgan.Yenemy && col==Morgan.Xenemy)
                    {
                        level[row][col] = 'M';
                        cout<<level[row][col];
                    }
                    //resto
                    else
                    {
                        level[row][col]=' ';
                        cout << level[row][col];
                    }
                }
                cout << "\n";   
            }
        cout<<"Life : "<<Bugo.life;
        }
    }

    void move(char mossa){
        //switch = serie di if a cascata
        //char mossa = getch();
        switch(mossa){
            case 'W': //Se premi W il codice parte da qua e scende finchè non viene gestito
            case 'w':{ //in questo caso anche se hai premuto W eseguirai il codice sottostante
                if(level[Bugo.Yhero-1][Bugo.Xhero]!='#')
                Bugo.Yhero--; //in un array bidimensionale il movimento verticale è invertito 
                            //(per andare in alto decremento)
                break; //perchè vogliamo che un solo caso gestisca il problema
            }
            case 'S':
            case 's':{ 
                if(level[Bugo.Yhero+1][Bugo.Xhero]!='#')
                Bugo.Yhero++;
                break;
            }
            case 'A':
            case 'a':{ 
                if(level[Bugo.Yhero][Bugo.Xhero-1]!='#')
                Bugo.Xhero--;
                break;
            }
            case 'D':
            case 'd':{ 
                if(level[Bugo.Yhero][Bugo.Xhero+1]!='#')
                Bugo.Xhero++;
                break;
            }
        }
       encounter();
                
    }
