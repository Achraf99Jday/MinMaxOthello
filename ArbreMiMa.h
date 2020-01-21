#ifndef _ARB_MIMA_
#define _ARB_MIMA_ 

#include "Othello.h"

typedef struct _NdMiMa_t
	{
	int Couleur; // Blanc ou noir joue
	struct _posJouables_t *liste_pos;
	struct _NdMiMa_t *JoueurBloque;
	}  NdMiMa_t;

NdMiMa_t *Construire_arbre(int plateau[H][H], int prof, int couleurQuiJoue);
int MeilleurPos(NdMiMa_t *arbre, int plateau[H][H], int (*EvaluerPlateau)(int plateau[H][H]),int *pi, int *pj);
NdMiMa_t *Detruire_arbre(NdMiMa_t *arbre);
int EvaluerPlateau_0(int plateau[H][H]);
int EvaluerPlateau_1(int plateau[H][H]);

#endif
