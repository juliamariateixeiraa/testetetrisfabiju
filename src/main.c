#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20
#define BLOCK_SIZE 2

void screenDrawBorders();
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

int level = 1;  // Nível do jogo
int score = 0;  // Pontuação

// Estado do tabuleiro
int board[WIDTH][HEIGHT];

// Estrutura para representar uma peça
typedef struct {
    int x, y;
    int type;
    int rotation;
} Piece;

Piece currentPiece;

void exibirTelaInicial() {
    system("clear"); // Limpa o terminal (ajuste para "cls" se estiver no Windows)
    
    // Chama a função para desenhar as bordas
    screenDrawBorders();

    // Centralizar o título ASCII
    int titulo_linhas = 6;  // O título tem 6 linhas
    int titulo_comprimento = 50;  // Comprimento médio do título (ajuste conforme necessário)
    int start_x = (MAXX - titulo_comprimento) / 2;  // Calcula a posição de início para centralizar

    // Exibe o título ASCII centralizado
    screenGotoxy(start_x, 5);  // Posição de início para o título (ajuste a linha se necessário)
    printf("██████╗░██╗░░░░░░█████╗░░█████╗░██╗░░██╗██╗░░░██╗\n");
    screenGotoxy(start_x, 6);
    printf("██╔══██╗██║░░░░░██╔══██╗██╔══██╗██║░██╔╝╚██╗░██╔╝\n");
    screenGotoxy(start_x, 7);
    printf("██████╦╝██║░░░░░██║░░██║██║░░╚═╝█████═╝░░╚████╔╝░\n");
    screenGotoxy(start_x, 8);
    printf("██╔══██╗██║░░░░░██║░░██║██║░░██╗██╔═██╗░░░╚██╔╝░░\n");
    screenGotoxy(start_x, 9);
    printf("██████╦╝███████╗╚█████╔╝╚█████╔╝██║░╚██╗░░░██║░░░\n");
    screenGotoxy(start_x, 10);
    printf("╚═════╝░╚══════╝░╚════╝░░╚════╝░╚═╝░░╚═╝░░░╚═╝░░░\n");

    // Exibe a mensagem de preparação centralizada
    screenGotoxy((MAXX - 42) / 2, 12);
    printf("Prepare-se para uma partida emocionante de TETRIS!\n");
    screenGotoxy((MAXX - 20) / 2, 13);
    printf("Pressione ENTER...\n");

    // Aguarde o usuário iniciar
    getchar();
}


void exibirInstrucoes() {
    system("clear"); // Limpa o terminal para as instruções
    
    // Desenha as bordas ao redor da área de instruções
    screenDrawBorders();

    // Centralizar o título das instruções
    int instrucoes_comprimento = 60; // Comprimento médio das instruções
    int start_x = (MAXX - instrucoes_comprimento) / 2;  // Recalcular a posição de início para centralizar

    // Exibe o título das instruções com borda
    screenGotoxy(start_x, 5);  // Posição de início para o título
    screenGotoxy(start_x, 6);
    printf("                                                          \n");
    screenGotoxy(start_x, 7);
    printf("                   INSTRUÇÕES DO TETRIS                   \n");
    screenGotoxy(start_x, 8);
    printf("                                                          \n");

    // Exibe o conteúdo das instruções, centralizado
    screenGotoxy(start_x, 9);
    printf(" Use as setas para mover as peças:                        \n");
    screenGotoxy(start_x, 10);
    printf("  A: Mover para a Esquerda                                 \n");
    screenGotoxy(start_x, 11);
    printf("  D: Mover para a Direita                                  \n");
    screenGotoxy(start_x, 12);
    printf("  W: Girar a peça                                         \n");
    screenGotoxy(start_x, 13);
    printf("  S: Acelerar a queda da peça                             \n");
    screenGotoxy(start_x, 14);
    printf("                                                          \n");

    // Exibe a mensagem dentro da borda
    screenGotoxy(start_x, 15);
    printf("                                                          \n");
    screenGotoxy(start_x, 16);
    printf("      Pressione ENTER para começar...                     \n");
    screenGotoxy(start_x, 17);
    printf("                                                          \n");

    // Espera o usuário pressionar Enter para continuar
    getchar();
}




// Função que verifica se a peça colidiu
int checkCollision(Piece *piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[piece->type][piece->rotation][i][j]) {
                int x = piece->x + i;
                int y = piece->y + j;
                if (x < 0 || x >= WIDTH || y >= HEIGHT || board[x][y]) {
                    return 1;  // Colidiu com a borda ou outra peça
                }
            }
        }
    }
    return 0;  // Não houve colisão
}

// Função que coloca a peça no tabuleiro
void placePiece(Piece *piece) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[piece->type][piece->rotation][i][j]) {
                board[piece->x + i][piece->y + j] = 1;  // Marca o tabuleiro com a peça
            }
        }
    }
}

// Função que rotaciona a peça
void rotatePiece() {
    int oldRotation = currentPiece.rotation;
    currentPiece.rotation = (currentPiece.rotation + 1) % 4;  // Rotaciona para a próxima posição
    if (checkCollision(&currentPiece)) {
        currentPiece.rotation = oldRotation;  // Se houver colisão, volta para a rotação anterior
    }
}

// Função que move a peça na horizontal
void movePiece(int dx) {
    currentPiece.x += dx;  // Desloca a peça para a esquerda (dx=-1) ou para a direita (dx=1)
    if (checkCollision(&currentPiece)) {
        currentPiece.x -= dx;  // Se houver colisão, desfaz o movimento
    }
}

// Função que desenha a peça na tela
void drawPiece(Piece *piece) {
    screenSetColor(RED, BLACK);  // Define a cor para desenhar a peça
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[piece->type][piece->rotation][i][j]) {  // Verifica se a célula da peça está ativa
                screenGotoxy(SCRSTARTX + (piece->x + i) * BLOCK_SIZE, SCRSTARTY + piece->y + j);  // Calcula a posição na tela
                printf("[]");  // Desenha o bloco da peça
            }
        }
    }
}


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
    // Exibe o nível e a pontuação
    screenGotoxy(SCRSTARTX + WIDTH * BLOCK_SIZE + 2, SCRSTARTY);
    printf("Nível: %d", level);
    screenGotoxy(SCRSTARTX + WIDTH * BLOCK_SIZE + 2, SCRSTARTY + 1);
    printf("Pontuação: %d", score);
}

void removeFullLines() {
    // Função que remove linhas completas
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
            score += 100; // Adiciona 100 pontos por linha completa
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
