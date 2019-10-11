// Copyright Burcea Marian-Gabriel 2018
#include "./functions.h"

int main() {
    freopen("snowfight.in", "r", stdin);
    freopen("snowfight.out", "w", stdout);
    // R = raza ghetarului, n = numarul de jucatori, remain = numarul de
    // jucatori ramasi in joc
    int R, n, remain;
    // a este harta, player este vectorul de jucatori
    struct Glaciar **a = NULL;
    struct Fighter *player;
    scanf("%d%d", &R, &n);
    remain = n;
    a = malloc((2 * R + 1) * sizeof(struct Glaciar*));
    for (int i = 0; i < 2 * R + 1; i++) {
        a[i] = malloc((2 * R + 1) * sizeof(struct Glaciar));
    }
    read_data(a, R, n, &player, &remain);
    a = making_moves(a, &R, n, player, remain);
    for (int i = 0; i < 2 * R + 1; i++) {
        free(a[i]);
    }
    free(a);
    for (int i = 0; i < n; i++) {
        free(player[i].name);
    }
    free(player);
    return 0;
}
