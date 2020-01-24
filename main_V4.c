/*
    Traccia: 

    Si vuole sviluppare un programma per la simulazione del gioco del tennis.
    Supponiamo di avere un giocatore che gioca contro un simulatore. Il simulatore
    lancia la palla che deve essere respinta dal giocatore.
    Il giocatore (rettangolo rosso) viene collocato in una mappa regolare di dimensioni
    come in figura. Il giocatore ha a disposizione una racchetta (nero in figura). Il
    simulatore fa un punto se il giocatore non riesce a respingere la palla altrimenti se il
    ricevitore colpisce la palla il punto va al giocatore.

    Il gioco procede in questo modo:
    • Il simulatore lancia la palla con una certa intensità che è identificata da un
    numero pseudocasuale da 1 a 10.
    • Se la palla cade nella cella arancione questa viene respinta dal battitore che
    fa un punto.
    • Se la palla cade in una cella gialla il punto va al battitore se l’intensità del
    tiro è minore di 3 altrimenti il punto va al simulatore.
    • Se la palla ricade in una zona azzurra il punto va al giocatore.
    • La palla non può essere mai lanciata sul giocatore.
    Simulare 100 volte la gara ognuna composta da 500 lanci. Il giocatore vince se
    respinge almeno l’80% dei lanci. Stampare la statistica delle vittorie. 
    Effettuare almeno 5 test, visualizzando lo schema come presentato in figura. 
*/

/*
    Finalità:

    Le finalità del programma sono quelle di simulare 100 partite di tennis. Ogni partita 
    comprende la simulazione di 500 lanci, per cui verranno effettuate un totale di 50000 
    simulazioni. 

    La simulazione consiste nel lancio di una palla da tennis (effettuato dal simulatore)
    nel campo del giocatore che ha una posizione fissa e ben determinata all'interno della 
    sua parte di campo. Il problema non richiede di considerare eventuali fuori campo, rete, 
    o altre condizioni ed elementi presenti nel gioco svolto nella vita reale.

    La situazione è molto banale ovvero ci sono almeno 3 situazioni tipiche che possono
    verificarsi:

        1) La palla lanciata dal simulatore colpisce una zona azzurra. Il punto va al giocatore.
        2) La palla viene lanciata in una zona arancione. Il punto va al giocatore.
        3) La palla viene lanciata in una zona gialla. Il punto va al giocatore solo se
        l'intensità o potenza con cui è stata lanciata la palla è inferiore a 3. 

    Già da queste 3 considerazioni iniziali, si può denotare che le probabilità di vincita del
    giocatore ad occhio dovrebbero essere molto elevate. Intorno al 70-80%. Questo perchè il
    simulatore può far punto solo in un determinato caso ben specifico: il suo tiro deve andare 
    in una zona gialla e con una potenza pari o maggiore a 3.

    Ho gestito il campo creando una matrice in cui ad ogni cella è associato un codice
    identificativo che va da 0 a 16. Ho fatto questa scelta in quanto il problema asserisce
    in maniera netta che la palla non può MAI essere lanciata sul giocatore, quindi all'interno
    della generazione del numero pseudocasuale (o della cella in cui deve essere lanciata la palla), 
    i codici relativi alle date celle rosse, non devono neppure comparire. Proprio per questo,
    benchè le celle del campo rappresentato in figura siano 20, userò solo 17 codici identificativi
    (0-16) per rappresentare il campo, in quanto le 3 celle del giocatore non van considerate.

    Qui di seguito riporto come ho deciso di organizzare il campo da tennis nella matrice:
    
    Identificatori Posizioni Campo
    ______________________
    | 0   1   2   3   4  |
    | 5  11   X  13   8  |   
    | 6  15   X  16   9  |   
    | 7  12   X  14  10  |   
    |____________________|

    Il programma funziona in questo modo:
        - Si genera un seed per i numeri pseudocasuali
        - Si imposta una variabile di scelta in modo che l'utente possa scegliere se eseguire
        altre simulazioni o meno.
        - Si avvia la simulazione dei 100 match. Questo è quello che accade per ogni match:
            - Reset del campo e pulizia dei risultati globali che verranno stampati a fine simulazione
            - Visualizzazione del campo da tennis e inizio della simulazione
            - All'interno di un for che va 0 a 99, si vanno a pulire i dati del match i-esimo e si assegna
            un ID al match
                - Si entra in un altro ciclo for (annidato a quello precedente) in cui si assegna un ID anche
                al j-esimo lancio dell'i-esimo match della simulazione
                - Generazione del lancio che comprende a sua volta: generazione della potenza del lancio, 
                della zona dove deve essere lanciata la palla e determinazione della zona in base agli indici
                di riga e colonna
                - Determinazione del lancio che consiste nel determinare in che zona del campo è andata la palla
                e se andata nella zona gialla, con quale potenza. Inoltre a seconda della zona viene incrementato 
                di volta in volta il contatore che riporta i punti del giocatore o quello che porta i punti del
                simulatore. infine viene aumentato il numero di colpi che ha subito quella zona e quella determinata
                cella (verranno poi sfruttati a fine match questi dati).
            - Usciti dal ciclo annidato, si passa alla determinazione del match e quindi nel determinare se il 
            giocatore abbia perso o vinto a seconda se ha fatto una percentuale inferiore o maggiore uguale all'80%
            - Fatto ciò si aggiornano i punteggi globali del simulatore e del giocatore
        - Usciti anche dal ciclo for più esterno, allora si passa alla stampa dei risultati singoli per ogni 
        simulazione e poi globali di tutte le simulazioni insieme.
        - A questo punto si passa a chiedere all'utente se vuole eseguire un'altra simulazione, in caso affermativo
        viene eseguita altrimenti il programma termina. L'interazione con l'utente viene gestita con un preciso
        controllo sull'errore.

        Nota Bene: La descrizione dei parametri di ogni funzione è abbastanza inutile da fare dal momento che vengono
        passati sempre dei puntatori a variabile di tipo struttura. Di conseguenza non andrò a menzionarli ogni volta.
*/

