#include "screen.h"
#include "timer.h"
#include "keyboard.h"
#include <stdlib.h>
#include <time.h>

#define LARGURA 10
#define ALTURA 20
#define TAMANHO_BLOCO 2

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

// Estado do tabuleiro
int tabuleiro[LARGURA][ALTURA];

// Estrutura para representar uma peça
typedef struct {
    int x, y;
    int tipo;
    int rotacao;
} Peca;

Peca pecaAtual;

void desenhaTabuleiro() {
    telaLimpar();
    telaDefinirCor(CIANO, PRETO);
    for (int i = 0; i < LARGURA; i++) {
        for (int j = 0; j < ALTURA; j++) {
            if (tabuleiro[i][j]) {
                telaMoverCursor(TELAINICIOX + i * TAMANHO_BLOCO, TELAINICIOY + j);
                printf("[]");
            }
        }
    }
}

void desenhaPeca(Peca *p) {
    telaDefinirCor(VERMELHO, PRETO);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                telaMoverCursor(TELAINICIOX + (p->x + i) * TAMANHO_BLOCO, TELAINICIOY + p->y + j);
                printf("[]");
            }
        }
    }
}

int verificaColisao(Peca *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                int x = p->x + i;
                int y = p->y + j;
                if (x < 0 || x >= LARGURA || y >= ALTURA || tabuleiro[x][y]) return 1;
            }
        }
    }
    return 0;
}

void posicionaPeca(Peca *p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (tetrominos[p->tipo][p->rotacao][i][j]) {
                tabuleiro[p->x + i][p->y + j] = 1;
            }
        }
    }
}

void removeLinhasCompletas() {
    for (int j = 0; j < ALTURA; j++) {
        int completa = 1;
        for (int i = 0; i < LARGURA; i++) {
            if (!tabuleiro[i][j]) {
                completa = 0;
                break;
            }
        }
        if (completa) {
            for (int k = j; k > 0; k--) {
                for (int i = 0; i < LARGURA; i++) {
                    tabuleiro[i][k] = tabuleiro[i][k - 1];
                }
            }
            for (int i = 0; i < LARGURA; i++) {
                tabuleiro[i][0] = 0;
            }
        }
    }
}

void geraPeca() {
    pecaAtual.x = LARGURA / 2 - 2;
    pecaAtual.y = 0;
    pecaAtual.tipo = rand() % 7;
    pecaAtual.rotacao = 0;
    if (verificaColisao(&pecaAtual)) {
        telaDestruir();
        printf("Game Over\n");
        exit(0);
    }
}

void rotacionaPeca() {
    int rotacaoAntiga = pecaAtual.rotacao;
    pecaAtual.rotacao = (pecaAtual.rotacao + 1) % 4;
    if (verificaColisao(&pecaAtual)) pecaAtual.rotacao = rotacaoAntiga;
}

void movePeca(int dx) {
    pecaAtual.x += dx;
    if (verificaColisao(&pecaAtual)) pecaAtual.x -= dx;
}

void descePeca() {
    pecaAtual.y++;
    if (verificaColisao(&pecaAtual)) {
        pecaAtual.y--;
        posicionaPeca(&pecaAtual);
        removeLinhasCompletas();
        geraPeca();
    }
}

void processaEntrada() {
    if (teclaPressionada()) {
        int tecla = lerTecla();
        switch (tecla) {
            case 'a': movePeca(-1); break;
            case 'd': movePeca(1); break;
            case 's': descePeca(); break;
            case 'w': rotacionaPeca(); break;
        }
    }
}

int main() {
    srand(time(NULL));
    telaIniciar(1);
    tecladoIniciar();
    temporizadorIniciar(500);

    geraPeca();

    while (1) {
        processaEntrada();
        if (temporizadorTempoEsgotado()) {
            descePeca();
        }
        desenhaTabuleiro();
        desenhaPeca(&pecaAtual);
        telaAtualizar();
    }

    telaDestruir();
    tecladoDestruir();
    return 0;
}
