#include "lore.h"
#include <curses.h>
#include <unistd.h>

void loretoth() {
    initscr();  // Inicializa a tela
    curs_set(0);  // Esconde o cursor

    printw("Bem-vindo ao mundo misterioso de Toth!\n\n");

    char historia[] = "Ha muito tempo, um mal antigo despertou nas profundezas do Labirinto Infinito.\n"
                      "Os aventureiros corajosos, como voce, foram chamados para explorar suas profundezas,\n"
                      "enfrentar monstros terriveis e encontrar tesouros lendarios.\n\n"
                      "O destino do reino esta em suas maos. Boa sorte, aventureiro!\n";

    int comprimento = strlen(historia);

    for (int i = 0; i < comprimento; ++i) {
        printw("%c", historia[i]);
        refresh();
        usleep(30000);
    }

    getch();

    endwin();
}
