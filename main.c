#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "splash.h"
#include "lore.h"
#include "game.h"
#include "combate.h"
#include "rank.h"

char *menu[] = {"JOGAR", "HISTORIA", "RANKING", "SAIR"};
int n_choices = sizeof(menu) / sizeof(char *);
int destaque = 0;

void imprimir_menu(WINDOW *win, int destaque) {
    int x, y, i;

    getmaxyx(win, y, x);

    for (i = 0; i < n_choices; ++i) {
        mvwprintw(win, y / 2 - n_choices / 2 + i, x / 2 - strlen(menu[i]) / 2,
                  "%s", menu[i]);
    }

    wattron(win, A_REVERSE);
    mvwprintw(win, y / 2 - n_choices / 2 + destaque, x / 2 - strlen(menu[destaque]) / 2, "%s", menu[destaque]);
    wattroff(win, A_REVERSE);

    wrefresh(win);
}

int main() {
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);

    int escolha = -1;
    int tecla;
    bool voltar_ao_menu = 0;

    create_splash_screen(stdscr);

    while (1) {
        clear();
        imprimir_menu(stdscr, destaque);
        refresh();

        tecla = getch();

        switch (tecla) {
            case KEY_UP:
                if (destaque > 0)
                    destaque--;
                break;
            case KEY_DOWN:
                if (destaque < n_choices - 1)
                    destaque++;
                break;
            case 10:
                escolha = destaque;
                break;
        }

        if (escolha != -1 && escolha != n_choices - 1) {
            clear();
            printw("Acessando %s...\n", menu[escolha]);
            refresh();
            if (escolha == 0) {  // JOGAR selecionado
            runGame(WINDOW *stdscr,int vida,int ataque,int vida_inimigo,int ataque_inimigo,fechar_jogo);
                voltar_ao_menu = 1;
            } else if (escolha == 1) {
                loretoth();
                voltar_ao_menu =1;
            }
            getch();
            escolha = -1;  // Reseta a escolha para que possa ser reutilizada
        } else if (escolha == 2) {
            ranqueamento();
            }
            else if (escolha == n_choices - 1) {
            printw("Aperte qualquer tecla para confirmar SAIR, ou 'n' para voltar\n");
            refresh();
            tecla = getch();
            if (tecla == 'n' || tecla == 'N') {
                escolha = -1;
            } else {
                break;
            }
        }

        if (voltar_ao_menu) {
            voltar_ao_menu = 0 ;
            escolha = -1;
        }
    }

    endwin();

    return 0;
}
