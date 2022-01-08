void game::clearBonus(){
    itemNode* bonus  = currentroom->myRoom.getCurrentBonus();
    itemNode* prev;
    prev = bonus;
    while(bonus != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (bonus->Bonus.getTaken() && prev == bonus){
            if(bonus->next != NULL){                                
                bonus = bonus->next;
                delete prev;
                prev = bonus;

            }
            //testa unico nodo
            else {
                delete bonus;
                currentroom->myRoom.setCurrentBonus(NULL);
                bonus = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!bonus->Bonus.getTaken() && prev == bonus){  
            currentroom->myRoom.setCurrentBonus(bonus);
            bonus = bonus->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (bonus->Bonus.getTaken() && prev != bonus){
            prev->next = bonus->next;
            delete bonus;
            bonus = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = bonus;
            bonus = bonus->next;
        }   
    }
}

void game::clearEnemy(){
    enemyNode* enemy  = currentroom->myRoom.getCurrentMonsters();
    enemyNode* prev;
    prev = enemy;
    while(enemy != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (enemy->monster.getAlive() && prev == enemy){
            if(enemy->next != NULL){                                
                enemy = enemy->next;
                delete prev;
                prev = enemy;

            }
            //testa unico nodo
            else {
                delete enemy;
                currentroom->myRoom.setCurrentMonster(NULL);
                enemy = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!enemy->monster.getAlive() && prev == enemy){  
            currentroom->myRoom.setCurrentMonster(enemy);
            enemy = enemy->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (enemy->monster.getAlive() && prev != enemy){
            prev->next = enemy->next;
            delete enemy;
            enemy = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = enemy;
            enemy = enemy->next;
        }   
    }
}

void game::clearAmmo(){
    bulletNode* bullet  = currentroom->myRoom.getCurrentAmmo();
    bulletNode* prev;
    prev = bullet;
    while(bullet != NULL){
        //caso testa. La condizione ci permette di controllare se siamo ancora la testa
        if (bullet->ammo.getAlive() && prev == bullet){
            if(bullet->next != NULL){                                
                bullet = bullet->next;
                delete prev;
                prev = bullet;

            }
            //testa unico nodo
            else {
                delete bullet;
                currentroom->myRoom.setCurrentAmmo(NULL);
                bullet = NULL;
                prev = NULL;
            }
        }
        //se non devo eliminare il primo sposto di uno bonus da prev
        else if (!bullet->ammo.getAlive() && prev == bullet){  
            currentroom->myRoom.setCurrentAmmo(bullet);
            bullet = bullet->next;
        }
        //se devo eliminare un nodo che non è in testa
        else if (bullet->ammo.getAlive() && prev != bullet){
            prev->next = bullet->next;
            delete bullet;
            bullet = prev->next;
        }
        //se non devo eliminare un nodo non in testa
        else{
            prev = bullet;
            bullet = bullet->next;
        }   
    }
}
