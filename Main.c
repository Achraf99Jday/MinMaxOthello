#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "Othello.h"
#include "Affichage.h"
#include "ListePos.h"
#include "ArbreMiMa.h"

int main(int argc, char **argv)
  {
  int plateau[H][H];
  int Mode;
  char *ModeStr[] = {"2 joueurs",
  							"1 joueur auto",
							"2 joueurs auto",
							"1 joueur distant",
							"1 joueur distant un joueur auto"};
  int NbN, NbB;
  PosJouable_t *listeJouable;
  int joueurCourant = NOIR; //Toujours le noir commence
  int joueurLocal = NOIR;
  char *fichierDistant = NULL;
  char *fichierLocal = NULL;
  int i, j;
  int Niveau[2];
  int (*EvaluerPlateau[])(int plateau[H][H]) = {EvaluerPlateau_1, EvaluerPlateau_1};

	void print_usage()
		{
		fprintf(stderr, "Usage: Othello Mode ...\n Mode :");
		fprintf(stderr, "\t0 -> 2 joueurs: ");
		fprintf(stderr, "Othello 0\n");
		fprintf(stderr, "\t1 -> 1 joueur auto: ");
		fprintf(stderr, "Othello 1 Niveau_blanc\n");
		fprintf(stderr, "\t2 -> 2 joueurs auto: ");
		fprintf(stderr, "Othello 2 Niveau_noir Niveau_blanc\n");
		fprintf(stderr, "\t3 -> 1 joueurs contre distant: ");
		fprintf(stderr, "Othello 3 Couleur_Locale Fichier_Local Fichier_Distant\n");
		fprintf(stderr, "\t4 -> 1 joueurs auto contre distant: ");
		fprintf(stderr, "Othello 4 Couleur_Locale Fichier_Local Fichier_Distant Niveau_Local\n");
		}

	if (argc < 2)
		{
		print_usage();
		return 1;
		}

	Mode = atoi(argv[1]);

	switch(Mode)
		{
		case 0: break;
		case 1: 
			if (argc != 3)
				{
				print_usage();
				return 1;
				}
			Niveau[1] = atoi(argv[2]);
			break;
		case 2: 
			if (argc != 4)
				{
				print_usage();
				return 1;
				}
			Niveau[0] = atoi(argv[2]);
			Niveau[1] = atoi(argv[3]);
			break;
		case 3: /*joueur distant*/
			if (argc != 5)
				{
				print_usage();
				return 1;
				}
			if (strcmp(argv[2], "blanc") == 0) joueurLocal = BLANC;
			fichierLocal = argv[3];
			fichierDistant = argv[4];
			break;
		case 4: 
			if (argc != 6)
				{
				print_usage();
				return 1;
				}
			if (strcmp(argv[2], "blanc") == 0) joueurLocal = BLANC;
			fichierLocal = argv[3];
			fichierDistant = argv[4];
			Niveau[joueurLocal -1] = atoi(argv[5]);
			break;
		default:
			print_usage();
			return 1;
		}

  Initialiser_plateau(plateau);
  Creer_fenetre(ModeStr[Mode]);

	while (Partie_terminee(plateau) == 0)
		{
		if ((listeJouable = Trouver_liste_pos_jouables(plateau, joueurCourant)))
			{
			/*Joueur interactif*/
			if(Mode==0 || ( Mode == 1 && joueurCourant==NOIR) || ( Mode == 3 && joueurCourant==joueurLocal))
				{
				do
		  			{
		  			if(Loop_until_play(plateau, &i, &j, joueurCourant) == -1)
						{
						if (Mode == 3 || Mode == 4) remove(fichierLocal);
						listeJouable = Detruire_liste(listeJouable);
						Detruire_fenetre();
						return 0;
						}
		  			} while (!Est_dans_liste(listeJouable, i , j));
				}
			/*Joueur distant*/
			else if ((Mode == 3 || Mode == 4) && joueurCourant != joueurLocal)
					{
					FILE *fdistant = NULL;
					/*Test si fichier distant existe*/
					while ((fdistant = fopen(fichierDistant, "r")) == NULL) sleep(1);
               
					while ( fscanf(fdistant, "%d,%d", &i, &j) != 2)
						{
						fclose(fdistant);
						fdistant = fopen(fichierDistant, "r");
						}
					fclose(fdistant);
					
					/*Notification de lecture*/
					remove(fichierLocal);
					}
 			/*Joueur automatique*/
				else
					{
					sleep(1);
					NdMiMa_t *arbreMiMa = Construire_arbre(plateau, Niveau[joueurCourant -1], joueurCourant);
					MeilleurPos(arbreMiMa, plateau, EvaluerPlateau[joueurCourant -1], &i, &j);
					arbreMiMa = Detruire_arbre(arbreMiMa);
					}

			Jouer_pion(plateau, i, j, joueurCourant);
			Dessine_plateau_graph(plateau, joueurCourant);
			listeJouable = Detruire_liste(listeJouable);

			/*Envoie du jeu au distant*/
			if ((Mode == 3 || Mode == 4) && joueurCourant == joueurLocal)
				{
				FILE *fdistant = NULL;
				FILE *flocal = fopen(fichierLocal, "w");
				if (flocal == NULL)
					{
					fprintf(stderr, "Ouverture impossible du fichier local!\n");
					return 1;
					}
				fprintf(flocal, "%d,%d", i ,j);
				fclose(flocal);

				/*Test si fichier distant existe*/
				/*Si existe signifie que le distant n a pas encore pris en compte notre jeu*/
				while ((fdistant = fopen(fichierDistant, "r")))
					{
					fclose(fdistant);
					sleep(1);
					}
				}

			}

	joueurCourant = Autre_joueur(joueurCourant);
	}

  Nb_pions(plateau, &NbN, &NbB);
  printf("Partie terminee!\n");
  if (NbN > NbB)
    printf("Les noirs ont gagne par %d contre %d\n", NbN, NbB);
  else
    {
    if (NbN == NbB)
      printf("Ex-aequo par %d contre %d\n", NbN, NbB);
    else
      printf("Les blancs ont gagne par %d contre %d\n", NbB, NbN);
    }

	Loop_until_play(plateau, &i, &j, joueurCourant);
	if (Mode == 3 || Mode == 4) remove(fichierLocal);
	Detruire_fenetre();
	return 0;
}
