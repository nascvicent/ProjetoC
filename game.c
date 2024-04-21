#include "game.h"
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "combate.h"
#include "rank.h"


int pontuacao = 0;

void createRoom(WINDOW *win, int y, int x, int height, int width) {
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            if (i == y || i == y + height - 1 || j == x || j == x + width - 1) {
                waddch(win, '#' | COLOR_PAIR(1));
            } else {
                waddch(win, ' ');
            }
        }
    }
}


void create_passage(WINDOW *win, int y, int x) {
    mvwaddch(win, y, x, ' ');
}

void create_enemy(WINDOW *win, int y, int x) {
    mvwaddch(win, y, x, 'E' | COLOR_PAIR(2));
}

int fechar_jogo = 0;
int borda_direita;

void bBaixo(int max_y, int max_x, int border_height) {
    int bordaBaixo_height = (int)(max_y * 0.2);
    int bordaBaixo_width = (int)(max_x * 0.8);

    WINDOW *borda_baixo = newwin(bordaBaixo_height, bordaBaixo_width, border_height, 0);
    box(borda_baixo, 0, 0);
    mvwprintw(borda_baixo, 1, 1, "Para lutar com algum inimigo, esteja pr�ximo dele e aperte 'f'");
    mvwprintw(borda_baixo, 2, 1, "Digite 'q' para sair");
    wrefresh(borda_baixo);
}

void bDireita(int max_y, int max_x, int border_width, int vida, int ataque, int *lista_infos, int total_enemies, int pontuacao) {
    // BORDA DIREITA
    int bordaDireita_height = (int)(max_y * 0.8);
    int bordaDireita_width = (int)(max_x * 0.2);

    WINDOW *borda_direita = newwin(bordaDireita_height, bordaDireita_width, 0, border_width);
    box(borda_direita, 0, 0);
    mvwprintw(borda_direita, 1, 1, "Vida: %d", lista_infos[0]);
    mvwprintw(borda_direita, 2, 1, "Ataque: %d", ataque);
    mvwprintw(borda_direita, 3, 1, "Inimigos: %d", total_enemies);
    wrefresh(borda_direita);

    if (vida <= 0) {
        werase(borda_direita);
        box(borda_direita, 0, 0);
        mvwprintw(borda_direita, 1, 1, "GAME OVER");
        wrefresh(borda_direita);
        sleep(2);
        endwin();
    }

    if (total_enemies == 0) {
        pontuacao = pontuacao*2 ;
        werase(borda_direita);
        box(borda_direita, 0, 0);
        mvwprintw(borda_direita, 1, 1, "VOC� GANHOU");
        wrefresh(borda_direita);
        sleep(2);
        endwin();
         FILE *file = fopen("pontuacao.txt", "a");
          if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Adiciona a pontuação ao arquivo
    fprintf(file, "%d\n", pontuacao);

    // Fecha o arquivo
    fclose(file);

    }
}




