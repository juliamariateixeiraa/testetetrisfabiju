#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_SIZE 2

// Definição das formas das peças com rotações possíveis
char tetrominos[7][4][4][4] = {
    { // I
        { {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} },
        { {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} }
    },
    { // O
        { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} }
    },
    { // T
        { {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }
    },
    { // S
        { {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} }
    },
    { // Z
        { {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} }
    },
    { // L
        { {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} },
        { {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} }
    },
    { // J
        { {1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0} },
        { {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0} }
    }
};


void exibirTelaInicial() {
    system("clear"); // Limpa o terminal (ajuste para "cls" se estiver no Windows)
    
    // Exibe o título
    printf("########################### #################################\n");
    printf("████████╗███████╗████████╗██████╗░██╗░███████╗\n ");
    printf("╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝\n ");
    printf("░░░██║░░░█████╗░░░░░██║░░░██████╔╝██║╚█████╗░\n ");
    printf("░░░██║░░░██╔══╝░░░░░██║░░░██╔══██╗██║░╚═══██╗\n ");
    printf("░░░██║░░░███████╗░░░██║░░░██║░░██║░░██║██║██║█████╔╝\n ");
    printf("░░░╚═╝░░░╚══════╝░░░╚═╝░░░╚═╝░░╚═╝╚═ ════╝░\n ");
    printf("######################################## #############\n");
    printf("\nPrepare-se para uma partida emocionante de TETRIS!\n");
    printf("Pressione ENTER...\n");

    // Aguarde o usuário iniciar
    getchar();
}

// Função para exibir as instruções do jogo
void exibirInstrucoes() {
    system("clear"); // Limpa o terminal para as instruções
    printf("############################################################\n");
    printf("#                                                          #\n");
    printf("#                   INSTRUÇÕES DO TETRIS                   #\n");
    printf("#                                                          #\n");
    printf("############################################################\n");
    printf("#                                                          #\n");
    printf("#  Como jogar:                                             #\n");
    printf("#  - Use as teclas de setas para controlar as peças:       #\n");
    printf("#    -> Seta para ESQUERDA  : Move a peça para a esquerda  #\n");
    printf("#    -> Seta para DIREITA   : Move a peça para a direita   #\n");
    printf("#    -> Seta para BAIXO     : Acelera a descida da peça    #\n");
    printf("#    -> Tecla ESPAÇO        : Gira a peça                  #\n");
    printf("#                                                          #\n");
    printf("#  Objetivo:                                               #\n");
    printf("#  - Complete linhas horizontais para ganhar pontos e      #\n");
    printf("#    limpar as linhas.                                     #\n");
    printf("#  - O jogo acaba se as peças chegarem ao topo.            #\n");
    printf("#                                                          #\n");
    printf("#  Pressione ENTER para começar o jogo!                    #\n");
    printf("#                                                          #\n");
    printf("############################################################\n");
    getchar(); // Espera o usuário pressionar ENTER para continuar
}
// Estado do tabuleiro
int board[WIDTH][HEIGHT];

// Estrutura para representar uma peça
typedef struct {
    int x, y;
    int type;
    int rotation;
} Piece;

Piece currentPiece;

void drawBoard() {
    screenClear();
    screenSetColor(CYAN, BLACK);
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            if (board[i][j]) {
                screenGotoxy(SCRSTARTX + i * BLOCK_SIZE, SCRSTARTY + j);
                printf("[]");
            }
        }
    }
}

void drawPiece(Piece *p) {
    screenSetColor(RED, BLACK);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->type][p->rotation][i][j]) {
                screenGotoxy(SCRSTARTX + (p->x + i) * BLOCK_SIZE, SCRSTARTY + p->y + j);
                printf("[]");
            }
        }
    }
}

int checkCollision(Piece *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->type][p->rotation][i][j]) {
                int x = p->x + i;
                int y = p->y + j;
                if (x < 0 || x >= WIDTH || y >= HEIGHT || board[x][y]) return 1;
            }
        }
    }
    return 0;
}

void placePiece(Piece *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->type][p->rotation][i][j]) {
                board[p->x + i][p->y + j] = 1;
            }
        }
    }
}

void removeFullLines() {
    for (int j = 0; j < HEIGHT; j++) {
        int full = 1;
        for (int i = 0; i < WIDTH; i++) {
            if (!board[i][j]) {
                full = 0;
                break;
            }
        }
        if (full) {
            for (int k = j; k > 0; k--) {
                for (int i = 0; i < WIDTH; i++) {
                    board[i][k] = board[i][k - 1];
                }
            }
            for (int i = 0; i < WIDTH; i++) {
                board[i][0] = 0;
            }
        }
    }
}

void spawnPiece() {
    currentPiece.x = WIDTH / 2 - 2;
    currentPiece.y = 0;
    currentPiece.type = rand() % 7;
    currentPiece.rotation = 0;
    if (checkCollision(&currentPiece)) {
        screenDestroy();
        printf("Game Over\n");
        exit(0);
    }
}

void rotatePiece() {
    int oldRotation = currentPiece.rotation;
    currentPiece.rotation = (currentPiece.rotation + 1) % 4;
    if (checkCollision(&currentPiece)) currentPiece.rotation = oldRotation;
}

void movePiece(int dx) {
    currentPiece.x += dx;
    if (checkCollision(&currentPiece)) currentPiece.x -= dx;
}

void dropPiece() {
    currentPiece.y++;
    if (checkCollision(&currentPiece)) {
        currentPiece.y--;
        placePiece(&currentPiece);
        removeFullLines();
        spawnPiece();
    }
}

void processInput() {
    if (keyhit()) {
        int key = readch();
        switch (key) {
            case 'a': movePiece(-1); break;
            case 'd': movePiece(1); break;
            case 's': dropPiece(); break;
            case 'w': rotatePiece(); break;
        }
    }
}

int main() {
    exibirTelaInicial();
    exibirInstrucoes();
    srand(time(NULL));
    screenInit(1);
    keyboardInit();
    timerInit(500);

    spawnPiece();

    while (1) {
        processInput();
        if (timerTimeOver()) {
            dropPiece();
        }
        drawBoard();
        drawPiece(&currentPiece);
        screenUpdate();
    }

    screenDestroy();
    keyboardDestroy();
    return 0;
}
