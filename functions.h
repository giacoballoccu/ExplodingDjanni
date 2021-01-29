#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include "structs.h"
/*In questo file dichiaro le funzioni*/
int atLeast2(Player p[]);
Card* caricaPartita(Card* deck, Player hPlayer[], Infos *gameinfo, int *cardsLeftinthedeck);
_Bool checkNope(Player p[], int currentP);
Card* chosenDeck(Card* deck, int difficulty, Player hPlayer[]);
void clearScreen(Player hPlayer[]);
_Bool controllaCarta(Card* deck, int deckposition);
void deckShuffle(Card deck[], int n);
void djanniCase(Player hPlayer[],Infos gameInfo, int indiceCarta);
_Bool doppioDjanni(Player p[], int currentP, int indiceCarta);
_Bool drawCard(Card* deck, Player p[], Infos gameInfo, int cardsLeftinthedeck);
void explodingDJ(Player p[], int nHand,int currentP);
void favorCase(Player hPlayer[],Infos gameInfo, int indiceCarta);
void favorCard(Player p[], int currentP);
Infos firstPlayer(void);
void future(Player hPlayer[],Infos gameInfo, int indiceCarta, Card* deck, int cardsLeftinthedeck);
Card * giveDjanni(Card* djanniCards,Player hPlayer[], int nDjanniCards);
Card * giveMeooow(Card* meooow,Player hPlayer[], int nMeooow);
int howmanycardsinthedeck(int difficulty);
void inputPlayer(Player p[], int i);
void leggiEnum(Card playerHand[], int indice);
Card* makeDjanniPart(Card* djanniCards, FILE* fp, int nDjanniCards);
Card* makeExplodingPart(Card *explodingDjanni, FILE* fp, int nExploding);
Card* makeMeooowPart(Card* meooow, FILE* fp, int nMeooow);
Card* makeDjanniPart(Card* djanniCards, FILE* fp, int nDjanniCards);
void meooowCase(Player hPlayer[],Infos gameInfo, int indiceCarta);
int nextGuy(int currentP, Player p[], _Bool attack);
void nopeWithNoeff(Player hPlayer[],Infos gameInfo, int indiceCarta);
_Bool nuovaPartita();
void playerCreationH(Player p[], int nHumans);
void printArray (Card deck[], int n);
void saveGame(Card* deck, Player hPlayer[], Infos gameinfo, int cardsLeft);
void scartaCarta(Player p[], int currentP, int indiceCarta);
int sceltaDifficolta();
void showFirst3(Card* deck,int cardsLeftinthedeck);
void shuffleCase(Player hPlayer[],Infos gameInfo, int indiceCarta, Card* deck, int cardsLeftinthedeck);
void stampaCarteInMano(Player p[],int currentP);
void swapp(Card *i, Card *j);
void welcome(void);
void winnerClear(Player hPlayer[], int currentP);
int whichCard(Player p[], int currentP);
#endif // FUNCTIONS_H_INCLUDED
