#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

/* Structure hétérogène */
typedef struct {
    int id;
    float x, y, z;
} Drone;

/* Calcul de distance par arithmétique de pointeurs pure */
float calculerDistance(Drone *d1, Drone *d2) {
    float dx = (*d1).x - (*d2).x;
    float dy = (*d1).y - (*d2).y;
    float dz = (*d1).z - (*d2).z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

/* Comparateur pour qsort */
int cmp_par_x(const void *a, const void *b) {
    const Drone *da = (const Drone *)a;
    const Drone *db = (const Drone *)b;
    if ((*da).x < (*db).x) return -1;
    if ((*da).x > (*db).x) return  1;
    return 0;
}

int main() {
    int N = 10000; /* Taille de l'essaim */
    srand(time(NULL));

    /* Allocation dynamique sur le Tas  */
    Drone *essaim = (Drone *)malloc(N * sizeof(Drone));
    if (essaim == NULL) return 1;

    /* Initialisation via arithmétique de pointeurs */
    for (int i = 0; i < N; i++) {
        Drone *actuel = (essaim + i);
        (*actuel).id = i;
        (*actuel).x = (float)rand() / (float)RAND_MAX * 500.0f;
        (*actuel).y = (float)rand() / (float)RAND_MAX * 500.0f;
        (*actuel).z = (float)rand() / (float)RAND_MAX * 500.0f;
    }

    /* Tri O(n log n) */
    qsort(essaim, N, sizeof(Drone), cmp_par_x);

    float distanceMin = FLT_MAX;
    Drone *d1_final = NULL;
    Drone *d2_final = NULL;

    /* Algorithme de fenêtre glissante optimisé */
    Drone *ptr_g = essaim;  
    Drone *ptr_i;           
    Drone *ptr_j;           

    for (ptr_i = essaim + 1; ptr_i < essaim + N; ++ptr_i) {
        /* On avance la borne gauche si l'écart en X dépasse déjà distanceMin */
        while (((*ptr_i).x - (*ptr_g).x) >= distanceMin) {
            ++ptr_g;
        }

        /* Comparaison interne limitée par la fenêtre spatiale */
        for (ptr_j = ptr_g; ptr_j < ptr_i; ++ptr_j) {
            float d = calculerDistance(ptr_i, ptr_j);
            if (d < distanceMin) {
                distanceMin = d;
                d1_final = ptr_i;
                d2_final = ptr_j;
            }
        }
    }

    printf("Alerte Collision : Drone %d et Drone %d | Distance: %.4f\n",
            (*d1_final).id, (*d2_final).id, distanceMin);

    free(essaim);
    return 0;
}