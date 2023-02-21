#define _CRT_SECURE_NO_WARNINGS
#define COLOR 0
#define STOP 12
#define TAKI 13
#define PLUSE 10
#define DIRECT 11
#include <stdio.h>
#include <stdlib.h> // in order to use the "rand" and "srand" functions
#include <time.h>
#include <stdbool.h>
#include <string.h>

//רותם שרף
//205986680

typedef struct {
    char color; //g - green, r - red, y - yellow, b - blue
    int sig; // integer betwwen 1 to 9
}CARD;

typedef struct {
    char name[20];
    int nCard;
    int maxCard;
    CARD *cards;
}PLAYER;

//creat random card
CARD generatorCard(int stat[15]){
    char colors[4] = {'G', 'R', 'Y', 'B'};
    CARD card;
    card.sig = rand() % 14;
    stat[card.sig]++;
    card.color = card.sig == COLOR ? ' ' : colors[rand() % 4];
    return card;
}

//print card
void printCard(CARD card){
    printf("*********\n*       *\n");
    switch(card.sig){
        case 0: printf("* COLOR *"); break;
        case 10: printf("*   +   *"); break;
        case 11: printf("*  <->  *"); break;
        case 12: printf("*  STOP *"); break;
        case 13: printf("*  TAKI *"); break;
        default: printf("*   %d   *", card.sig);
    }
    printf("\n*   %c   *\n", card.color);
    printf("*       *\n*********\n");
}

//when the player put card on the deck we remove the card from hes hade
void removeCard(PLAYER *player, int idx, CARD *upperCard){
    *upperCard = player->cards[idx];
    for (int i = idx; i < player->nCard- 1; i++) {
        player->cards[i] = player->cards[i + 1];
    }
    player->nCard--;
}
// chak if the choosen card feet to the upper card
bool isValideCard(PLAYER *player, int idx, CARD *upperCard){
    //return true;
    if(idx == -1) return true;
    if(idx < 0 || idx >= player->nCard) return false;
    return player->cards[idx].sig == COLOR || player->cards[idx].sig == upperCard->sig
    || player->cards[idx].color == upperCard->color;
}

//after turn end, this function change the turn to the next player
int getNextPlayerTurn(int turn, int nPlay, bool isRight){
    if(isRight){
        if(turn + 1 == nPlay) return 0;
        return turn + 1;
    }
    if(turn == 0) return nPlay - 1;
    return turn - 1;
}

//add card to the hade
void drawCard(PLAYER *player, int stat[14]){
    player->cards = (CARD*) realloc(player->cards, (player->nCard + 1) * sizeof(CARD));
    player->cards[player->nCard] = generatorCard(stat);
    player->nCard++;
}

//print all card in the hand
void printHand(PLAYER* player){
    int i;
    for(i = 0; i < player->nCard; i++){
                printf("Card #%d:\n\n", i + 1);
                printCard(player->cards[i]);
            }
}

//the TAKI card
void runTaki(PLAYER* player, CARD* upperCard){
    int choice = 0;
	printf("You entered TAKI mode of color %c\n", upperCard->color);
	do{
	    printHand(player);
	    printf("Enter next card, enter 0 to stop \n");
		scanf("%d", &choice);
		if(!choice) break;
		if(choice < player->nCard && choice > 0 && player->cards[choice - 1].color == upperCard->color){
		    removeCard(player, choice, upperCard);
		}else printf("invalide color\n");
	}while (true);
}

//the color card
void runColor(CARD* upperCard){
    int color;
    printf("Please enter your color choice:\n");
    printf("1 - Yellow\n");
    printf("2 - Red\n");
    printf("3 - Blue\n");
    printf("4 - Green\n");
    do{
        scanf("%d", &color);
        if(0 < color && color <= 4) break;
        printf("invalide color, try again\n");
    }while(true);
    upperCard->color = color == 1 ? 'Y'
        : color == 2 ? 'R'
        : color == 3 ? 'B'
        : 'G';
}

void printStatistic(int stat[14]){
    int max, i, j;
    printf("****Game Statistic******\nCard # | Frequency\n____________\n");
    for(i = 0; i < 14; i++){
        max = 0;
        for(j = 1; j < 14; j++){
            if(stat[max] < stat[j]) max = j;
        }
        printf("   %d   |   %d   \n", max, stat[max]);
        stat[max] = -1;
    }
}


int main()
{
    int nPlay, i, j, nameLe, turn = 0, n, stat[14];
    char name[20];
    bool finishTurn, finishGame = false, isRight = true;

    for(i = 0; i < 14; i++) stat[i] = 0;
    printf("Welcome To TAKI:\n");
    CARD upperCard = generatorCard(stat);
    PLAYER *players;
    srand(time(NULL));
    do{
        printf("Please enter the number of player:\n");
        scanf("%d", &nPlay);
        if(nPlay > 0) break;
        printf("you not allow choose 0\n");
    }while(true);
    players = (PLAYER*) malloc(sizeof(PLAYER) * nPlay);
    for(i = 0; i < nPlay; i++){
        printf("Please enter the first name of player #%d:\n", i + 1);
        scanf("%s", players[i].name);

        players[i].nCard = 5;
        players[i].cards = (CARD*) malloc(sizeof(CARD) * 5);
        for(j = 0; j < 5; j++) {players[i].cards[j] = generatorCard(stat);}
    }
    do{
        do{
            printf("Upper card:\n");
            printCard(upperCard);
            printf("%s's turn:\n", players[turn].name);
            printHand(&players[turn]);
            printf("Please enter 0 if you want to take a card from the deck or 1-%d if you want to put one of your cards in the middle:", players[turn].nCard);
            scanf("%d", &n);
            finishTurn = false;
            if(isValideCard(&players[turn], n - 1, &upperCard)){
                if(n == 0) {
                    drawCard(&players[turn], stat);
                    finishTurn = true;
                }else{
                    removeCard(&players[turn], n - 1, &upperCard);
                    finishTurn = true;
                }
                switch(upperCard.sig){
                    case PLUSE:
                        finishTurn = false;
                        break;
                    case DIRECT:
                        isRight = !isRight;
                        break;
                    case STOP:
                        turn = getNextPlayerTurn(turn, nPlay, isRight);
                        break;
                    case TAKI:
                        runTaki(&players[turn], &upperCard);
                        break;
                    case COLOR: 
                        runColor(&upperCard);
                    default: break;
                }
            }else printf("invalide card, try again!\n");
        
    
        }while(!finishTurn);
        
        if(players[turn].nCard == 0) {
            
            finishGame = true;
            printf("playes %d win!!!", turn + 1);
        }
        turn = getNextPlayerTurn(turn, nPlay, isRight);
    }while(!finishGame);
    for(i = 0; i < nPlay; i++) free(players[i].cards);
    printStatistic(stat);
    
    
    
    
    return 0;
}