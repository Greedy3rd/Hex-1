/* Simple program:  Create a blank window, wait for keypress, quit.
 
   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
 
//#include <SDL2/SDL.h> // ce serait pour la SDL 2
#include <SDL/SDL.h>
#include <SDL/SDL_keysym.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
 
#define WINHI 600 // window height
#define WINWI 800 // window width

typedef struct
{
    int h, w;
}Dimensions;
 
int main(int argc, char *argv[])
{
	int TabGrille[11][11];
	int i, j;
	for(i = 0; i < 11; i++)
	{
		for(j = 0; j < 11; j++)
		{
			TabGrille[i][j] = 0;
		}
	}
	
    Uint32 initflags = SDL_INIT_VIDEO; //| SDL_DOUBLEBUF;  /* See documentation for details */
    SDL_Surface *ecran, *rectangle, *board, *texte1, *txtJ1, *txtJ2; // quelques surfaces

	//SDL_WM_SetIcon(SDL_LoadBMP("Images/yolol.png"),NULL); Test ajout icone
	
    Dimensions fenetre; // main window
   
    fenetre.h = WINHI;  // en pixels
    fenetre.w = WINWI;   

    Uint8  video_bpp = 32; // 32 bits de couleur

    Uint32 videoflags = SDL_HWSURFACE; // utiliser la mémoire vidéo

    Uint32 vert, rouge, bleu, blanc; // des variables pour des couleurs

    bool continuer; // pour la boucle principale des évènements
    
    SDL_Event event; // aussi
 
    /* Initialize the SDL library */
    if ( SDL_Init(initflags) < 0 )
    {
        fprintf(stderr, "N'arrive pas a initialiser la SDL : %s\n", SDL_GetError());
        exit(1);
    }
 
    /* Set video mode */
    ecran = SDL_SetVideoMode(fenetre.w, fenetre.h, video_bpp, videoflags); // surface principale

    if (ecran == NULL)
    {
        fprintf(stderr, "N'arrive pas a etablir mode video%dx%dx%d : %s\n", fenetre.w, fenetre.h, video_bpp, SDL_GetError());
        SDL_Quit();
        exit(2);
    }

    vert = SDL_MapRGB(ecran->format,0,128,0); // c'est du vert
    rouge = SDL_MapRGB(ecran->format,255,0,0); // c'est du rouge
    bleu = SDL_MapRGB(ecran->format,102,204,255); // c'est du bleu
    blanc = SDL_MapRGB(ecran->format,255,255,255); // c'est du blanc

    SDL_FillRect(ecran,NULL,bleu); // de la couleur dans le fond de la fenêtre principale
    SDL_WM_SetCaption("~ Hex ~", NULL); // legende de la fenêtre

    // Une autre surface avec du texte
    TTF_Init();
    
    TTF_Font *fontMenu = TTF_OpenFont("CAC-Champagne/cac_champagne.ttf",42); // charger une fonte (un peu moche)
    SDL_Color fontBlack = {0,0,0};
    texte1 = TTF_RenderText_Blended(fontMenu,"Test du jeu Hex !",fontBlack);
	txtJ1 = TTF_RenderText_Blended(fontMenu,"Tour du Joueur 1 (Bleu)",fontBlack);
	txtJ2 = TTF_RenderText_Blended(fontMenu,"Tour du Joueur 2 (Rouge)",fontBlack);
	
    // encore une surface
    rectangle = SDL_CreateRGBSurface(initflags,225,80,video_bpp,0,0,0,0); // une autre surface pour inclure dans l'autre
    SDL_FillRect(rectangle,NULL,blanc); // un fond rouge

    // position des surfaces
    SDL_Rect posRect, posTexte1, posTxtJ;
    posRect.x = 20;
    posRect.y = 80;

    posTexte1.x = posRect.x+5;
    posTexte1.y = posRect.y+20;
    
    posTxtJ.x = 300;
    posTxtJ.y = 50;

    SDL_BlitSurface(rectangle,NULL,ecran,&posRect); // on associe avec la fenêtre principale
    SDL_BlitSurface(texte1,NULL,ecran,&posTexte1); // idem pour superposer avec la précédente

    // Affichage du plateau de jeu
    board = IMG_Load("Images/hex.png");    // plateau de jeu la taille en pixels est celle de l'image
    SDL_Rect posBoard;                     // sa position
    posBoard.x = 275;
    posBoard.y = 100;
    SDL_BlitSurface(board,NULL,ecran,&posBoard); // on l'associe avec la surface principale

    // affichage du pion bleu
    SDL_Surface *pionBleu = IMG_Load("Images/button-blue21.png");
    SDL_Rect posPionBleu;               
    /*posPionBleu.x = 303+45+30+30; // 377
    posPionBleu.y = 124+25; // 149
    SDL_BlitSurface(pionBleu2,NULL,ecran,&posPionBleu2);*/

    // Affichage du pion rouge
    SDL_Surface *pionRouge = IMG_Load("Images/button-red21.png");
    SDL_Rect posPionRouge;               
    /*posPionRouge.x = 543;
    posPionRouge.y = 273;
    SDL_BlitSurface(pionRouge,NULL,ecran,&posPionRouge);*/
    
    // Affichage du curseur
    SDL_Surface *curseur = IMG_Load("Images/curseur2.png");
    SDL_Rect posCurseur;               
    posCurseur.x = 291;
    posCurseur.y = 116;
    SDL_BlitSurface(curseur,NULL,ecran,&posCurseur);

	// TEST POSITION CURSEUR
	int PosCurX, PosCurY;
	PosCurX = 0;
	PosCurY = 0;
	
	// TEST TOUR JOUEUR
	int TourJoueur = 1; // 1 = J1 = BLEU // 2 = J2 = ROUGE
	
    SDL_Flip(ecran); //maj des surfaces
 	SDL_EnableKeyRepeat(150,150);
 // boucle des évènements
    continuer = true; // un furieux du c ferait plutôt une boucle infinie. Je préfère rester correct.
    SDLKey key_pressed;
    while (continuer)
    {
        while (SDL_PollEvent(&event)) 
        {
			SDL_FillRect(ecran,&posCurseur,32);
            switch (event.type)
            {
                case SDL_MOUSEMOTION:
                    //printf("Ça bouge\n"); // sans intérêt, c'est juste pour montrer
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_MIDDLE)
                    {   
                        int clicX = event.motion.x;
                        int clicY = event.motion.y;
                        printf("X=%d Y=%d\n",clicX,clicY); // on récupère les coordonnées du clic
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {   
                        if (event.motion.x > 20 &&
							event.motion.x <= 20 + 225 &&
							event.motion.y > 80 &&
							event.motion.y <= 80 + 80)
							printf("Ca fonctionne ! UN BOUTON !\n");
                    }
                    break;
                case SDL_KEYDOWN:
                    key_pressed = event.key.keysym.sym; // on récupère la touche
                    switch (key_pressed)
                    {
                    case SDLK_ESCAPE: /* Esc keypress quits the app... */
                        continuer = false;
                        break;
                    case SDLK_RETURN:
						if (!TabGrille[PosCurX][PosCurY])
						{
							printf("TEST\n");
							TabGrille[PosCurX][PosCurY] = TourJoueur;
							if (TourJoueur == 1)
								TourJoueur = 2;
							else
								TourJoueur = 1;
						}
						break;
                    case SDLK_LEFT:
                        printf("Left +1\n");
                        if (PosCurX > 0)
						{
							posCurseur.x -= 30;
							PosCurX--;
						}
                        break;
                    case SDLK_RIGHT:
                        printf("Right +1\n");
                        if (PosCurX < 10)
                        {
							posCurseur.x += 30;
							PosCurX++;
						}
                        break;
                    case SDLK_UP:
                        printf("Up +1\n");
                        if (PosCurY > 0)
                        {
							posCurseur.x -= 15;
							posCurseur.y -= 25;
							PosCurY--;
						}
                        break;
                    case SDLK_DOWN:
                        printf("Down +1\n");
                        if (PosCurY < 10)
                        {
							posCurseur.x += 15;
							posCurseur.y += 25;
							PosCurY++;
						}
                        break;
                    default:
						break;
                    }
                    break;
                case SDL_QUIT:
                    continuer = false;
                    break;
                default:
                    break;
            }
        }
        // refresh screen
        // mettre ici tous les blit utiles s'il y a des changements dans les surfaces, board, nouveaux pions
        SDL_FillRect(ecran,NULL,bleu);
        SDL_BlitSurface(rectangle,NULL,ecran,&posRect);
		SDL_BlitSurface(texte1,NULL,ecran,&posTexte1);
        SDL_BlitSurface(board,NULL,ecran,&posBoard);
        for(i = 0; i < 11; i++)
        {
			for(j = 0; j < 11; j++)
			{
				if (TabGrille[i][j] == 1)
				{
					posPionBleu.x = 298 + (i*30) + (j*15);
					posPionBleu.y = 124 + (j*25);
					SDL_BlitSurface(pionBleu,NULL,ecran,&posPionBleu);
				}
				if (TabGrille[i][j] == 2)
				{
					posPionRouge.x = 298 + (i*30) + (j*15);
					posPionRouge.y = 124 + (j*25);
					SDL_BlitSurface(pionRouge,NULL,ecran,&posPionRouge);
				}
			}
		}
        /*SDL_BlitSurface(pionBleu,NULL,ecran,&posPionBleu);
        SDL_BlitSurface(pionBleu2,NULL,ecran,&posPionBleu2);*/
        SDL_BlitSurface(curseur,NULL,ecran,&posCurseur);
        if (TourJoueur == 1) SDL_BlitSurface(txtJ1,NULL,ecran,&posTxtJ);
        else SDL_BlitSurface(txtJ2,NULL,ecran,&posTxtJ);
        SDL_Flip(ecran); //maj des surfaces pour affichage
    }
    //
    /* Clean up the SDL library */
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(rectangle);
    SDL_FreeSurface(board);
    TTF_CloseFont(fontMenu);
    SDL_FreeSurface(texte1);
    TTF_Quit();
    SDL_Quit();
    return(0);
}
