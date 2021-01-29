#include "functions.h"
/*In questo file sono presenti le funzioni*/

/*******NUOVA PARTITA*****/
/**DECK**/
/*Funzione che controlla la presenza di salvataggi e chiede se si vuole iniziare una nuova partita o caricarne una
chiama all'interno di se la funzione sceltaDifficoltà e resitutisce il valore della difficoltà scelta dall'utente*/
_Bool nuovaPartita(){
    _Bool load;
    int choise;
   do{
        printf("- Vuoi iniziare una nuova partita o caricarne una precedentemente salvata?\n");
        printf("1. Nuova Partita\n");
        printf("2. Carica Partita\n");
        scanf("%d", &choise);
        getchar();
        if(choise == 2){
            load = true; //Carica partita
        }
        else{
            load = false; //Nuova partita
        }
   }while(choise == 0 || choise > 2);
    return load;
}

/*Funzione che permette al giocatore di scegliere la difficoltà e di restituirla alla funzione che la chiama in questo caso nuova partita*/
int sceltaDifficolta(){
    int difficulty;
    do{
        printf("- Scegli la difficolta':\nEasy = 0\tMedium = 1\tHard = 2\t");
        scanf("%d", &difficulty);
        getchar();
    }while(difficulty > 2);
        switch (difficulty){ //case che gestisce la scelta comunicandola al giocatore
    case 0:
        printf("\nHai scelto Easy!\n");
        break;
    case 1:
        printf("\nHai scelto Medium!\n");
        break;
    case 2:
        printf("\nHai scelto Hard!\n");
        break;
    }
    return difficulty;
}

/*Funzione che gestisce la creazioone del deck in base alla scelta della difficoltà, chiama funzioni che partizionano il deck e danno le carte
restituendo come valore di ritorno il deck dopo essere stato riassemblato e mischiato*/
Card* chosenDeck(Card* deck, int difficulty, Player hPlayer[]){
    int nExploding, nMeooow, nDjanniCards, i;
    int nDjanniLeft, nMeooowLeft, totalCardsLeft;
    Card* explodingDjanni=NULL; //partizione solo exploding
    Card* meooow=NULL; //partizione del deck contenente solo carte MEOOOW
    Card* djanniCards =NULL; //partizione contentente tutte le altre carte
    FILE * fp = NULL;
    FILE * fp1 = NULL;
    FILE * fp2 = NULL;
        switch (difficulty){ //case che gestisce e apre il file corrispondente alla difficoltà scelta
    case 0:
        fp = fopen("explodingDjanniEasy.txt", "r+"); //easy 8 meooow + 3 exploding + 46 djanni
        break;
    case 1:
        fp = fopen("explodingDjanniMedium.txt", "r+"); //medium 6 meooow + 3 exploding + 46 djanni
        break;
    case 2:
        fp = fopen("explodingDjanniHard.txt", "r+"); //hard 4 meooow + 3 exploding + 46 djanni
        break;
    }
    fscanf(fp, "%d %d %d", &nExploding, &nMeooow, &nDjanniCards); //Estrazione n carte per tipologia
    explodingDjanni = makeExplodingPart(explodingDjanni, fp, nExploding); //Creazione parte exploding
    meooow = makeMeooowPart(meooow, fp, nMeooow); //Creazione partizione di soli meooow e li mescola
    meooow = giveMeooow(meooow, hPlayer, nMeooow); //Da le carte ai giocatori rimuovendole dal deck lo mescola e lo restituisce
    djanniCards = makeDjanniPart(djanniCards, fp, nDjanniCards);  //Creazione partizione con le altre carte
    djanniCards = giveDjanni(djanniCards, hPlayer, nDjanniCards); //Da le carte ai giocatori rimuovendole dal deck lo mescola e lo restituisce
    nMeooowLeft= nMeooow - MEOOOWTOGIVE; //Calcolo dei meooow rimasti dopo aver dato le carte
    nDjanniLeft = nDjanniCards - DJANNITOGIVE; //Calcolo delle altre carte rimaste
    totalCardsLeft= nMeooowLeft + nDjanniLeft + nExploding;
    fp1 = fopen("carterimastedopoassegnazione.txt", "w"); //file in cui verranno scritte le carte rimaste per poter essere ricaricate nel deck
    //Scrittura MEOOOW
    for(i=0; i<nMeooowLeft; i++){
        fprintf(fp1, "%u %s", meooow[i].type, meooow[i].phrase);
        fprintf(fp1, "\n");
    }
    //Scrittura delle altre carte
        for(i=0; i<nDjanniLeft; i++){
        fprintf(fp1, "%u %s", djanniCards[i].type, djanniCards[i].phrase);
        fprintf(fp1, "\n");
    }
    //Scrittura delle exploding
        for(i=0; i<nExploding; i++){
        fprintf(fp1, "%u %s", explodingDjanni[i].type, explodingDjanni[i].phrase);
        fprintf(fp1, "\n");
    }
    fclose(fp1); //Chiusura del file carterimastedopoassegnazione
    deck = (Card*)malloc(totalCardsLeft * sizeof(Card)); //Allocazione del deck con il massimo dei posti
    fp2 = fopen("carterimastedopoassegnazione.txt", "r"); //Riapertura del file per scrivere nel deck
        for(i=0; i<totalCardsLeft; i++){
        fscanf(fp2, "%u %[^\n]s", &deck[i].type, deck[i].phrase);
        fscanf(fp2, "\n");
    }
    fclose(fp2); //Chiusura del file carterimastedopoassegnazione
    #if DEBUG_MODE == 0
    if(deck != NULL){
    printArray(deck, totalCardsLeft);
    }
    #endif
    return deck;
}
/*Funzione che crea la parte exploding ricevendo il file, non lo chiude in modo da poter essere ripreso dalle successive funzioni
per l'estrappolo delle partizioni iniziali*/
Card* makeExplodingPart(Card *explodingDjanni, FILE* fp, int nExploding){
    int i;
    explodingDjanni = (Card*)malloc(nExploding * sizeof(Card)); //allocazione deck contente tutte le carte (deck in game)
    for(i=0; i<nExploding; i++){
        fscanf(fp, "%u %[^\n]s", &explodingDjanni[i].type, explodingDjanni[i].phrase);
        fscanf(fp , "\n");
    }
    //Il file rimane aperto
    #if DEBUG_MODE == 0
    printArray(explodingDjanni, nExploding);
    #endif
    return explodingDjanni;
}
/*Funzione che crea una partizione iniziale (prima dell'assegnazione delle carte) con tutte le MEOOOW, per fare ciò
sfrutta il file passato dalla funzione chiamante non ancora chiuso e posizionato alla prima MEOOOW*/
Card* makeMeooowPart(Card* meooow, FILE* fp, int nMeooow){ //partition EM = EXPLODING + MEOOOW
    int i;
    meooow = (Card*)malloc(nMeooow * sizeof(Card)); //allocazione deck contente tutte le carte (deck in game)
    if(meooow == NULL){ //controllo per verificare che l'allocazione sia andata a buon fine
        exit(-1);
    }
    for(i=0; i<nMeooow; i++){ //DA VEDERE SE SI PUO RIMUOVERE
        fscanf(fp, "%u %[^\n]s", &meooow[i].type, meooow[i].phrase);
        fscanf(fp , "\n");
    }
    deckShuffle(meooow, nMeooow);
    //Il file rimane aperto
    #if DEBUG_MODE == 0
    printArray(meooow, nMeooow);
    #endif
    return meooow;
}

