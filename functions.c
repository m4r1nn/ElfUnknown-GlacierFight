// Copyright Burcea Marian-Gabriel 2018
#include "./functions.h"

void swaps(int *a, int *b) {
    (*a) = (*a) ^ (*b);
    (*b) = (*a) ^ (*b);
    (*a) = (*a) ^ (*b);
}

void swap_struct(struct Fighter *a, struct Fighter *b) {
    struct Fighter aux;
    aux = (*a);
    (*a) = (*b);
    (*b) = aux;
}

void read_data(struct Glaciar **a, int R, int n, struct Fighter **player,
int *remain) {
    char name[CHR];
    (*player) = malloc(n * sizeof(struct Fighter));
    for (int i = 0; i < 2 * R + 1; i++) {
        for (int j = 0; j < 2 * R + 1; j++) {
            scanf("%d%d", &a[i][j].hill, &a[i][j].gloves);
            // valoarea -1 inseamna ca nu se gaseste niciun pitic in celula
            a[i][j].player = -1;
            // if-ul care stabileste care celule sunt pe harta (nu in apa)
            if ((R - i) * (R - i) + (R - j) * (R - j) <= R * R) {
                a[i][j].is_on_map = 1;
            } else {
                a[i][j].is_on_map = 0;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        scanf("%s%d%d%d%d", name, &(*player)[i].line,
        &(*player)[i].column, &(*player)[i].hp, &(*player)[i].stamina);
        (*player)[i].kills = 0;
        (*player)[i].name = malloc((strlen(name) + 1) * sizeof(char));
        strncpy((*player)[i].name, name, strlen(name) + 1);
        // verific daca jucatorul pica pe ghetar
        if ((*player)[i]. line >= 0 && (*player)[i].line < 2 * R + 1 &&
            (*player)[i].column >= 0 && (*player)[i].column < 2 * R + 1 &&
            a[(*player)[i].line][(*player)[i].column].is_on_map) {
            // piticul ia manusile din celula, iar campul player al celulei
            // primeste indicele piticului
            (*player)[i].gloves =
            a[(*player)[i].line][(*player)[i].column].gloves;
            a[(*player)[i].line][(*player)[i].column].gloves = 0;
            a[(*player)[i].line][(*player)[i].column].player = i;
        } else {
            printf("%s has missed the glacier.\n", (*player)[i].name);
            (*remain)--;
            // stamina -1 inseamna ca piticul nu mai este in joc
            (*player)[i].stamina = -1;
        }
    }
}

int fight_winner(struct Fighter *player, int fighter1, int fighter2,
int *remain) {
    int current;
    // decid cine incepe 'bulgareala', dupa regula din enunt
    if (player[fighter1].stamina > player[fighter2].stamina) {
        current = 1;
    } else {
        current = 0;
    }
    // bulgareala se termina cand unul dintre pitici are hp-ul negativ
    while (player[fighter1].hp > 0 && player[fighter2].hp > 0) {
        if (current == 1) {
            player[fighter2].hp -= player[fighter1].gloves;
        } else {
            player[fighter1].hp -= player[fighter2].gloves;
        }
        current ^= 1;
    }
    // fac transferul de stamina de la piticul scos din joc la piticul
    // castigator, cresc numarul de kill-uri pentru castigator pentru
    // scoreboard, afisez mesajul corespunzator si returnez castigatorul luptei
    if (player[fighter1].hp <= 0) {
        player[fighter2].stamina += player[fighter1].stamina;
        player[fighter1].stamina = -1;
        printf("%s sent %s back home.\n",
        player[fighter2].name, player[fighter1].name);
        (*remain)--;
        player[fighter2].kills++;
        return fighter2;
    } else {
        player[fighter1].stamina += player[fighter2].stamina;
        player[fighter2].stamina = -1;
        printf("%s sent %s back home.\n",
        player[fighter1].name, player[fighter2].name);
        (*remain)--;
        player[fighter1].kills++;
        return fighter1;
    }
}

void move_up(struct Glaciar **a, struct Fighter *player, int id, int *remain) {
    // verific daca piticul iese in afara ghetarului
    if (player[id].line > 0) {
        // verific daca piticul are suficienta stamina sa se mute
        if (player[id].stamina >= fabs(
        a[player[id].line][player[id].column].hill -
        a[player[id].line - 1][player[id].column].hill)) {
            // fac schimbul de manusi daca este cazul
            if (a[player[id].line - 1][player[id].column].gloves >
                player[id].gloves) {
                swaps(&a[player[id].line - 1][player[id].column].gloves,
                &player[id].gloves);
            }
            // daca am ajuns pana aici inseamna ca pot sa sterg indicele
            // piticului din celula precedenta, piticul pierde stamina
            a[player[id].line][player[id].column].player = -1;
            player[id].stamina -= (int)fabs(
            a[player[id].line - 1][player[id].column].hill -
            a[player[id].line][player[id].column].hill);
            // daca in celula in care se muta se gaseste alt pitic, cei 2 se
            // vor bulgari
            if (a[player[id].line - 1][player[id].column].player != -1) {
                int fighter1, fighter2;
                fighter1 = a[player[id].line - 1][player[id].column].player;
                fighter2 = id;
                a[player[id].line - 1][player[id].column].player =
                fight_winner(player, fighter1, fighter2, &(*remain));
            } else {
                a[player[id].line - 1][player[id].column].player = id;
            }
            // verific daca celula in care s-a mutat apartine ghetarului
            if (a[player[id].line - 1][player[id].column].is_on_map == 0) {
                printf("%s fell off the glacier.\n", player[id].name);
                (*remain)--;
                player[id].stamina = -1;
            }
            player[id].line--;
        }
    } else {
        printf("%s fell off the glacier.\n", player[id].name);
        (*remain)--;
        player[id].stamina = -1;
        a[player[id].line][player[id].column].player = -1;
    }
}

void move_down(struct Glaciar **a, int R, struct Fighter *player, int id,
int *remain) {
    // verific daca piticul iese in afara ghetarului
    if (player[id].line < 2 * R) {
        // verific daca piticul are suficienta stamina sa se mute
        if (player[id].stamina >= fabs(
        a[player[id].line][player[id].column].hill -
        a[player[id].line + 1][player[id].column].hill)) {
            // fac schimbul de manusi daca este cazul
            if (a[player[id].line + 1][player[id].column].gloves >
                player[id].gloves) {
                swaps(&a[player[id].line + 1][player[id].column].gloves,
                &player[id].gloves);
            }
            // daca am ajuns pana aici inseamna ca pot sa sterg indicele
            // piticului din celula precedenta, piticul pierde stamina
            a[player[id].line][player[id].column].player = -1;
            player[id].stamina -= (int)fabs(
            a[player[id].line + 1][player[id].column].hill -
            a[player[id].line][player[id].column].hill);
            // daca in celula in care se muta se gaseste alt pitic, cei 2 se
            // vor bulgari
            if (a[player[id].line + 1][player[id].column].player != -1) {
                int fighter1, fighter2;
                fighter1 = a[player[id].line + 1][player[id].column].player;
                fighter2 = id;
                a[player[id].line + 1][player[id].column].player =
                fight_winner(player, fighter1, fighter2, &(*remain));
            } else {
                a[player[id].line + 1][player[id].column].player = id;
            }
            // verific daca celula in care s-a mutat apartine ghetarului
            if (a[player[id].line + 1][player[id].column].is_on_map == 0) {
                printf("%s fell off the glacier.\n", player[id].name);
                (*remain)--;
                player[id].stamina = -1;
            }
            player[id].line++;
        }
    } else {
        printf("%s fell off the glacier.\n", player[id].name);
        (*remain)--;
        player[id].stamina = -1;
        a[player[id].line][player[id].column].player = -1;
    }
}

void move_left(struct Glaciar **a, struct Fighter *player, int id,
int *remain) {
    // verific daca piticul iese in afara ghetarului
    if (player[id].column > 0) {
        // verific daca piticul are suficienta stamina sa se mute
        if (player[id].stamina >= fabs(
        a[player[id].line][player[id].column].hill -
        a[player[id].line][player[id].column - 1].hill)) {
            // fac schimbul de manusi daca este cazul
            if (a[player[id].line][player[id].column - 1].gloves >
                player[id].gloves) {
                swaps(&a[player[id].line][player[id].column - 1].gloves,
                &player[id].gloves);
            }
            // daca am ajuns pana aici inseamna ca pot sa sterg indicele
            // piticului din celula precedenta, piticul pierde stamina
            a[player[id].line][player[id].column].player = -1;
            player[id].stamina -= (int)fabs(
            a[player[id].line][player[id].column - 1].hill -
            a[player[id].line][player[id].column].hill);
            // daca in celula in care se muta se gaseste alt pitic, cei 2 se
            // vor bulgari
            if (a[player[id].line][player[id].column - 1].player != -1) {
                int fighter1, fighter2;
                fighter1 = a[player[id].line][player[id].column - 1].player;
                fighter2 = id;
                a[player[id].line][player[id].column - 1].player =
                fight_winner(player, fighter1, fighter2, &(*remain));
            } else {
                a[player[id].line][player[id].column - 1].player = id;
            }
            // verific daca celula in care s-a mutat apartine ghetarului
            if (a[player[id].line][player[id].column - 1].is_on_map == 0) {
                printf("%s fell off the glacier.\n", player[id].name);
                (*remain)--;
                player[id].stamina = -1;
            }
            player[id].column--;
        }
    } else {
        printf("%s fell off the glacier.\n", player[id].name);
        (*remain)--;
        player[id].stamina = -1;
        a[player[id].line][player[id].column].player = -1;
    }
}

void move_right(struct Glaciar **a, int R, struct Fighter *player, int id,
int *remain) {
    // verific daca piticul iese in afara ghetarului
    if (player[id].column < 2 * R) {
        // verific daca piticul are suficienta stamina sa se mute
        if (player[id].stamina >= fabs(
        a[player[id].line][player[id].column].hill -
        a[player[id].line][player[id].column + 1].hill)) {
            // fac schimbul de manusi daca este cazul
            if (a[player[id].line][player[id].column + 1].gloves >
                player[id].gloves) {
                swaps(&a[player[id].line][player[id].column + 1].gloves,
                &player[id].gloves);
            }
            // daca am ajuns pana aici inseamna ca pot sa sterg indicele
            // piticului din celula precedenta, piticul pierde stamina
            a[player[id].line][player[id].column].player = -1;
            player[id].stamina -= (int)fabs(
            a[player[id].line][player[id].column + 1].hill -
            a[player[id].line][player[id].column].hill);
            // daca in celula in care se muta se gaseste alt pitic, cei 2 se
            // vor bulgari
            if (a[player[id].line][player[id].column + 1].player != -1) {
                int fighter1, fighter2;
                fighter1 = a[player[id].line][player[id].column + 1].player;
                fighter2 = id;
                a[player[id].line][player[id].column + 1].player =
                fight_winner(player, fighter1, fighter2, &(*remain));
            } else {
                a[player[id].line][player[id].column + 1].player = id;
            }
            // verific daca celula in care s-a mutat apartine ghetarului
            if (a[player[id].line][player[id].column + 1].is_on_map == 0) {
                printf("%s fell off the glacier.\n", player[id].name);
                (*remain)--;
                player[id].stamina = -1;
            }
            player[id].column++;
        }
    } else {
        printf("%s fell off the glacier.\n", player[id].name);
        (*remain)--;
        player[id].stamina = -1;
        a[player[id].line][player[id].column].player = -1;
    }
}

void move_player(struct Glaciar **a, int R, int n, struct Fighter *player,
int *remain) {
    // UDLR este un secventa de caractere care vor da directia in care se va
    // muta piticul, id este indicele jucatorului care se muta
    char UDLR[CHR];
    int id;
    scanf("%s", UDLR);
    id = atoi(UDLR);
    scanf("%s", UDLR);
    // parcurg secventa si fac mutarile cat timp piticul este in joc
    for (unsigned long i = 0; i < strlen(UDLR) && player[id].stamina >= 0;
    i++) {
        switch (UDLR[i]) {
            // verific de fiecare data a ramas un singur pitic pentru a-l
            // declara castigator
            case 'U':
                move_up(a, player, id, &(*remain));
                if ((*remain) == 1) {
                    finish(n, player, &(*remain));
                    return;
                }
                break;
            case 'D':
                move_down(a, R, player, id, &(*remain));
                if ((*remain) == 1) {
                    finish(n, player, &(*remain));
                    return;
                }
                break;
            case 'L':
                move_left(a, player, id, &(*remain));
                if ((*remain) == 1) {
                    finish(n, player, &(*remain));
                    return;
                }
                break;
            case 'R':
                move_right(a, R, player, id, &(*remain));
                if ((*remain) == 1) {
                    finish(n, player, &(*remain));
                    return;
                }
                break;
        }
    }
}

void snowstorm(struct Glaciar **a, struct Fighter *player, int n,
int *remain) {
    int K, DMG, radius, X, Y;
    scanf("%d", &K);
    // obtin numerele din enunt
    DMG = K >> 24;
    if (DMG < 0) {
        DMG += 256;
    }
    radius = ((K << 8) >> 24);
    Y = ((K << 16) >> 24);
    X = ((K << 24) >> 24);
    // parcurg vectorul de pitici si scad hp-ul fiecarui jucator ramas pe
    // ghetar, elimin piticul daca are hp negativ
    for (int i = 0; i < n; i++) {
        if (player[i].stamina >= 0 && (X - player[i].line) * (X -
            player[i].line) + (Y - player[i].column) * (Y -
            player[i].column) <= radius * radius) {
            player[i].hp -= DMG;
            if (player[i].hp <= 0) {
                player[i].stamina = -1;
                printf("%s was hit by snowstorm.\n",
                player[i].name);
                (*remain)--;
                a[player[i].line][player[i].column].player = -1;
            }
        }
    }
}

void scoreboard(int n, struct Fighter *player) {
    struct Fighter *v;
    // v este copia vectorului de pitici, copie pe care o sortez dupa
    // criteriile din enunt (cele 3)
    v = malloc(n * sizeof(struct Fighter));
    for (int i = 0; i < n; i++) {
        v[i] = player[i];
    }
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (v[i].stamina == -1 && v[j]. stamina > -1) {
                swap_struct(&v[i], &v[j]);
            } else {
                if ((v[i].stamina == -1 && v[j].stamina == -1)
                || (v[i].stamina > -1 && v[j].stamina > -1)) {
                    if (v[i].kills < v[j].kills) {
                        swap_struct(&v[i], &v[j]);
                    } else {
                        if (v[i].kills == v[j].kills &&
                        strcmp(v[i].name, v[j].name) > 0) {
                            swap_struct(&v[i], &v[j]);
                        }
                    }
                }
            }
        }
    }
    // afisez numele, starea (DRY, WET) si numarul de kill-uri pentru fiecare
    printf("SCOREBOARD:\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t", v[i].name);
        if (v[i].stamina > -1) {
            printf("DRY\t");
        } else {
            printf("WET\t");
        }
        printf("%d\n", v[i].kills);
    }
    free(v);
}

