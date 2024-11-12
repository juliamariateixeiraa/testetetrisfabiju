#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA 10  // Largura do tabuleiro
#define ALTURA 20   // Altura do tabuleiro
#define TAMANHO_BLOCO 2 // Tamanho do bloco para exibição na tela

// Definição das formas das peças (tetrominos) e suas rotações possíveis
char tetrominos[7][4][4][4] = {
    { // I
        { {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} },
        { {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0} },
        // Rotação de 90 graus
    },
    // Continuação das outras peças e rotações (O, T, S, Z, L, J)
};

// Estado do tabuleiro, onde cada posição indica se há um bloco ou não
int tabuleiro[LARGURA][ALTURA];

// Estrutura que representa uma peça com sua posição, tipo e rotação
typedef struct {
    int x, y;       // Coordenadas da peça
    int tipo;       // Tipo da peça (I, O, T, etc.)
    int rotacao;    // Rotações da peça
} Peca;

Peca pecaAtual; // Peça que está atualmente ativa no jogo

// Função para desenhar o tabuleiro
void desenharTabuleiro() {
    screenClear(); // Limpa a tela
    screenSetColor(CYAN, BLACK); // Define a cor de fundo
    for (int i = 0; i < LARGURA; i++) {
        for (int j = 0; j < ALTURA; j++) {
            if (tabuleiro[i][j]) { // Verifica se há bloco naquela posição
                screenGotoxy(SCRSTARTX + i * TAMANHO_BLOCO, SCRSTARTY + j); // Posiciona o cursor
                printf("[]"); // Desenha o bloco
            }
        }
    }
}

// Função para desenhar a peça atual
void desenharPeca(Peca *p) {
    screenSetColor(RED, BLACK); // Define a cor da peça
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                screenGotoxy(SCRSTARTX + (p->x + i) * TAMANHO_BLOCO, SCRSTARTY + p->y + j);
                printf("[]");
            }
        }
    }
}

// Função para verificar colisões
int verificarColisao(Peca *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                int x = p->x + i;
                int y = p->y + j;
                // Verifica se saiu dos limites ou colidiu com o tabuleiro
                if (x < 0 || x >= LARGURA || y >= ALTURA || tabuleiro[x][y]) return 1;
            }
        }
    }
    return 0;
}

// Função para posicionar a peça no tabuleiro
void posicionarPeca(Peca *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                tabuleiro[p->x + i][p->y + j] = 1; // Marca o bloco no tabuleiro
            }
        }
    }
}

// Função para remover as linhas completas do tabuleiro
void removerLinhasCompletas() {
    for (int j = 0; j < ALTURA; j++) {
        int completa = 1;
        for (int i = 0; i < LARGURA; i++) {
            if (!tabuleiro[i][j]) {
                completa = 0; // Marca que a linha não está completa
                break;
            }
        }
        // Aqui pode-se implementar o código para apagar a linha completa e ajustar o tabuleiro
    }
}