/*Funzione che crea una partizione iniziale (prima dell'assegnazione delle carte) con tutte le djanni, per fare ciò
sfrutta il file passato dalla funzione chiamante non ancora chiuso e posizionato alla prima djanni*/
Card* makeDjanniPart(Card* djanniCards, FILE* fp, int nDjanniCards){
    int i;
    djanniCards = (Card*)malloc(nDjanniCards * sizeof(Card)); //allocazione deck contente tutte le carte (deck in game)
    if(djanniCards == NULL){ //controllo per verificare che l'allocazione sia andata a buon fine
        exit(-1);
    }
    for(i=0; i<nDjanniCards; i++){
        fscanf(fp, "%u %[^\n]s", &djanniCards[i].type, djanniCards[i].phrase);
        fscanf(fp , "\n");
    }
    fclose(fp);
    deckShuffle(djanniCards, nDjanniCards);
    #if DEBUG_MODE == 0
    printArray(djanniCards, nDjanniCards);
    #endif
    return djanniCards;
}

/*Funzione che assegna un MEOOOW per ogni giocatore e con l'aiuto dei file txt scrive i meooow rimasti in una partizione
Card* che renderà al chiamante */
Card * giveMeooow(Card* meooow,Player hPlayer[], int nMeooow){
    int i, j=0, nMeooowtogive=4;
    int nMeooowLeft = nMeooow - nMeooowtogive;
    FILE* fp=NULL, *fp1=NULL;
            for(i=0; i<NPLAYERS; i++){
                hPlayer[i].hand = (Card*)malloc(1 * sizeof(Card));
                hPlayer[i].hand[0] = meooow[j];
                hPlayer[i].nHand++;
                j++;
            }
    fp= fopen("meooowdopoassegnazione.txt", "w"); //Scrive nel file text
        for(i=3; i<nMeooowLeft; i++){
        fprintf(fp1, "%u %s", meooow[i].type, meooow[i].phrase);
        fprintf(fp1, "\n");
    }
    fclose(fp);
    fp1= fopen("meooowdopoassegnazione.txt", "r");
    for(i=0; i<nMeooowLeft; i++){
        fscanf(fp1, "%u %[^\n]s", &meooow[i].type, meooow[i].phrase); //Carica la partizione dal file all'array
        fscanf(fp1, "\n");
    }
    return meooow; //Ritorna la partizione di meooow rimasti
}