struct Glaciar** meltdown(struct Glaciar **a, int *R, int n,
struct Fighter *player, int *remain) {
    // extra este satmina pe care o primesc jucatorii care nu sunt eliminati
    int extra;
    scanf("%d", &extra);
    // la fel ca la snowstorm, parcurg vectorul de pitici si ii elimin pe cei
    // care nu se gasesc in noul ghetar ce urmeaza sa fie alocat, piticii care
    // supravietuiesc se vor muta cu o unitate mai la stanga si o unitate mai
    // in sus si vor primi stamina extra
    for (int i = 0; i < n; i++) {
        if (player[i].stamina > -1) {
            if (((*R) - player[i].line) * ((*R) -
            player[i].line) + ((*R) - player[i].column) * ((*R) -
            player[i].column) > ((*R) - 1) * ((*R) - 1)) {
                player[i].stamina = -1;
                printf("%s got wet because of global warming.\n",
                player[i].name);
                a[player[i].line][player[i].column].player = -1;
                (*remain)--;
            } else {
                player[i].stamina += extra;
                player[i].line--;
                player[i].column--;
            }
        }
    }
    // 'mut' tot ghetarul mai in stanga-sus inainte sa realoc matricea
    for (int i = 1; i < 2 * (*R); i++) {
        for (int j = 1; j < 2 * (*R); j++) {
            a[i - 1][j - 1] = a[i][j];
        }
    }
    free(a[2 * (*R)]);
    free(a[2 * (*R) - 1]);
    (*R)--;
    a = realloc(a, (2 * (*R) + 1) * sizeof(struct Glaciar*));
    for (int i = 0; i < (2 * (*R) + 1); i++) {
        a[i] = realloc(a[i], (2 * (*R) + 1) * sizeof(struct Glaciar));
    }
    // refac campul care imi spune care celule sunt valide
    for (int i = 0; i < 2 * (*R) + 1; i++) {
        for (int j = 0; j < 2 * (*R) + 1; j++) {
            if (((*R) - i) * ((*R) - i) + ((*R) - j) * ((*R) - j) <=
            (*R) * (*R)) {
                a[i][j].is_on_map = 1;
            } else {
                a[i][j].is_on_map = 0;
            }
        }
    }
    return a;
}