/*
    Librerie Sfruttate:

    - stdio.h per l'I/O
    - stdlib.h e time.h per i numeri pseudocasuali
    - string.h per gestire alcune stringhe usate durante il corso del programma e per sfruttare la strcpy
    - windows.h per la gestione e l'implementazione dei colori (campo da tennis colorato)

    Costanti:

    - nThrows, numero Lanci
    - nMatches, numero Match
    - rows, numero righe campo
    - cols, numero colonne campo
    - true e false, per simulare il comportamento di una variabile booleana
    - nPosition, numero delle celle che possono essere colpite all'interno del campo da tennis

    Tipi Derivati Dichiarati:

    - Bool, per simulare il comportamento di una variabile booleana
    - Score e LongScore, per contenere rispettivamente il numero di punti per ogni match e quelli globali
    - Position, per ospitare l'indice associato alla cella della matrice
    - Power, la potenza del tiro effettuato dal simulatore
    - ID, per rappresentare tutte le variabili di tipo codice identificativo sfruttate nel programma
    - PitchZone, che identifica uno dei possibili valori che può assumere una variabile in base all'enum
    per andare a determinare la zona del campo
    - Color, per contenere i colori

    Enum Utilizzati:

    - zoneGroups, rappresenta le zone che possono essere rappresentate all'interno del campo
    - ConsoleColor, i colori con i codici per rappresentarli in C

    Array Globali Usati:

    - zoneNames, contiene i nomi sotto forma di stringa delle zone del campo
    - positionM, contiene l'indice di riga associato ad ogni ID in maniera progressiva
    - positionN, contiene l'indice di colonna associato ad ogni ID in maniera progressiva

    Struct Sfruttate:

    - simulation, usata per contenere i dati globali delle 100 simulazioni
    - match, per contenere i dati e le statistiche relative al singolo match
    - throw, per contenere i dati del singolo lancio
    - zone, per rappresentare un'entità che fosse formata da un ID che identificcasse la posizione,
    un indice di riga, uno di colonna, il nome della zona espresso come stringa e l'associazione
    della sua zona in relazione all'enum prima descritto.
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>

#define nThrows 500
#define nMatches 100
#define rows 4
#define cols 5
#define FALSE 0
#define TRUE 1
#define nPosition 17

enum zoneGroups { Azure, Yellow, Orange, Red, Ball };
char *zoneNames[] = { "Azure\0", "Yellow\0", "Orange\0", "Red\0" };

enum ConsoleColors {
   WhiteFore   = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
   BlackBack   =    0,
   YellowLow    = 224,
   YellowHigh   = 111,
   RedLow       = 192,
   RedHigh      =  79,
   AzureLow     = 176,
   AzureHigh    =  63,
   OrangeHigh   = 207, 
};

typedef unsigned short int Bool;
typedef unsigned short int Score;
typedef unsigned short int Position;
typedef unsigned short int Power;
typedef unsigned short int ID;
typedef unsigned int LongScore;
typedef enum zoneGroups PitchZone;
typedef enum ConsoleColors Color;

// #Struct

struct zone {
    ID positionID;
    Position m;
    Position n;
    char zoneName[7];
    PitchZone zoneGroup;
};
typedef struct zone Zone;

struct throw {
    ID throwID;
    Power power;
    Bool highPower;
    Zone hitZone;
};
typedef struct throw Throw;

struct match {
    ID matchID;
    Score playerScore;
    Score simulatorScore;
    float playerRateWin;
    Bool playerWin;
    Throw throws[nThrows];

    ID mostAffectedPosition;
    unsigned short int maxHitPosition;
    Position hitCountPosition[nPosition];

    ID mostAffectedZone;
    unsigned short int maxHitZone;
    Position hitCountZone[3];
};
typedef struct match Match;

struct simulation {
    Match matches[100];
    LongScore globalPlayerScore;
    LongScore globalSimulatorScore;
    float globalPlayerRateWin;
};
typedef struct simulation Simulation;

Position positionM[nPosition] = { 0, 0, 0, 0, 0, 1, 2, 3, 1, 2, 3, 1, 3, 1, 3, 2, 2 };
Position positionN[nPosition] = { 0, 1, 2, 3, 4, 0, 0, 0, 4, 4, 4, 1, 1, 3, 3, 1, 3 };

// #Procedure

void startSimulation(Simulation *);

void setMatrix(int *);
void getMatrix(int *);

void generateThrow(Throw *);
void generatePower(Throw *);
void getThrow(Throw *);
void determineThrow(Match *, Throw *);

void incrementPlayerScore(Match *);
void incrementSimulatorScore(Match *);

void generateZone(Zone *);
void determineZone(Zone *);

void clearMatch(Match *);
void determineMatch(Match *);

void getResult(int *, Match *);
void clearGlobalResult(Simulation *);
void getGlobalResult(Simulation *);
void updateGlobalResult(Simulation *, Match *);

void nop();
void setColor(unsigned short int);


// #Main

void main() {
    //Generazione del Seed
    srand((unsigned int) time(0));
    Bool userContinue = FALSE;

    do {
        Simulation newSimulation;
        startSimulation(&newSimulation);

        do {
            printf("\n\nVuoi eseguire un'altra simulazione? (0 == No || 1 == Si)\nScegli: ");
            //Controllo sull'errore dell'input dell'utente
            if(scanf("%hu", &userContinue) != 1) {
                fflush(stdin);
                userContinue = 2;
            }
        } while(userContinue != TRUE && userContinue != FALSE);
    } while(userContinue);
    printf("\n\nArrivederci :D\n\n");
    system("pause");
}

/*
    startSimulation è la procedura da cui parte tutto il processo di simulazione. Si occupa di generare e simulare
    100 match, ciascuno composto da 500 lanci ciascuno. Dopo aver generato singolarmente i lanci, ne determina i 
    risultati. Dopodichè determina i risultati del match e aggiorna le statistiche globali.

    Infine si passa a stampare queste statistiche e informazioni. Gli viene passato come parametro un puntatore alla
    variabile di tipo struct Simulation dichiarata nel main, in questo modo vado a lavorare direttamente sugli 
    indirizzi.
*/