/*Funzione che assegna 4 Djanni per ogni giocatore e con l'aiuto dei file txt scrive i Djanni rimasti in una partizione
Card* che renderà al chiamante */
Card * giveDjanni(Card* djanniCards,Player hPlayer[], int nDjanniCards){
    int i, j, k=0;
    int nDjanniLeft = nDjanniCards - DJANNITOGIVE;
    FILE* fp1=NULL;
    FILE* fp =NULL;
        for(i=0; i<NPLAYERS; i++){//Scorrimento dei player
                hPlayer[i].hand=(Card*)realloc(hPlayer[i].hand,5*sizeof(Card)); //Reallocazione della mano per ogni player
            for(j=1; j<5; j++){ //L'indice scorre da 1 a 5 perchè l'indice 0 della mano di ogni player è già occupato dalla carta MEOOOW
                hPlayer[i].hand[j] = djanniCards[k];
                k++; //Scorrimento del mazzo
                hPlayer[i].nHand++; //Aumento del n carte in mano
            }
        }
    fp= fopen("djannidopoassegnazione.txt", "w");
        for(i=k; i<nDjanniCards; i++){ //Parte dalla prima carta rimasta fino all'ultima e scrive nel file txt
        fprintf(fp, "%u %s", djanniCards[i].type, djanniCards[i].phrase);
        fprintf(fp, "\n");
    }
    fclose(fp);
    fp1 = fopen("djannidopoassegnazione.txt", "r"); //Carica dal file txt
        for(i=0; i<nDjanniLeft; i++){
            fscanf(fp1, "%u %s", &djanniCards[i].type, djanniCards[i].phrase);
            fscanf(fp1, "\n");
    }
    return djanniCards; //rende la partizione di sole djanni
}

/*Funzione che data la difficoltà calcola quante carte sono presenti nel deck prima di essere pescata alcuna carta,
è una funzione utile per avere il conto delle carte all'inizio nel main, non registra le variazioni del mazzo*/
int howmanycardsinthedeck(difficulty){
   int nExploding, nMeooow, nDjanniCards, totalCards, cardsInTheDeck, nCardstoGive;
   nCardstoGive = NCARDSTOGIVE1TURN;
   FILE *fp=NULL;
        switch (difficulty){ //case che gestisce e elabora il tipo di deck scelto
    case 0:
        fp = fopen("explodingDjanniEasy.txt", "r+"); //8 meooow 3 exploding 46 djanni
        break;
    case 1:
        fp = fopen("explodingDjanniMedium.txt", "r+"); //6 meooow 3 exploding 46 djanni
        break;
    case 2:
        fp = fopen("explodingDjanniHard.txt", "r+"); //4 meooow 3 exploding 46 djanni
        break;
    }
    fscanf(fp, "%d %d %d", &nExploding, &nMeooow, &nDjanniCards);
    totalCards = nExploding + nMeooow + nDjanniCards; //conteggio del totale carte del deck
    cardsInTheDeck = totalCards - nCardstoGive; //Calcolo delle carte rimaste dopo la prima distribuzione
    fclose(fp);
    return cardsInTheDeck;
}
/**Players**/
/*Riempimento dell'array a Player presente nel main, con le informazioni base di ogni giocartore, ovvero nome, numero carte in mano
carte in mano, tipo di giocatore e booleano ad indicare se il player è vivo*/
void playerCreationH(Player p[], int nHumans){ //funzione che crea i player umani
    int i;
    for(i=0; i<nHumans; i++){
        inputPlayer(p, i); //Ottiene il nome del giocatore
        p[i].alive = true; // Il giocatore è in vita
        p[i].player_type = human; //Il giocatore è umano
        p[i].nHand = 0; //Mano vuota
        p[i].hand = NULL;
    }
}
/*Procedura per l'inserimento del nome del giocatore*/
void inputPlayer(Player p[], int i){
    int c;//Variabile ausiliaria utilizzata per verificare che il nome non abbia superato lo spazio consentito
    printf("\nInserisci il nome del giocatore %d, Max 23 caratteri\n", i+1);
    scanf("%[^\n]s", p[i].name);
    getchar();
    c = strlen(p[i].name); //calcolo della lunghezza stringa inserita
    /*Messaggio di errore e richiamo funzione nel caso l'utente inserisca un nome troppo lungo*/
    if(c > DIM_N){ //Nel caso l'utente inserisse un numero maggiore a 23 ovvero la dimensione massima per il nome gli verrà chiesto di rinserire il nome
        printf("Il nome supera il limite massimo di caratteri!\n");
        inputPlayer(p, i);
    }
}

/*Funzione che decreta tramite una generazione casuale quale giocatore dovrà iniziare e controlla se la partita è stata salvata*/
Infos firstPlayer(void){
    int i;
    Infos infoGame;
    srand ( time(NULL) ); //azzeramento seed
    i= rand()%4;
    infoGame.playerTurn = i; //Player scelto dalla sorte
    infoGame.save=false;
    return infoGame;
}