void finish(int n, struct Fighter *player, int *remain) {
    // funcitia care afiseaza mesajul final apelata doar cand remain = 1
    for (int i = 0; i < n; i++) {
        if (player[i].stamina > -1) {
            printf("%s has won.\n", player[i].name);
        }
    }
    // remain = 0 inseamna ca jocul s-a terminat
    (*remain) = 0;
}

struct Glaciar** making_moves(struct Glaciar **a, int *R, int n,
struct Fighter *player, int remain) {
    char move[CHR];
    getchar();
    // citesc toate mutarile si apelez functiile care le va executa
    // fac acest lucru cat timp mai sunt mutari sau jocul nu este gata
    while (scanf("%s", move) != EOF) {
        if (remain == 1) {
            finish(n, player, &remain);
            break;
        }
        if (strcmp(move, "MOVE") == 0) {
            move_player(a, (*R), n, player, &remain);
            if (remain == 0) {
                break;
            }
        } else {
            if (strcmp(move, "SNOWSTORM") == 0) {
                snowstorm(a, player, n, &remain);
            } else {
                if (strcmp(move, "PRINT_SCOREBOARD") == 0) {
                    scoreboard(n, player);
                } else {
                    if (strcmp(move, "MELTDOWN") == 0) {
                        a = meltdown(a, &(*R), n, player, &remain);
                    }
                }
            }
        }
    }
    return a;
}
