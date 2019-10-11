// Copyright Burcea Marian-Gabriel 2018
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef functions_h
#define functions_h

#define NMAX 1000
#define CHR 16

struct Glaciar{
    int hill;
    int gloves;
    // campul player contine indicele jucatorului care se gaseste in celula
    int player;
    // is_on_map este 1 daca celula este pe harta, 0 in caz contrar
    int is_on_map;
};

struct Fighter{
    char *name;
    int line;
    int column;
    int hp;
    int stamina;
    int gloves;
    int kills;
};

void swaps(int *a, int *b);
void swap_struct(struct Fighter *a, struct Fighter *b);
void read_data(struct Glaciar **a, int R, int n, struct Fighter **player,
    int *remain);
void print_data(struct Glaciar **a, int R, int n, struct Fighter *player,
    int remain);
int fight_winner(struct Fighter *player, int fighter1, int fighter2,
    int *remain);
void move_up(struct Glaciar **a, struct Fighter *player, int id, int *remain);
void move_down(struct Glaciar **a, int R, struct Fighter *player, int id,
    int *remain);
void move_left(struct Glaciar **a, struct Fighter *player, int id, int *remain);
void move_right(struct Glaciar **a, int R, struct Fighter *player, int id,
    int *remain);
void move_player(struct Glaciar **a, int R, int n, struct Fighter *player,
    int *remain);
void snowstorm(struct Glaciar **a, struct Fighter *player, int n, int *remain);
void scoreboard(int n, struct Fighter *player);
struct Glaciar** meltdown(struct Glaciar **a, int *R, int n,
    struct Fighter *player, int *remain);
void finish(int n, struct Fighter *player, int *remain);
struct Glaciar** making_moves(struct Glaciar **a, int *R, int n,
    struct Fighter *player, int remain);

#endif
