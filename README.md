### Operation Antarctica - Progetto Programmazione 2020-2021

### Compilare
  In questo progetto abbiamo usato Visual Studio Code e le librerie windows.h e conio.h.  
  Abbiamo incluso le nostre impostazioni, ma nel caso le vogliate usare, probabilmente, dovrete modificare:  
  - launch.json -> configurations -> miDebuggerPath  
  - tasks.json -> tasks -> command

>_>Player?  
>_>...  
>_>Player mi senti?  
>_>...  
>_>Player siamo vicini alla base nemica 211, ma la bufera non ci permette di recuperarti. Abbiamo sottovalutato l’Antartide.  
>_>...  
>_>Mi dispiace ma da ora sei solo, e sappiamo che in giro sono rimaste numerose truppe Maziste. Fatti valere, buona fortuna soldato.  
>_>...  

### Comandi
- WASD: Movimento
- J: sparo a sinistra
- K: sparo a destra
- P: pausa

### Meccaniche
Player avanza nelle varie stanze attraverso le porte affrontando i diversi Nemici che incontra e raccogliendo Bonus.

I Nemici sono di due tipi:

- M: Mazista ordinario, pattuglia la sua zona, a volte si muovono in coppia.
- T: Mazista armato di Torretta, è sempre solo e continua a sparare nella direzione di Player.

I Bonus sono di tre tipi:

- ♥: Kit Medico, Player ha un massimo di tre vite, che perde imbattendosi nei Mazisti e venendo colpito da un proiettile, e recupera con i Kit Medici.
- ©: Coin, Player ottiene punti aggiuntivi trovando Segreti Militari nascosti in giro per la base 211.
- ®: Magazine, Player recupera munizioni.

Player ha due modi di attaccare i Mazisti: sparare e colpire dall’alto.

I Bonus colpiti dai proiettili sono distrutti per sempre senza ottenere la ricompensa.
