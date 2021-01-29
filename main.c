#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "macro.h"
#include "functions.h"
#include "structs.h"
int main(){
    /*Dichiarazione Variabili*/
    Card *deck=NULL;
    int static turniScontati;
    int difficulty, cardsLeftinthedeck, indiceCarta,morti;
    int *pCardsLeftinthedeck = &cardsLeftinthedeck;
    Infos gameInfo;
    Infos *pGameInfo = &gameInfo;
    _Bool loadornew, exploding, nope, attack=false;
    Player hPlayer[NPLAYERS];
    char choise;
    /*Interfaccia e caricamento di elementi utili allo svolgimento del gioco*/
    welcome();
    loadornew = nuovaPartita();
    if(loadornew == true){ //Se il giocatore vuole caricare una partita
       deck = caricaPartita(deck, hPlayer, pGameInfo, pCardsLeftinthedeck);
    }else{ //Se non vuole caricare vengono creati i P, il deck, e le info
    difficulty = sceltaDifficolta();
    playerCreationH(hPlayer, NPLAYERS);
    deck = chosenDeck(deck, difficulty, hPlayer);
    cardsLeftinthedeck = howmanycardsinthedeck(difficulty);
    deckShuffle(deck, cardsLeftinthedeck);
    gameInfo = firstPlayer();
    }
    /*Inizio gioco*/
    clearScreen(hPlayer); //Per separare il gioco dall'interfaccia di scelta partita
   do{
/*Operazioni preliminari per il controllo dell'attack*/

        if(gameInfo.save == 1){ //Controllo che permette di caricare partite con player sotto attacco, se un player è sotto attacco e la partita
            attack = true;  //e' stata salvata gameinfo.save varrà true quindi con questa informazione evince che il giocatore sia sotto attack
        }
        if(turniScontati == 1){ //Turni scontati è una variabile statica che verifica che il giocatore abbia scontato 1 turno extra
            turniScontati=0;    //Mentre è sotto attack
            attack=false;
            gameInfo.save=false;
        }
       if(attack == true){ //Se il giocatore è sotto attacco il primo turno che farà aumenterà di 1 il valore di turni scontati
        turniScontati++;
       }
/*A chi tocca e richiesta d'azione*/
        printf("- Tocca a %s (p%d)\nCarte in Mano:\n", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
        stampaCarteInMano(hPlayer, gameInfo.playerTurn);
        printf("- Cosa vuoi fare?\t\t");
        scanf("%c", &choise);
        getchar();
/**Inizio switch con azioni consentite al player ogni turno*/
        switch(choise){
/*Pescare una carta*/
    case 'd': //Se il giocatore scegli di pescare
    case 'D':
        exploding = drawCard(deck, hPlayer, gameInfo, cardsLeftinthedeck);
        if(exploding == false){  //Se il giocatore NON ha pescato una bomba
            cardsLeftinthedeck--;
            printf("Hai pescato una ");
            leggiEnum(hPlayer[gameInfo.playerTurn].hand, hPlayer[gameInfo.playerTurn].nHand-1);
            printf("\n");
        }
        gameInfo.playerTurn = nextGuy(gameInfo.playerTurn, hPlayer, attack);
        break;
/*Giocare una carta*/
    case 'p':
    case 'P':
        indiceCarta = whichCard(hPlayer, gameInfo.playerTurn);
        switch(hPlayer[gameInfo.playerTurn].hand[indiceCarta].type){
            case 1: //Meooow
                meooowCase(hPlayer, gameInfo, indiceCarta);
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                break;

            case 2: //Shuffle
                shuffleCase(hPlayer, gameInfo, indiceCarta, deck, cardsLeftinthedeck);
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                break;

            case 3: //Nope
                nopeWithNoeff(hPlayer, gameInfo, indiceCarta);
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                break;

            case 4: //See the future
                future(hPlayer, gameInfo, indiceCarta, deck, cardsLeftinthedeck);
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                break;

            case 5: // Attack
                nope = checkNope(hPlayer, gameInfo.playerTurn);
                if(nope == true){
                    printf("\nLa nope ha fatto il suo sporco lavoro!\n");
                }else
                if(nope == false){  //Se nessun nope si oppone si attiva
                    printf("Il player %s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
                    leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
                    printf("\n");
                    printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
                    gameInfo.playerTurn = nextGuy(gameInfo.playerTurn, hPlayer, attack); //Scorre il turno
                    attack=true; //E attiva il segnalino attack
                }
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                if(attack == true){  //Se il giocatore è sotto attacco si salva la partita in automatico e si setta il fatto che sia stata salvata a true
                    saveGame(deck, hPlayer, gameInfo, cardsLeftinthedeck);
                    gameInfo.save = true;
                }
                break;

            case 6: //Skip
                nope = checkNope(hPlayer, gameInfo.playerTurn);
            if(nope == true){
                    printf("\nLa nope ha fatto il suo sporco lavoro!\n");
                }else
            if(nope == false){  //Se nessun nope si oppone si attiva
                printf("Il player %s (p%d) ha giocato una ", hPlayer[gameInfo.playerTurn].name, gameInfo.playerTurn+1);
                leggiEnum(hPlayer[gameInfo.playerTurn].hand, indiceCarta);
                printf("\n");
                printf("%s\n", hPlayer[gameInfo.playerTurn].hand[indiceCarta].phrase);
            }
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                gameInfo.playerTurn = nextGuy(gameInfo.playerTurn, hPlayer, attack);
                break;

            case 7: //Favor
                favorCase(hPlayer,gameInfo,indiceCarta);
                scartaCarta(hPlayer, gameInfo.playerTurn, indiceCarta);
                break;

            case 8: //Djanni
                djanniCase(hPlayer,gameInfo,indiceCarta);
                break;

            default:
                break;
        }
        break;
/*Salvare partita*/
    case 's':
    case 'S':
        if(attack == true){ //Se si salva mentre un giocatore e' sotto attack allora gameinfo.save diventa true
            gameInfo.save = true;
            saveGame(deck, hPlayer, gameInfo, cardsLeftinthedeck); //Salvataggio con giocatore sotto attack
            printf("\nPartita salvata con successo!\n");
            }else{ //In una situazione di attack false invece si salva lasciando gameinfo.save false
                saveGame(deck, hPlayer, gameInfo, cardsLeftinthedeck); //Salvataggio in una situazione non attack
                printf("\nPartita salvata con successo!\n");
            }
        break;
            }
/**Fine dello switch delle scelte*/
/*Questa parte pulisce lo schermo premendo invio in modo da permettere a ogni giocatore in teoria di vedere
solo le proprie carte*/
        printf("\nDigita invio per continuare . . . \n"); //Per impedire ad un giocatore di vedere le carte dell'altro ma allo stesso tempo
        getchar();                                  //Vedere che cosa ha pescato dopo il draw gli si chiede di premere invio
        clearScreen(hPlayer);                       //In modo da pulire lo schermo volta per volta
        morti = atLeast2(hPlayer); //Controlla se la partita deve ancora continuare
     }while( morti != 3);

/*Proclamazione del vincitore!*/
    printf("%s e' l'ultimo soppravvisuto!\n", hPlayer[gameInfo.playerTurn].name);
    winnerClear(hPlayer, gameInfo.playerTurn);
    return 0;
}