void startSimulation(Simulation *newSimulation) {
    /*
        - Dichiarazione Matrice
        - Pulizia Statistiche Globali
        - Stampa Iniziale del Campo e briefing con l'utente
    */
    int matrix[rows][cols];
    setMatrix(&matrix[0][0]);
    clearGlobalResult(newSimulation);
    
    printf("\n\n\t\t\t\t***Campo da Tennis***\n\n");
    getMatrix(&matrix[0][0]);
    printf("\n\nQuesto e' il campo, ora inziera' la simulazione. Una simulazione comprende 100 partite \ne ogni partita comprende 500 lanci. Per ogni parita verranno visualizzati i risultati, \nle statistiche e le percentuali della partita. Alla fine ci sara' una statistice generale.\n\n");
    system("pause");
    fflush(stdin);

    for(int i = 0; i < nMatches; i++) {
        clearMatch(&newSimulation->matches[i]); //Pulizia dati Match
        newSimulation->matches[i].matchID = (i + 1); //Assegnazione ID Match

        for(int j = 0; j < nThrows; j++) {
            newSimulation->matches[i].throws[j].throwID = (j + 1); //Assegnazione ID Lancio
            generateThrow(&newSimulation->matches[i].throws[j]); //Generazione Lancio
            determineThrow(&newSimulation->matches[i], &newSimulation->matches[i].throws[j]); //Determina dell'esito del lancio
        }

        determineMatch(&newSimulation->matches[i]); //Determina dell'esito del match
        updateGlobalResult(newSimulation, &newSimulation->matches[i]); //Aggiornamento delle statistiche globali
    }

    for(int i = 0;i < nMatches; i++) {
        setMatrix(&matrix[0][0]); //Set Matrice con valori iniziali per ogni lancio
        getResult(&matrix[0][0], &newSimulation->matches[i]); //Stampa dei Risultati del singolo Match
    }
    
    getGlobalResult(newSimulation); //Stampa dei Risultati Globali
}



