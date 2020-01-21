#ifndef _LISTEPOS_H_
#define _LISTEPOS_H_ 

typedef struct _posJouables_t
	{
	int i;
	int j;
	struct _posJouables_t *suiv;
	struct _NdMiMa_t *Nd;
	} PosJouable_t;
PosJouable_t *Inserer(PosJouable_t *liste_pos, int posi, int posj);

void Afficher_lPosJouables(PosJouable_t *liste_pos);

PosJouable_t *Detruire_liste(PosJouable_t *liste_pos);
int Est_dans_liste(PosJouable_t *liste_pos, int i, int j);

//int len(cellule_t* liste);


#endif
