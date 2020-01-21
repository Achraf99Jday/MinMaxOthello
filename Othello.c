#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Othello.h"


int Autre_joueur(int joueur)
  {
  if (joueur == 1) return 2;
  return 1;
  }

/*Le plateau est numerote a partor de 0,0 qui represente la case en haut a gauche*/

void Initialiser_plateau(int plateau[H][H]){
  int i,j;
  for(i=0;i<H;i++){
    for(j=0;j<H; j++){
      plateau[i][j]=VIDE;
    }
  }

  /*Config standard en croix*/
  plateau[H/2-1][H/2-1]=BLANC; 
  plateau[H/2][H/2]=BLANC; 
  plateau[H/2-1][H/2]=NOIR; 
  plateau[H/2][H/2-1]=NOIR; 

}

void Copier_plateau(int dst[H][H], int src[H][H])
	{
	int i,j;
	for(i=0;i<H;i++)
		for(j=0;j<H; j++)
			dst[i][j] = src[i][j];
	}


//Version 2 avec retour du nb de pions gagnes
//Compter et renvoyer le nombre de pions qui seront retournes pour une direction donnee
int Gain_dir(int plateau[H][H], int iLigne, int iCol, int dirLigne, int dirCol, int couleurQuiJoue)
  {
  int nbPionsGagnes = 0;

  do
    {
    iLigne += dirLigne;
    iCol += dirCol;

    if (iLigne >= H) return 0;
    if (iLigne < 0) return 0;
    if (iCol >= H) return 0;
    if (iCol < 0) return 0;

    if (plateau[iLigne][iCol] == couleurQuiJoue)  return nbPionsGagnes;
    nbPionsGagnes++;
    }
  while (plateau[iLigne][iCol] != VIDE);

  return 0;
  }

int Est_jouable_gain(int plateau[H][H], int iLigne, int iCol, int couleurQuiJoue)
	{
	int dirCol, dirLigne;
	
	for (dirCol=-1; dirCol<2; dirCol++)
		for (dirLigne=-1; dirLigne<2; dirLigne++)
			if(!( dirCol==0 && dirLigne==0 ))
				if (Gain_dir(plateau, iLigne, iCol, dirLigne, dirCol, couleurQuiJoue))
	 	  			return 1;
	
	return 0;
	}

int Partie_terminee(int plateau[H][H])
	{
	int i,j;
	for(i=0;i<H;i++)
		for(j=0;j<H; j++)
			if (plateau[i][j]==VIDE)
				{
				if(Est_jouable_gain(plateau, i, j, NOIR)) return 0;
				if(Est_jouable_gain(plateau, i, j, BLANC)) return 0;
				}
	return 1;
	}

//Renvoie la liste des position jouables
PosJouable_t *Trouver_liste_pos_jouables(int plateau[H][H], int couleurQuiJoue)
	{
	PosJouable_t *listePos=NULL;
	int i,j;
	for(i=0; i<H; i++){
		for(j=0; j<H; j++){
			if(plateau[i][j]==VIDE){
				if(Est_jouable_gain(plateau,i,j,couleurQuiJoue)){
					listePos=Inserer(listePos,i,j);
				}
			}
		}
	}
	
	return listePos;
	}

//Retourner les pions dans une direction donnee

void Retourner_pions(int plateau[H][H], int iLigne, int iCol, int dirLigne, int dirCol, int couleurQuiJoue)
	{
	iLigne += dirLigne;
	iCol += dirCol;
	while (plateau[iLigne][iCol] != couleurQuiJoue)
      {
      plateau[iLigne][iCol] = couleurQuiJoue;
      iLigne += dirLigne;
      iCol += dirCol;
      }
	}

void Jouer_pion(int plateau[H][H], int iLigne, int iCol, int couleurQuiJoue)
	{
	int dirCol, dirLigne;
	for (dirCol=-1; dirCol<2; dirCol++)
		for (dirLigne=-1; dirLigne<2; dirLigne++)
			if ( dirCol || dirLigne)
 				if(Gain_dir(plateau, iLigne, iCol, dirLigne, dirCol, couleurQuiJoue))
					Retourner_pions(plateau, iLigne, iCol, dirLigne, dirCol, couleurQuiJoue);
          
	plateau[iLigne][iCol] = couleurQuiJoue;
	return;
	}

void Nb_pions(int plateau[H][H], int *NbN, int *NbB)
	{
	int i,j;
	
	*NbB = 0;
	*NbN = 0;
	
	for(i=0;i<H;i++)
		for(j=0;j<H; j++)
			if (plateau[i][j])
				{
				if (plateau[i][j] == 1) (*NbN)++;
				else (*NbB)++;
				}
	}
