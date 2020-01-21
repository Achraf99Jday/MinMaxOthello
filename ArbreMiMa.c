#include <stdlib.h>
#include <stdio.h>
#include "ArbreMiMa.h"
#include "ListePos.h"
#include "Othello.h"

int EvaluerPlateau_0(int plateau[H][H])
	{
	int i,j;
	int res =0;
	for(i=0;i<H;i++{
		for(j=0;j<H;j++){
			if(plateau[i][j]==NOIR){
				res++;
			}
			else if(plateau[i][j]==BLANC){
				res--;
			}		
		}
	}
	return res;
	}

int EvaluerPlateau_1(int plateau[H][H]){
	int i,j;
	int res =0;
	return res;
	}

NdMiMa_t *Construire_arbre(int plateau[H][H], int prof, int couleurQuiJoue)
	{
	if(prof>0){
	
		NdMiMa_t *nd=NULL;
		PosJouable_t *liste_pos=NULL;
		int new_plateau[H][H];
		if(Partie_terminee(plateau)) return NULL;
		nd=malloc(sizeof(NdMiMa_t));
		nd->Couleur=couleurQuiJoue;
		liste_pos=Trouver_liste_pos_jouables(plateau, couleurQuiJoue);
		if(liste_pos==NULL){
			nd->JoueurBloque=Construire_arbre(plateau,prof-1,Autre_joueur(couleurQuiJoue));
			nd->liste_pos=NULL;
			return NULL;
		}
		nd->JoueurBloque=NULL;
		nd->liste_pos=liste_pos;
		while(liste_pos){
			Copier_plateau(new_plateau,plateau);
			Jouer_pion(new_plateau, liste_pos->i, liste_pos->j,couleurQuiJoue);
			liste_pos->Nd=Construire_arbre(new_plateau, prof-1, Autre_joueur(couleurQuiJoue));
		}
		return nd;
	}
		
	return NULL;
}

int MinMax(NdMiMa_t *arbre, int plateau[H][H], int (*EvaluerPlateau)(int plateau[H][H]))
	{
	int min;
	int minmax;
	int new;
	PosJouable_t *liste;
	if(arbre==NULL)
	    return EvaluerPlateau(plateau);
	if(arbre->liste_pos==NULL){
	    if(arbre->JoueurBloque==NULL)
	        return EvaluerPlateau(plateau);
	    else
	        liste=arbre->JoueurBloque;
	}else
	    liste=arbre->liste_pos;
	Copier_plateau(new,plateau);
	Jouer_pion(new,liste->i,liste->j,arbre->Couleur);
	minmax=MinMax(liste->Nd,new,EvaluerPlateau);
	liste=liste->suiv;
	if(arbre->Couleur==NOIR){
	    int min=minmax;
	    while(liste){
	        Copier_plateau(new,plateau);
	        Jouer_pion(new,liste->i,liste->j,arbre->Couleur);
	        minmax=MinMax(liste->Nd,new,EvaluerPlateau);
	        if(minmax>min)
	            min=minmax;
	        liste=liste->suiv;
	}
	return new;
	}
	min=minmax;
	while(liste){
	       Copier_plateau(new,plateau);
	        Jouer_pion(new,liste->i,liste->j,arbre->Couleur);
	        minmax=MinMax(liste->Nd,new,EvaluerPlateau);
	        if(minmax>min)
	            min=minmax;
	        liste=liste->suiv;
	 }
	 return min;
	}

int MeilleurPos(NdMiMa_t *arbre, int plateau[H][H], int (*EvaluerPlateau)(int plateau[H][H]),int *pi, int *pj)
	{
	*pi = 0;
	*pj = 0;
	int min, minmax,max;
	int new[H][H];
	PosJouable_t *pos, *MeilleurPos;
	if(arbre->liste_pos==NULL) return 0;
	pos=arbre->liste_pos->suiv;
	MeilleurPos=arbre->liste_pos;
	Copier_plateau(new,plateau);
	Jouer_pion(new,MeilleurPos->i,MeilleurPos->j,arbre->Couleur);
	minmax=MinMax(MeilleurPos->Nd,new,EvaluerPlateau);
	if(arbre->Couleur=NOIR){
	    max=minmax;
	    while(pos){
	        Copier_plateau(new,plateau);
	        Jouer_pion(new,pos->i,pos->j,arbre->Couleur);
	        minmax=MinMax(pos->Nd,new,EvaluerPlateau);
	        if(max<minmax){
	            max=minmax;
	            MeilleurPos=pos;
	        }
	        pos=pos->suiv;
	    }
	    *pj=MeilleurPos->j;
	    *pi=MeilleurPos->i;
	    return 1;
	}
	min=max;
	while(pos){
	        Copier_plateau(new,plateau);
	        Jouer_pion(new,pos->i,pos->j,arbre->Couleur);
	        minmax=MinMax(pos->Nd,new,EvaluerPlateau);
	        if(min>minmax){
	            min=minmax;
	            MeilleurPos=pos;
	        }
	        pos=pos->suiv;
	    }
	    *pj=MeilleurPos->i;
	    *pi=MeilleurPos->j;
	    return 1;
}

NdMiMa_t *Detruire_arbre(NdMiMa_t *arbre)
	{
	if (arbre)
		{
		Detruire_liste(arbre->liste_pos);
		free(arbre);
		}
	return NULL;
	}