/****Carica/Salva****/
/*Funzione che permette di estrappolare da un file sav di salvataggio i vari valori dei player, carte rimaste, deck, e info
nell'ordine di salvataggio*/
Card* caricaPartita(Card* deck, Player hPlayer[], Infos *gameinfo, int *cardsLeftinthedeck){
    int i, j, cardsLeft;
    Infos gameinfoAux;
    FILE* fp=NULL;
    fp = fopen("partita.sav", "rb");
    if(fp == NULL){
        printf("Impossibile caricare! Non sono presenti salvataggi di precenti partite.\n ");
        exit(0); //Se si prova a caricare senza salvataggi il gioco lo comunica ed esce
    }
    /*Caricamento players*/
    for(i=0;i<NPLAYERS; i++){
        fread(&hPlayer[i].name, sizeof(char), DIM_N+1, fp);
        fread(&hPlayer[i].alive, sizeof(_Bool), 1, fp);
        if(hPlayer[i].alive == true){
            fread(&hPlayer[i].nHand, sizeof(int), 1, fp);
            fread(&hPlayer[i].player_type, sizeof(Type), 1, fp);
                hPlayer[i].hand=(Card*)malloc(hPlayer[i].nHand * sizeof(Card));
                for(j=0; j<hPlayer[i].nHand; j++){
                    fread(&hPlayer[i].hand[j].phrase, sizeof(char), DIM_P+1, fp);
                    fread(&hPlayer[i].hand[j].type, sizeof(CardType), 1, fp);
                }
        }
    }
    /*Caricamento deck*/
    fread(&cardsLeft, sizeof(int), 1, fp);
    deck=(Card*)malloc(cardsLeft * sizeof(Card));
    for(i=0; i<cardsLeft; i++){
        fread(&deck[i].phrase, sizeof(char), DIM_P+1, fp);
        fread(&deck[i].type, sizeof(CardType), 1, fp);
    }
    /*Caricamento info*/
    fread(&gameinfoAux.playerTurn, sizeof(int), 1, fp);
    fread(&gameinfoAux.save, sizeof(_Bool), 1, fp);
    fclose(fp);
    *cardsLeftinthedeck = cardsLeft; //Trasferimento valori nei vettori che puntano alle variabili del main
    *gameinfo = gameinfoAux;
    return deck;
}
/*Procedura che consente di salvare la partita su file binario*/
void saveGame(Card* deck, Player hPlayer[], Infos gameinfo, int cardsLeft){
    int i, j;
    FILE* fp=NULL;
    fp = fopen("partita.sav", "wb");
    if(fp == NULL)
    exit(0);
    /*Caricamento players*/
    for(i=0;i<NPLAYERS; i++){
        fwrite(&hPlayer[i].name, sizeof(char), DIM_N+1, fp);
        fwrite(&hPlayer[i].alive, sizeof(_Bool), 1, fp);
        if(hPlayer[i].alive == true){
            fwrite(&hPlayer[i].nHand, sizeof(int), 1, fp);
            fwrite(&hPlayer[i].player_type, sizeof(Type), 1, fp);
                for(j=0; j<hPlayer[i].nHand; j++){
                    fwrite(&hPlayer[i].hand[j].phrase, sizeof(char), DIM_P+1, fp);
                    fwrite(&hPlayer[i].hand[j].type, sizeof(CardType), 1, fp);
                }
        }
    }
    /*Caricamento deck*/
    fwrite(&cardsLeft, sizeof(int), 1, fp);
    for(i=0; i<cardsLeft; i++){
        fwrite(&deck[i].phrase, sizeof(char), DIM_P+1, fp);
        fwrite(&deck[i].type, sizeof(CardType), 1, fp);
    }
    /*Caricamento info*/
    fwrite(&gameinfo.playerTurn, sizeof(int), 1, fp);
    fwrite(&gameinfo.save, sizeof(_Bool), 1, fp);
    fclose(fp);

}
/***Funzioni in game***/
/*Funzione che scorre il turno */
int nextGuy(int currentP, Player p[], _Bool attack){
    int nextPlayer;
    nextPlayer = currentP;
    if(attack == false){ //Se il giocatore non è sotto attacco scorre
        do{
            if(nextPlayer == 3){
                nextPlayer=0;
            }else{
                nextPlayer++;
            }
        }while(p[nextPlayer].alive == false);
    }
    return nextPlayer;
}

