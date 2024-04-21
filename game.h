#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#define PLAYER '@'
#define ROOM '#'
#define EMPTY '.'
#include <curses.h>

void create_enemy(WINDOW *win, int y, int x);
void bBaixo(int max_y, int max_x, int border_height);
void createRoom(WINDOW *win, int y, int x, int height, int width);
void create_passage(WINDOW *win, int y, int x);
void bDireita(int max_y, int max_x, int border_width, int vida, int ataque, int *lista_infos, int total_enemies, int pontuacao);
void runGame(WINDOW *stdscr, int vida, int ataque, int vida_inimigo, int ataque_inimigo, int fechar_jogo);

#endif // GAME_H_INCLUDED
