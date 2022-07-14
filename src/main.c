#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


const char PLAYER_1_TOKEN = 'O';
const char PLAYER_2_TOKEN = 'X';

typedef struct Move {
    int i;
    int j;
} Move;


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
bool won(Game *game, Player player, Move *move) {
    int alignedToken = 0;
    // Horizontal Check
    for(int j = max(move->j - 4, 0); j <= min(move->j +4, 6) ; j++) {
        if(game->board[move->i][j] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }

    alignedToken = 0;
    // Vertical Check
    for(int i = max(move->i - 4, 0); i <= min(move->i +4, 6) ; i++) {
        if(game->board[i][move->j] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }

    // Diagonal / Check
    // />  Decrease i, Increase j 
    alignedToken = 0;
    int jLoc[7] = {-3,-2,-1,0,1,2,3};
    int iLoc[7] = {3,2,1,0,-1,-2,3};
    for(int count = 0 ; count < 7 ; count++) {
        int i = move->i + iLoc[count];
        int j = move->j + jLoc[count];

        // when computed location outside of game board we skip check
        if(i < 0 || i > 6 || j < 0 ||j > 6) continue;

        if(game->board[i][j] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }

    // Diagonal \ Check
    // \>  Increase i, Increase j
    alignedToken = 0;
    int jLoc2[7] = {-3,-2,-1,0,1,2,3};
    int iLoc2[7] = {-3,-2,-1,0,1,2,3};
    for(int count = 0 ; count < 7 ; count++) {
        int i = move->i + iLoc2[count];
        int j = move->j + jLoc2[count];
        
        // when computed location outside of game board we skip check
        if(i < 0 || i > 6 || j < 0 ||j > 6) continue;
        if(game->board[i][j] == player.token) {
            alignedToken++;
        } else alignedToken = 0;
        if(alignedToken >= 4) return true;
    }
    return false;
}

// [0:0][0:1][0:2][0:3][0:4][0:5][0:6]
// [1:0][1:1][1:2][1:3][1:4][1:5][1:6]
// [2:0][2:1][2:2][2:3][2:4][2:5][2:6]
// [3:0][3:1][3:2][3:3][3:4][3:5][3:6]
// [4:0][4:1][4:2][4:3][4:4][4:5][4:6]
// [5:0][5:1][5:2][5:3][5:4][5:5][5:6]
// [6:0][6:1][6:2][6:3][6:4][6:5][6:6]

int main()
{
    Game *g = newGame("P1", "P2");
    printf("[%s: %c] [%s: %c]\n", g->players[0].name, g->players[0].token, g->players[1].name, g->players[1].token);
    
    Slots s = slots(g);
    Move *move = malloc(sizeof(Move));
    move->i = 5;
    move->j = 0;
    printf("%d, %d \n", slot(s, 4), s.length);

    g->board[0][4] = 'X';

    s = slots(g);
    printf("%d, %d \n", slot(s, 4), s.length);

    printBoard(g);

    play(g, 0, 'X');
    play(g, 0, 'X');
    if(won(g, g->players[1], move)) {
        printf("Winner !\n");
    }

    play(g, 0, 'X');
    play(g, 0, 'X');
    
    printf("\n");
    printBoard(g);
    if(won(g, g->players[1], move)) {
        printf("Winner !\n");
    }

    play(g, 0, 'X');
    printf("\n");
    printBoard(g);

    if(won(g, g->players[1], move)) {
        printf("Winner !\n");
    }
    return 0;
}