/*Funzione per pescare una carta*/
_Bool drawCard(Card* deck, Player p[], Infos gameInfo, int cardsLeftinthedeck){
    int deckposition;
    _Bool exploding;
    deckposition=cardsLeftinthedeck;
    deckposition--; //Impostazione dell'indice
    int currentP=gameInfo.playerTurn;
    exploding = controllaCarta(deck, deckposition); //Si controlla che la carta pescata non sia un exploding
    if(exploding == false){ // Se non lo è
        p[currentP].nHand++; // + 1 carta in mano
        p[currentP].hand=(Card*)realloc(p[currentP].hand, p[currentP].nHand * sizeof(Card)); //riallocazione della mano
        p[currentP].hand[p[currentP].nHand-1]=deck[deckposition];//Aggiunta della carta pescata
        deck=(Card*)realloc(deck,deckposition * sizeof(Card)); //Riallocazione del deck con un elemento in meno, corrisponde a riallocare e scartare la carta in cima contemporaneamente
    }
    else{ //Se lo è
        explodingDJ(p, p[currentP].nHand, currentP); //Si controlla se il giocatore si può salvare o meno
        deckShuffle(deck, cardsLeftinthedeck);
    }
    return exploding;
}

/*Procedura che controlla che la carta pescata non sia un exploding, se è un exploding offrirà al giocatore la possibilità
di salvarsi scartando una meooow nel caso non lo voglia o non possegga alcuna meooow il giocatore muore*/
void explodingDJ(Player p[], int nHand,int currentP){
    char choise;
    int i;
    _Bool haveMeooow=false;
    for(i=0; i<p[currentP].nHand; i++){ //Controlla se il player possiede meooow
        if(p[currentP].hand[i].type == 1){
            haveMeooow = true;
            break;
        }
    }
    if(haveMeooow == false){ //Se non ha meooow muore a prescindere
        printf("\n-Hai pescato un exploding Djanni! Non possedendo MEOOOW non puoi fare altro che esplodere..\n");
        printf("*%s (p%d) e' esploso!*\n", p[currentP].name, currentP+1);
        p[currentP].alive = false;
        free(p[currentP].hand);
        p[currentP].nHand=0;
    }else
    if(haveMeooow == true){ //Se ha una meooow gli viene chiesto se si vuole utilizzare
        printf("-Hai pescato un exploding Djanni! Sei sull'orlo del baratro, ma ti puoi salvare con una carta MEOOOW, vuoi farlo? Y/N\t");
        scanf("%c", &choise);
        getchar();
        if((choise == 'y') || (choise == 'Y')){
            scartaCarta(p, currentP, i);
            printf("\n");
            printf("%s (p%d) ha scartato MEOOOW!\n", p[currentP].name, currentP+1);
            printf("%s (p%d) ha rimesso EXPLODING DJANNI nel mazzo\n", p[currentP].name, currentP+1);
        }
        else{
            printf("*%s (p%d) e' esploso!*\n", p[currentP].name, currentP+1); //Se dice di no il player muore
            p[currentP].alive = false;
            free(p[currentP].hand);
            p[currentP].nHand=0;
        }

    }
}

