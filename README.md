### Situazione attuale:
    Gestione spawn dei nemici e dei bonus e ritoccare costruttore di item
    Aggiustare sezioni drill case 3
    collisione proiettile proiettile
    (Cambiare nome al booleano di enemy: alive) 
    
    
Funzioni che interagiscono con proiettili

- XbulletCollision(int x, int y)
- XgenerateBullet(bool direction, cast shooter)
- bulletMove()
- deleteNodes()
- XaddToList(bulletNode* newNode)
- findAmmo(int x, int y) (ricontrallere bene questa)
- enemyCollision(enemyNode* currentEnemy)
- enemyMove()

### Miglioramenti:
    Generazione buchi nelle piattaforme
    Autocolpirsi spostandosi subito dopo aver sparato
    Per ora i nemici devono avere una singola vita altrimenti succedono cose strane. Risolvere o lasciare così
    Rimuovere nodi da liste(possiamo pulire le liste al cambio di livello)
    Caduta personaggio caso buco 
    Schermata Iniziale
    Schermata Finale mostrando score finale
    scivere relazione e registrare video
    commentare le funzioni
