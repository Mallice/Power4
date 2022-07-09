#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


const char PLAYER_1_TOKEN = 'O';
const char PLAYER_2_TOKEN = 'X';

typedef struct Player {
    char *name;
    char token;
} Player;
typedef struct Game {
    char board[7][7];
    Player players[2];
} Game;

/**
 * Slots structure save an array of values indicating the columns that can still contain a token
 * and the length of this array
 */
typedef struct Slots {
    int *values;
    int length;
} Slots;

int slot(Slots s, int index) {
    if(index > s.length || index < 0) return -1;
    return s.values[index];
}

Game *newGame(char *player1, char *player2) {
    Player p1 = {player1, PLAYER_1_TOKEN};
    Player p2 = {player2, PLAYER_2_TOKEN};
    Game *game = malloc(sizeof(Game));
    game->players[0] = p1;
    game->players[1] = p2;
    memset(game->board, ' ', sizeof(game->board));
    return game; 
}

/**
 * Found where it is possible to play a token on the board
 */
Slots slots(Game *game) {
    int *slots = malloc(7 * sizeof(int));
    int counter = 0;
    for(int j = 0; j < 7; j++) {
        if(game->board[0][j] == ' ') {
            slots[counter++] = j;
        }
    }

    Slots s = {realloc(slots, counter * sizeof(int)), counter};
    return s;
}

void printBoard(Game *game) {
    for(int i = 0 ; i < 7 ; i++) {
        for(int j = 0 ; j < 7 ; j++) {
            
            printf("[%c]", game->board[i][j]);
        }
        printf("\n");
    }
}


void play(Game *g, int column, char token) {
    for(int i = 7 ; i > 0; --i) {
        if(g->board[i][column] == ' ') {
            g->board[i][column] = token;
            return;
        }
    }
}


/**
 * 
 */
bool won(Game *game, Player player, int playedI, int playedJ) {
    int alignedToken = 0;
    // Horizontal Check
    for(int j = max(playedJ - 4, 0); j < min(playedJ +4, 7) ; j++) {
        if(game->board[playedI][j] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }

    alignedToken = 0;
    // Vertical Check
    for(int i = max(playedI - 4, 0); i < min(playedI +4, 7) ; i++) {
        if(game->board[i][playedJ] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }

    // Diagonal / Check
    alignedToken =0;

    // Diagonal \ Check
    return false;
}


int main()
{
    Game *g = newGame("P1", "P2");
    printf("[%s: %c] [%s: %c]\n", g->players[0].name, g->players[0].token, g->players[1].name, g->players[1].token);
    printBoard(g);
    
    Slots s = slots(g);
    printf("%d, %d \n", slot(s, 4), s.length);

    g->board[0][4] = 'X';

    s = slots(g);
    printf("%d, %d \n", slot(s, 4), s.length);

    printBoard(g);

    play(g, 0, 'X');
    play(g, 0, 'X');
        if(won(g, g->players[1], 5, 0)) {
        printf("Winner !\n");
    }

    play(g, 0, 'X');
    play(g, 0, 'X');
    
    printf("\n");
    printBoard(g);
        if(won(g, g->players[1], 5, 0)) {
        printf("Winner !\n");
    }

    play(g, 0, 'X');
    printf("\n");
    printBoard(g);
    if(won(g, g->players[1], 5, 0)) {
        printf("Winner !\n");
    }
    return 0;
}