void runGame(WINDOW *stdscr, int vida, int ataque, int vida_inimigo, int ataque_inimigo, int fechar_jogo) {
    curs_set(0);
    nodelay(stdscr, 1);
    timeout(100);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int border_height = (int)(max_y * 0.8);
    int border_width = (int)(max_x * 0.8);

    WINDOW *sala_win = newwin(border_height, border_width, 0, 0);
    box(sala_win, 0, 0);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    int player_x , player_y;
    player_x = 1;
    player_y = 1;

    srand(time(NULL));

    int num_rooms = rand() % 4 + 4;
    int max_height = border_height - 2;
    int max_width = border_width - 2;

    int rooms[num_rooms][4]; // Cada sala � representada por um array [y, x, height, width]
    int total_enemies = 0;


    for (int i = 0; i < num_rooms; i++) {
        int height = rand() % 3 + 6;
        int width = rand() % 3 + 6;
        int y = rand() % (max_height - height - 2) + 2;
        int x = rand() % (max_width - width - 2) + 2;


        int overlap = 0;
        for (int j = 0; j < i; j++) {
            if (rooms[j][0] < y + height + 2 &&
                rooms[j][0] + rooms[j][2] + 2 > y &&
                rooms[j][1] < x + width + 2 &&
                rooms[j][1] + rooms[j][3] + 2 > x) {
                overlap = 1;
                break;
            }
        }

        if (overlap) {
            continue;
        }

        rooms[i][0] = y;
        rooms[i][1] = x;
        rooms[i][2] = height;
        rooms[i][3] = width;

        createRoom(sala_win, y, x, height, width);




    if (rooms) {
        int prev_y, prev_x, prev_h, prev_w;
        getmaxyx(sala_win, prev_h, prev_w); // Assuming sala_win is your window
        getyx(sala_win, prev_y, prev_x);

        int passage_y = rand() % (prev_h - 2) + prev_y + 1;
        int passage_x = prev_x + prev_w - 1;
        create_passage(sala_win, passage_y, passage_x);

        int num_enemies = 1;
        total_enemies += num_enemies;

        for (int i = 0; i < num_enemies; i++) {
            int enemy_y = rand() % (height - 2) + y + 1;
            int enemy_x = rand() % (width - 2) + x + 1;
            create_enemy(sala_win, enemy_y, enemy_x);
        }
     }
    }
    mvwaddch(sala_win, player_y, player_x, '@' | COLOR_PAIR(1));
    wrefresh(sala_win);
    int lista_infos[2] = {vida, vida_inimigo};
    int inimigos_a_remover[10];
    int luta_em_andamento = 0;

    while (1) {
        bBaixo(max_y, max_x, border_height);
        bDireita(max_y, max_x, border_width, vida, ataque, lista_infos, total_enemies, pontuacao);
        if (fechar_jogo == 1) {
            break;
        }

        int key;
        key = getch();

        int surrounding_coords[8][2] = {
        {player_y - 1, player_x - 1}, {player_y - 1, player_x}, {player_y - 1, player_x + 1},
        {player_y, player_x - 1}, {player_y, player_x + 1},
        {player_y + 1, player_x - 1}, {player_y + 1, player_x}, {player_y + 1, player_x + 1}
    };
    if (!luta_em_andamento && key == 'f') {
        luta_em_andamento = 1;
    } else if (luta_em_andamento) {
         int inimigos_a_remover[100][2];  // xy

        int i;
        for (i = 0; i < sizeof(surrounding_coords) / sizeof(surrounding_coords[0]); i++) {
            int coord_y = surrounding_coords[i][0];
            int coord_x = surrounding_coords[i][1];

            // Suponha que sala_win.inch seja uma fun��o que retorna o caractere na posi��o (coord_y, coord_x).
            int ch = mvwinch(sala_win, coord_y, coord_x);

            // Extraia o caractere real da c�lula
            char character = ch & 0xFF;

            if (character == 'E') {
                // Suponha que Fight.luta seja uma fun��o em C com a mesma funcionalidade da vers�o Python.
                int lista_infos[2];
              //  lista_infos = luta(vida, ataque, vida_inimigo, ataque_inimigo);
                vida = lista_infos[0];
                vida_inimigo = lista_infos[1];

                if (lista_infos[1] <= 0) {
                    inimigos_a_remover[i][0] = coord_y;
                    inimigos_a_remover[i][1] = coord_x;
                    total_enemies--;
                    pontuacao++;
                }
            }
    } if (vida <= 0) {
        luta_em_andamento = 0;
        fechar_jogo= 1;
    } luta_em_andamento = 0;
    for (i = 0; i < sizeof(inimigos_a_remover) / sizeof(inimigos_a_remover[0]); i++) {
        int coord_y = inimigos_a_remover[i][0];
        int coord_x = inimigos_a_remover[i][1];
        // Remover o caractere na posi��o (coord_y, coord_x)
        mvaddch(coord_y, coord_x, ' ');
    }

      if (total_enemies == 0) {
        fechar_jogo = 1;
    }

    if (key == 'q') {
        break;
    }

    for (int i = 0; i < sizeof(inimigos_a_remover) / sizeof(inimigos_a_remover[0]); i++) {
        int coord_y = inimigos_a_remover[i][0];
        int coord_x = inimigos_a_remover[i][1];
        mvwaddch(sala_win, coord_y, coord_x, ' ');
    }

    int next_player_y;
    int next_player_x;
    next_player_y = player_y;
    next_player_x = player_x;

    switch (key) {
        case 'w':
            next_player_y -= 1;
            break;
        case 's':
            next_player_y += 1;
            break;
        case 'a':
            next_player_x -= 1;
            break;
        case 'd':
            next_player_x += 1;
            break;
    }  int ch = mvwinch(sala_win, next_player_y, next_player_x);

    if ((ch != '#') && (next_player_y >= 0 && next_player_y < border_height) && (next_player_x >= 0 && next_player_x < border_width)) {
        // Verifica se o caractere � '.' ou ' '
        if (mvwinch(sala_win, next_player_y, next_player_x) == '.' || mvwinch(sala_win, next_player_y, next_player_x) == ' ') {
            // Atualiza a posi��o do jogador na janela
            mvwaddch(sala_win, player_y, player_x, ' ');
            mvwaddch(sala_win, next_player_y, next_player_x, '@' | COLOR_PAIR(3));
            player_y = next_player_y;
            player_x = next_player_x;
        }
    }

}}}

