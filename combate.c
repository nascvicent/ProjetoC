#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



void stts(int vida , int ataque, int vida_inimigo, int ataque_inimigo) {
        srand (time(NULL));


        vida = rand()%3 + 12; // 12~14 de vida do jogador
        ataque = rand()%3 + 6; // 6~8 de ataque do jogador
        vida_inimigo = rand()%3 + 7; // 7 ~ 9 de vida do inimigo
        ataque_inimigo = rand()%2 + 4; // 4 ~ 6 de ataque do inimigo



}

int* luta(int vida, int ataque, int vida_inimigo, int ataque_inimigo) {
    static int result[2];

    while (1) {
        int iniciativa = rand() % 3;
        if (vida <= 0) {
            break;
        }

        if (iniciativa == 1) {  // player come�a
            vida_inimigo -= ataque;
            if (vida_inimigo <= 0) {
                break;
            } else {
                sleep(1);
                vida -= ataque_inimigo;
                if (vida <= 0) {
                    break;
                }
            }
        }

        if (iniciativa == 2) {
            vida -= ataque_inimigo;
            if (vida <= 0) {
                break;
            } else {
                sleep(1);
                vida_inimigo -= ataque;
                if (vida_inimigo <= 0) {
                    break;
                } else {
                    sleep(1);
                    vida -= ataque_inimigo;
                }
            }
        }
    }

    result[0] = vida;
    result[1] = vida_inimigo;
    return result;
}