// #GestioneMatrice

/*
    setMatrix non fa altro che prendere la matrice in input e settarla in base al modello definito nei commenti
    iniziali in modo da determinare le varie zone in base al codice identificativo associato a ciascuna cella.

    Viene sfruttato l'enum in modo da rendere più leggibile il codice e si va ad operare minuziosamente su 
    gli indici di riga e di colonna per settare al meglio la matrice, in vista delle operazioni future.
*/

void setMatrix(int *matrix){
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(i == 0 || j == 0 || j == cols - 1) {
                *(matrix + (cols * i) + j) = Azure;
            } else if (((j == 1) || (j == 3)) && ((i == 1) || (i == 3))) {
                *(matrix + (cols * i) + j) = Yellow;
            } else if ((j == 1 || j == 3) && i== 2) {
                *(matrix + (cols * i) + j) = Orange;
            } else {
                *(matrix + (cols * i) + j) = Red;
            }
        }
    }
}

/*
    getMatrix invece si occupa di stampare la matrice. Ma non solo stampare, infatti sfrutta i colori e la
    libreria windows.h in modo da settarli con precisione e generare un campo abbastanza fedele a quello
    proposto in figura nella traccia.
*/

void getMatrix(int *matrix) {
    for(int i = 0; i < rows; i++) {
        printf("\t\t\t");
        for(int j = 0; j < cols; j++) {
            switch(*(matrix + (cols * i) + j)) {
                case Azure: 
                    setColor(AzureHigh);
                    printf("%6c", 'A');
                    break;
                case Yellow:
                    setColor(YellowHigh);
                    printf("%6c", 'Y');
                    break;
                case Orange:
                    setColor(OrangeHigh);
                    printf("%6c", 'O');
                    break;
                case Red:
                    setColor(RedHigh);
                    printf("%6c", 'R');
                    break;
                case Ball:
                    printf("%6c", 'B');
                    break;
                default:
                    printf("\nError #1\n");
                    break;
            }
            setColor(BlackBack);
            setColor(WhiteFore);
        }
        printf("\n");
    }
    printf("\n");
}



// #GestioneLancio

/*
    generateThrow si occupa di generare un singolo lancio. Per farlo chiama altre 3 procedure:
        - la prima per generare la potenza
        - la seconda per generare la posizione dove deve essere indirizzata la palla e settare zona
        - la terza per settare indice di riga e di colonna della matrice in corrispondenza della cella
        che deve essere colpita
*/

void generateThrow(Throw *throw) {
    generatePower(throw);
    generateZone(&throw->hitZone);
    determineZone(&throw->hitZone);
}

/*
    generatePower si occupa solo di generare la potenza con un numero pseudocasuale e di andare a 
    determinare se la potenza è alta oppure no, a seconda se sia maggiore uguale a 3 o inferiore.
*/

void generatePower(Throw *throw) {
    throw->power = 1 + rand() % 10;
    throw->highPower = throw->power >= 3 ? TRUE : FALSE;
}