/*Procedura che gestisce la giocata di Meooow durante il turno*/
void meooowCase(Player hPlayer[],Infos gameInfo, int indiceCarta){
    _Bool nope;
    nope = checkNope(hPlayer, gameInfo.playerTurn);
        if(nope == false){  //Se nessun nope si oppone si attiva
            printf("%s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
            leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
            printf("\n");
            printf("\n");
            printf("Non succede nulla...\n");
    }
}
/*Procedura che gestisce la giocata di una shuffle*/
void shuffleCase(Player hPlayer[],Infos gameInfo, int indiceCarta, Card* deck, int cardsLeftinthedeck){
    _Bool nope;
    nope = checkNope(hPlayer, gameInfo.playerTurn);
    if(nope == false){  //Se nessun nope si oppone si attiva
        deckShuffle(deck, cardsLeftinthedeck);
        printf("\n%s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name,  gameInfo.playerTurn+1);
        leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
        printf("\n");
        printf("\n");
        printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
    }
}

/*Funzione che controlla se il giocatore successivo ha un nope, in caso non ce l'abbia la funzione esce restituendo false subito
nel caso in cui invece il player successivo abbia un nope, chiede se vuole essere giocato e restituirà il valore di nope
a seconda della scelta del player, true per una scelta positiva, false per negativa. Se la scelta risulta positiva controlla
che il player che ha giocato la carta che deve essere annulata da quel nope ha una nope per controbattere*/
_Bool checkNope(Player p[], int currentP){
    char choise;
    int i;
    _Bool attack=false;
    _Bool nopeHand=false; //Nope posseduto dal player successivo
    _Bool nopevsnope; //Quando due nope si scontrano
    _Bool nopeUsed; //Se il nope è stato attivato con successo
    int nextPlayer;
    nextPlayer = nextGuy(currentP, p, attack);
        for(i=0; i<p[nextPlayer].nHand; i++){
        if(p[nextPlayer].hand[i].type == 3){ //Se il giocatore successivo possiede almeno una nope esce subito dal ciclo e nope prende il valore true
            nopeHand = true;
            break;
        }
    }
        if(nopeHand == false){ //Se non ne possedeva nemmeno una la funzione ritorna al main il valore false di nope
            return nopeHand;
        }

   /*Se il giocatore successivo ha una nope*/
            printf("\n%s (p%d) possiede una NOPE! Vuoi utilizzarla? Y/N\t", p[nextPlayer].name, nextPlayer+1);
            scanf("%c", &choise);
            getchar();
            switch(choise){
        case 'y':
        case 'Y':
            printf("%s\n", p[nextPlayer].hand[i].phrase);
            scartaCarta(p, nextPlayer, i);
            nopeUsed = true; //Se viene utilizzata scarta la carta e restituisce nope con valore true
            nopevsnope = checkNope(p, nextPlayer); //Controlla se il giocatore vittima della nope può e vuole controbattere con un'altra nope
            if(nopevsnope == true){ //Se il giocatore lo fa
                nopeUsed = false;
                return nopeUsed; //La carta giocata all'inizio non viene annullata
            }
            if(nopevsnope == false){ //Se non lo fa
                nopeUsed = true;
                return nopeUsed; //La carta giocata all'inizio viene annullata
            }
            break;
        default:
            nopeUsed = false; //Se non viene utilizzata restituisce false e nessuna carta viene scartata
            printf("\n%s (p%d) ha deciso di non giocare la NOPE!\n", p[nextPlayer].name,nextPlayer+1);
            return nopeUsed;
            }
            return nopeUsed;
}

/*Procedura nel caso in cui un player giochi una nope nel proprio turno*/
void nopeWithNoeff(Player hPlayer[],Infos gameInfo, int indiceCarta){
    _Bool nope;
    nope = checkNope(hPlayer, gameInfo.playerTurn);
    if(nope == true){
        printf("\nLa nope ha fatto il suo sporco lavoro!\n");
    }else
    if(nope == false){
        printf("%s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
        leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
        printf("\n");
        printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
        printf("Ma non succede niente...\n");
    }
}

/*Procedura nel caso venga giocata una see the future*/
void future(Player hPlayer[],Infos gameInfo, int indiceCarta, Card* deck, int cardsLeftinthedeck){
    _Bool nope;
    nope = checkNope(hPlayer, gameInfo.playerTurn);
    if(nope == true){
        printf("\nLa nope ha fatto il suo sporco lavoro!\n");
    }else
    if(nope == false){ //Se nessun nope si oppone si attiva
        printf("\n%s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name,  gameInfo.playerTurn+1);
        leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
        printf("\n");
        printf("\n");
        printf("%s", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
        printf(":\n\n");
        showFirst3(deck, cardsLeftinthedeck);
        printf("\n");
    }
}
/*Procedura nel caso venga giocata una favor card*/
void favorCase(Player hPlayer[],Infos gameInfo, int indiceCarta){
    _Bool nope;
    nope = checkNope(hPlayer, gameInfo.playerTurn); //Controlla se ci sono nope per contestare la giocata
    if(nope == true){
        printf("\nLa nope ha fatto il suo sporco lavoro!\n");
    }else
    if(nope == false){  //Se nessun nope si oppone si attiva
        favorCard(hPlayer, gameInfo.playerTurn);
        printf("%s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name,gameInfo.playerTurn+1);
        leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
        printf("\n");
        printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
    }
}

/*Procedura che gestisce la carta favor permettendo di scegliere a che player rubare e al player vittima di scegliere
la carta da dargli*/
void favorCard(Player p[], int currentP){
    int giocatoreScelto, indiceCarta;
    do{
        printf("A quale giocatore vuoi rubare la carta? (Ricorda che non puoi rubare le carte ai morti..)\t"); //Chiede a chi la usa a chi rubare
        scanf("%d", &giocatoreScelto);
        getchar();
        giocatoreScelto--;
    }while(p[giocatoreScelto].alive == false || giocatoreScelto == currentP || p[giocatoreScelto].nHand == 0); // Non si può rubare una carta ad un giocatore morto
        printf("%s (p%d) scegli:\n", p[giocatoreScelto].name, giocatoreScelto+1); //il player vittima sceglie che carta dargli
        stampaCarteInMano(p, giocatoreScelto);
        do{
            printf("Quale carta vuoi cedere?\t");
        scanf("%d", &indiceCarta);
        getchar();
        printf("\n");
        }while(indiceCarta > p[giocatoreScelto].nHand);
        indiceCarta--;
        printf("%s (p%d) si ritrova con una ", p[giocatoreScelto].name, giocatoreScelto+1);
        leggiEnum(p[giocatoreScelto].hand, indiceCarta);
        printf(" in meno\n\n");
        p[currentP].nHand++; //aumento realloco e assegno la carta
        p[currentP].hand=(Card*)realloc(p[currentP].hand, p[currentP].nHand * sizeof(Card));
        p[currentP].hand[p[currentP].nHand-1]= p[giocatoreScelto].hand[indiceCarta];
        scartaCarta(p, giocatoreScelto, indiceCarta);
}

/*Procedura del caso un giocatore giochi una djanni card, se ne possiede un'altra identica glielo segnala e attiva direttamente
l'effetto delle djanni card in coppia, senza chiederlo perchè se uno possiede due djanni e ovvio che voglia attivare l'effetto
e non scartarne una singolarmente.. Nel caso ne possega solo una la scarta*/
void djanniCase(Player hPlayer[],Infos gameInfo, int indiceCarta){
    _Bool nope;
    _Bool djannix2;
    nope = checkNope(hPlayer, gameInfo.playerTurn);
    if(nope == true){
        printf("\nLa nope ha fatto il suo sporco lavoro!\n");
    }else
    if(nope == false){
        djannix2 = doppioDjanni(hPlayer, gameInfo.playerTurn, indiceCarta);
            if(djannix2 == true){
                printf("\n%s (p%d) ha giocato 2 Djanni, two better than one!\n", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
                printf("\n");
            }else{
                printf("\nIl player %s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
                printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
                printf("Ma non succede niente...\n");
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
            }
    }
}

/*DoppioDjanni si occupa di gestire il caso in cui un giocatore abbia scartato una DJANNI CARD e ne possegga una uguale
nel caso ne possegga 2 ritorna true e sfrutta il potere delle djanni card giocate in coppia, ovvero ruba una carta
casuale all'avversario scelto, nel caso in cui il player possiede solo un tipo di djanni card ritorna
e false scarta solo la carta scelta */
_Bool doppioDjanni(Player p[], int currentP, int indiceCarta ){
    int i, giocatoreScelto, cartaRubata;
    _Bool doppioDjanni=false;
    int frasiUguali;
    srand(time(NULL)); //Azzero seed
    for(i=0; i<p[currentP].nHand; i++){ //Controlla tramite strcmp che le frasi siano uguali
            if(i != indiceCarta){
                frasiUguali = strcmp(p[currentP].hand[indiceCarta].phrase, p[currentP].hand[i].phrase);
                    if(frasiUguali == 0){
                        doppioDjanni=true; //Se sono uguali ritorna true ed esce
                        break;
                    }else{
                    doppioDjanni = false; //Se si possiede una solo djanni di un tipo ritorna false
                    }
            }
    }
    if(doppioDjanni == true){ //Se ritorna true segnala al giocatore il fatto
            printf("Hai due Djanni cards uguali! Le puoi scartare entrambe per rubare una carta ad un giocatore!\n");
            do{
                printf("Scegli il giocatore:\n"); //Permette di scegliere il giocatore a cui rubarla
                scanf("%d", &giocatoreScelto);
                getchar();
            giocatoreScelto--;
            }while(p[giocatoreScelto].alive == false || giocatoreScelto == currentP || p[giocatoreScelto].nHand == 0);
            cartaRubata = rand()%p[giocatoreScelto].nHand; //Genera la carta da rubare del player scelto
            scartaCarta(p, currentP, indiceCarta); //Scarta le due Djanni
            scartaCarta(p, currentP, i);
            p[currentP].nHand++; //aggiunge la carta
            p[currentP].hand=(Card*)realloc(p[currentP].hand, p[currentP].nHand * sizeof(Card));
            p[currentP].hand[p[currentP].nHand-1]= p[giocatoreScelto].hand[cartaRubata];
            scartaCarta(p, giocatoreScelto, cartaRubata); //e la rimuove dalla persona che ha subito lo scippo
            printf("Hai rubato una ");
            leggiEnum(p[currentP].hand, p[currentP].nHand-1);
            printf("\n");
    }
    return doppioDjanni; //In conclusione restituisce un booleano al main che cambia il testo a seconda siano stati
}                       //giocati un djanni o due


/****************FUNZIONI VARIE****************/
/*Funzione che conta i morti, serve per far capire al while quando deve fermarsi*/
int atLeast2(Player p[]){
    int i,morti = 0;
    for(i=0; i<NPLAYERS; i++){
        if(p[i].alive == 0){
            morti++;
        }
    }
    return morti;
}

/*Procedura che pulisce lo schermo*/
void clearScreen(Player hPlayer[]){
    #ifdef _WIN32
    system("cls");
    #endif
    #ifdef linux
    system("clear");
    #endif
    printf("\t\t\t\t*************ExploadingDjanni Game!*************\n\n");
    printf("Ogni giocatore durante il proprio turno puo' pescare premendo D, giocare una carta premendo P o salvare premendo S\n");
    printf("\n\n");

}

/*Controlla che la carta sia una exploding o meno o una nope*/
_Bool controllaCarta(Card* deck, int deckposition){
    _Bool esplosione;
    switch(deck[deckposition].type){
case 0:
    esplosione = true;
    break;
case 1:
case 2:
case 3:
case 4:
case 5:
case 6:
case 7:
case 8:
    esplosione = false;
    break;
    }
    return esplosione;
}

/*Procedura per mescolare il deck dove n è il numero di carte da mescolare*/
void deckShuffle(Card deck[], int n){
 int i, j;
 srand ( time(NULL) );
    for (i = n-1; i > 0; i--){
        j = rand() % (i+1); //prende un indice casuale compreso tra 0 ed n
        swapp(&deck[i], &deck[j]); // scambia il contenuto di deck[i] con quello contenuto nell'indice generato casualmente
    }
}

/*Procedura con lo scopo di stampare il tipo di carta al posto del numero*/
void leggiEnum(Card playerHand[], int indice){
    switch(playerHand[indice].type){
    case 0:
        printf("EXPLODING DJANNI");
        break;
    case 1:
        printf("*MEOOOW CARD*");
        break;
    case 2:
        printf("SHUFFLE CARD");
        break;
    case 3:
        printf("NOPE CARD");
        break;
    case 4:
        printf("SEE THE FUTURE CARD");
        break;
    case 5:
        printf("ATTACK CARD");
        break;
    case 6:
        printf("SKIP CARD");
        break;
    case 7:
        printf("FAVOR CARD");
        break;
    case 8:
        printf("%s", playerHand[indice].phrase);
        break;
    }
}

/*Procedura per stampare un array passandogli l'array e il totale di posti*/
void printArray (Card deck[], int n){ //Funzione per stampare un array di n elementi, ricorrente nella debugmode 1
    int i;
    for (i=0; i<n; i++){
        printf("%u\t%s  ", deck[i].type, deck[i].phrase);
        printf("\n");
    }
}

/*Procedura per scartare una carta*/
void scartaCarta(Player p[], int currentP, int indiceCarta){
    int i, j=0;
    for(i=0; i<p[currentP].nHand; i++){
        if(i != indiceCarta){ //Se la carta da scartare non è quella dell'indice di i
            p[currentP].hand[j]=p[currentP].hand[i];  //Riscrive la carta
            j++; //e va avanti scorrendo
        }
        //se invece il ciclo arriva alla carta da scartare non entra nell'if e j non viene aumentata, quindi la carta da scartare viene sovrascritta
    }
    p[currentP].nHand--; //Le carte in mano vengono ridotte di 1
    p[currentP].hand =(Card*)realloc(p[currentP].hand, p[currentP].nHand * sizeof(Card)); //La mano si rialloca
}

/*Funzione che gestisce l'azione della carta SEE THE FUTURE, mostrando le prime 3 carte del deck*/
void showFirst3(Card* deck,int cardsLeftinthedeck){
    int i;
    cardsLeftinthedeck--; //Imposto l'indice
    for(i=0; i<3; i++){
        leggiEnum(deck, cardsLeftinthedeck); //stampa dell'enumerazione e della frase delle carte successive
        printf("\t");
        cardsLeftinthedeck--; //Questa istruzione scorre la posizione del deck
}
}

/*Procedura che permette di stampare le carte in mano possedute da un giocatore*/
void stampaCarteInMano(Player p[],int currentP){
    int i;
    for(i=0; i<p[currentP].nHand; i++){
        printf("%d. ", i+1);
        leggiEnum(p[currentP].hand, i);
        printf("\n");
    }
}
/*Procedura fondamentale per il deckshuffle che sfrutta un array ausiliario per scambiare i contenuti delle celle passate */
void swapp(Card *i, Card *j){
    Card aux = *i;
    *i = *j;
    *j = aux;
}

/*Funzione che saluta il giocatore e lo accoglie all'interno del gioco*/
void welcome(void){ //Procedura per dare il benvenuto al giocatore
    printf("\t\t\t\t********Benvenuto all'exploadingDjanni Game!********\n\n");
    printf("Le regole sono semplici, ogni turno un giocatore pesca una carta,le carte sono diverse ma se prendi una bomba sei fuori!\n");
}

/*Funzione che individua l'indice della carta scelta e lo restituisce*/
int whichCard(Player p[], int currentP){
     int choise;
     if(p[currentP].nHand > 0){
        do{
        printf("Quale carta ? Sceglila digitando il numero della carta (1, 2, 3 ecc.)\t");
        scanf("%d", &choise);
        getchar();
        }while(choise > p[currentP].nHand); //Se il giocatore inserisce una carta che non possiede gli viene richiesto di inserire una carta
        choise--;
        printf("\n");
        return choise;
            }
            else{
                printf("Non hai carte da giocare!\n\n"); //Se non ha carte ritorna 9 un tipo di carta che non esiste
                return 9;   //che passando per il default del tipo delle carte effettua un break
            }
}

/*Procedura che pulisce lo schermo e dichiara la vittoria del giocatore*/
void winnerClear(Player hPlayer[], int currentP){
    #ifdef _WIN32
    system("cls");
    #endif
    #ifdef linux
    system("clear");
    #endif
    printf("\t\t\t\t*************ExploadingDjanni Game!*************\n\n");
    printf("\n\n\n");
    printf("\tW\n");
    printf("\tI\n");
    printf("\tN\n");
    printf("\tN\n");
    printf("\tN\n");
    printf("\tE\n");
    printf("\tR\n");
    printf("\t\t\t****COMPLIMENTI %s SEI IL VINCITORE DELL'EXPLOADING DJANNI GAME!****\n", hPlayer[currentP].name);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tW\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tI\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tN\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tN\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tN\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tE\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\tR\n");
}