/* 
    getThrow permette di andare a visualizzare i dettagli del singolo lancio appena compiuto.
    Questa funzione non viene effettivamente sfruttata dal programma com'è impostato attualmente,
    ma è stata ampiamente sfruttata in fase di debug e test, quindi mi sembrava poco opportuno 
    non renderle merito.
*/

void getThrow(Throw *throw) {
    printf("\n\n***Risultati Lancio Numero %hu***\n\n", throw->throwID);
    throw->highPower ? printf("High Power\n") : printf("Low Power\n");
    printf("Power: %hu\nZone: %s\n M: %hu\nN: %hu\nPosition ID: %hu\n", throw->power, throw->hitZone.zoneName, throw->hitZone.m, throw->hitZone.n, throw->hitZone.positionID);
}

/*
    determineThrow si occupa invece di andare a incrementare a seconda della zona colpita e della
    potenza del tiro generato, i punti del simulatore o del giocatore. Infine incrementa il numero
    di hit che ha avuto una determinata zona e una determinata casella del campo da tennis.
*/

void determineThrow(Match *match, Throw *throw) {
    if(throw->hitZone.positionID > 10 && throw->hitZone.positionID <= 14) {
        throw->highPower ? incrementSimulatorScore(match) : incrementPlayerScore(match); 
    } else {
        incrementPlayerScore(match);
    }

    match->hitCountZone[throw->hitZone.zoneGroup] += 1;
    match->hitCountPosition[throw->hitZone.positionID] += 1;
}

/*
    incrementPlayerScore incrementa il punteggio del giocatore
*/

void incrementPlayerScore(Match *match) {
    match->playerScore += 1;
}

/*
    incrementSimulatorScore incrementa il punteggio del simulatore
*/

void incrementSimulatorScore(Match *match) {
    match->simulatorScore += 1;
}



// #GestioneZona

/*
    generateZone si occupa invece di generare la posizione dove deve andare la palla e di
    settare di conseguenza la zona interessata a quella determinata posizione. 
    Infine copia il nome della zona in un campo della struct zona.
*/

void generateZone(Zone *zone) {
    ID position = rand() % nPosition;
    zone->positionID = position;

    if(position <= 10) {
        zone->zoneGroup = Azure;
    } else if(position > 10 && position <= 14) {
        zone->zoneGroup = Yellow;
    } else {
        zone->zoneGroup = Orange;
    }

    strcpy(zone->zoneName, zoneNames[zone->zoneGroup]);
}

/*
    determineZone invece setta l'indice di riga e di colonna della matrice dove è andata la palla
    in base al valore contenuto prima alla posizione positionID dell'array positionM e poi l'indice di
    colonna in bassa al valore contenuto alla posizione positionID dell'array positionN.
*/

void determineZone(Zone *zone) {
    zone->m = positionM[zone->positionID];
    zone->n = positionN[zone->positionID];
}



// #GestioneMatch

/*
    clearMatch pulisce i dati del match passatogli come parametro
*/

void clearMatch(Match *match) {
    match->simulatorScore = 0;
    match->playerScore = 0;
    match->playerWin = 0;
    match->playerRateWin = 0.00f;

    for(int i = 0; i < nPosition; i++) {
        match->hitCountPosition[i] = 0;
    }

    for(int i = 0; i < 3; i++) {
        match->hitCountZone[i] = 0;
    }
}

/*
    determineMatch invece si occupa di andare a determinare se il giocatore ha vinto ho perso, in base
    a una determina percentuale che viene calcolata nella prima istruzione di questa procedura.
*/

void determineMatch(Match *match) {
    match->playerRateWin = (100.0 * match->playerScore) / nThrows;
    match->playerWin = match->playerRateWin >= 80.0f ? TRUE : FALSE;
}



// #Statistiche e Risultati

/*
    getResult prende in input un puntatore della matrice e un puntatore della variabile di tipo
    struct Match e quello che fa è generarne dei risultati in termini di dati, statistiche e informazioni
    richieste in via generale dalla traccia, come per esempio le vittorie del match, le sconfitte, la 
    percentuale di vittorie e così via.

    Inoltre indica anche qual è stata la zona più colpita del campo e la cella più colpita del campo,
    rendendo il tutto in maniera visuale tramite la visualizzazione del campo stesso con una B che indica
    la cella più colpita.
*/

void getResult(int *matrix, Match *match) {
    unsigned short int maxHitZone;
    printf("\n\n\n\t\t\t***Risultati Match Numero %hu***\n\n", match->matchID);
    printf("Player Score: %hu\nSimulator Score: %hu\nPlayer Rate Win: %0.2f %%\n", match->playerScore, match->simulatorScore, match->playerRateWin);
    match->playerWin ? printf("Player Won\n\n") : printf("Player Lost\n\n");

    //Ricerca della cella colpita maggiormente durnte il Match
    match->maxHitPosition = match->hitCountPosition[0];

    for(int i = 1; i < nPosition; i++) {
        match->hitCountPosition[i] > match->maxHitPosition ? match->mostAffectedPosition = i : nop();
    }
      
    printf("Azure Launches: %hu\nYellow Launces: %hu\nOrange Launches: %hu\n", match->hitCountZone[0], match->hitCountZone[1], match->hitCountZone[2]);

    //Ricerca della Zona colpita maggiormente durante il Match
    match->maxHitZone = match->hitCountZone[0];
    
    for(int i = 1; i < 3; i++) {
        if(match->hitCountZone[i] > match->maxHitZone) {
            match->mostAffectedZone = i;
        }
    }

    match->maxHitPosition = match->hitCountPosition[match->mostAffectedPosition];
    match->maxHitZone = match->hitCountZone[match->mostAffectedZone];

    /*
        Determina dove si trova la cella maggiormente colpita in termini di riga e di colonna 
        e visualizzazione della matrice (campo da tennis)
    */

    Throw tempThrow;
    tempThrow.hitZone.positionID = match->mostAffectedPosition;
    determineZone(&tempThrow.hitZone);
    *(matrix + (tempThrow.hitZone.m * cols) + tempThrow.hitZone.n) = Ball;
   
    printf("Most Hitted Zone: %s\n", zoneNames[match->mostAffectedZone]);
    printf("\n\t\t\t***Most Affected Position (see B)***\n\n");
    getMatrix(matrix); 
    printf("____________________________________________________________________________\n");
}

/*
    clearGlobalResult pulisce i risultati (stats) globali
*/

void clearGlobalResult(Simulation *newSimulation) {
    newSimulation->globalPlayerRateWin = 0.00f;
    newSimulation->globalPlayerScore = 0;
    newSimulation->globalSimulatorScore = 0;
}

/*
    getGlobalResult si occupa di stampare i risultati globali di tutti i match
*/

void getGlobalResult(Simulation *newSimulation) {
    newSimulation->globalPlayerRateWin = (100.0 * newSimulation->globalPlayerScore) / (nMatches * nThrows);

    printf("\n\n\t***Risultati Generali 100 Simulazioni da 500 lanci ciascuna***\n\n");
    printf("Player Global Score: %u\nSimulator Global Score: %u\nGlobal Player' Rate Win Percentage: %0.2f %%\n\n", newSimulation->globalPlayerScore, newSimulation->globalSimulatorScore, newSimulation->globalPlayerRateWin);
}

/*
    updateGlobalResult si occupa di aggiornare match dopo match i risultati (o stats) globali
*/

void updateGlobalResult(Simulation *newSimulation, Match *match) {
    newSimulation->globalPlayerScore += match->playerScore;
    newSimulation->globalSimulatorScore += match->simulatorScore;
}



// #AltreFunzioni

/*
    nop corrisponde a No Operation. Viene sfruttata quando nella realizzazione di un if su una sola riga, 
    la clausola else non può restare vuota e lasciata a sè stante.
*/

void nop() {
    return; 
}

/*
    setColor in ultima analisi setta il colore della console in modo da rendere la visualizzazione del campo
    colorata e abbastanza corrispondente a quella che è la visione proposta dalla traccia
*/

void setColor(unsigned short int color) {
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, color);
}

/*    
_________________________________________________________________________________________________________________________________
|                                                                                                                               |
|                                                        All rights reserved                                                    |
|                                                        Made by: @dennewbie                                                    |
|                                        Contact: https://www.linkedin.com/in/denny-caruso/                                     |
|                                                                                                                               |
|_______________________________________________________________________________________________________________________________|
*/