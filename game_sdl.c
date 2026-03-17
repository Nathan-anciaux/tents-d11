// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "model.h"

/* ***************** implémentation du jeu ************************ */

#include "game.h"
#include "game_tools.h"
#include "game_struct.h"
#include "game_private.h"
#include "game_ext.h"
#include "game_aux.h"

/* **************************************************************** */

#define FONT "res/arial.ttf"
#define FONTSIZE 36
#define RES_BACKGROUND "res/background.png"
#define RES_TENT "res/tent.png"
#define RES_BIGTREE "res/bigtree.png"
#define RES_TREE "res/tree.png"
#define RES_BATON "res/baton.png"
#define RES_HERBE "res/HERBE.png"

/* **************************************************************** */

/* PUT YOUR VARIABLES HERE */
struct Env_t {
  SDL_Texture* background;
  SDL_Texture* tent;
  SDL_Texture* tree;
  SDL_Texture* bigtree;
  SDL_Texture* herbe;
  SDL_Texture* text;
  SDL_Texture* baton;
  game g;
//   int bomb_x, bomb_y;
//   int mario_x, mario_y;
};

/* **************************************************************** */

/* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {
  Env* env = malloc(sizeof(struct Env_t));

  PRINT("Move the bomb with keyboard arrows and move Mario with mouse.\n");
  PRINT("Press ESC to quit. Enjoy this SDL2 sample!\n");

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* init positions */
  // pas utile dans notre cas je pense a part pour la creation du tableau de la game
//   env->bomb_x = w / 2;
//   env->bomb_y = h / 4;
//   env->mario_x = w / 2;
//   env->mario_y = 3 * h / 4;

  /* init background texture from PNG image */
  env->background = IMG_LoadTexture(ren, RES_BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", RES_BACKGROUND);

  /* init tent texture from PNG image */
  env->tent = IMG_LoadTexture(ren, RES_TENT);
  if (!env->tent) ERROR("IMG_LoadTexture: %s\n", RES_TENT);

  /* init bigtree texture from PNG image */
  env->bigtree = IMG_LoadTexture(ren, RES_BIGTREE);
  if (!env->bigtree) ERROR("IMG_LoadTexture: %s\n", RES_BIGTREE);

  /* init TENT texture from PNG image */
  env->tree = IMG_LoadTexture(ren, RES_TREE);
  if (!env->tree) ERROR("IMG_LoadTexture: %s\n", RES_TREE);

  /* init GRASS texture from PNG image */
  env->herbe = IMG_LoadTexture(ren, RES_HERBE);
  if (!env->herbe) ERROR("IMG_LoadTexture: %s\n", RES_HERBE);
  
  /* init BATON texture from PNG image */
  env->baton = IMG_LoadTexture(ren, RES_BATON);
  if (!env->baton) ERROR("IMG_LoadTexture: %s\n", RES_BATON);

  env->g = game_load("saves/sample_5x5.txt");
  /* init text texture using Arial font */
    // ça sert juste a écrire le hello world moche au milieu
//   SDL_Color color = {0, 0, 255, 255}; /* blue color in RGBA */
//   TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
//   if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
//   TTF_SetFontStyle(font, TTF_STYLE_BOLD);                                   // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
//   SDL_Surface* surf = TTF_RenderText_Blended(font, "Hello World!", color);  // blended rendering for ultra nice text
//   env->text = SDL_CreateTextureFromSurface(ren, surf);
//   SDL_FreeSurface(surf);
//   TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

/* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
void render(SDL_Window* win, SDL_Renderer* ren, Env* env) {
  // SDL_Rect rect;

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  
  // pas de code en dur donc je met des variables pour chosisir
  int nb_cases_w = 5;
  int nb_cases_h = 5;
  float marge = 0.05;
  int taille_case = (fmin(h, w))/((nb_cases_h + nb_cases_w) /2) - fmin(h, w)*marge; // marge a adapter
  int grille_w = nb_cases_w * taille_case;
  int grille_h = nb_cases_h * taille_case;

  int x = (w - grille_w) / 2;
  int y = (h - grille_h) / 2;

  int epaisseur = taille_case * 1;
  if (epaisseur <4) epaisseur = 4;
  int gap = taille_case * 0.45;
  int taille_baton = taille_case + gap;

  int off = epaisseur /1.8;

  for (int i = 0; i < nb_cases_h; i++){
    for(int j=0; j<= nb_cases_w; j++){
        SDL_Rect place = {
            x+(j*taille_case) - off,
            y+(i*taille_case) - (gap /2 ),
            epaisseur,
            taille_baton
        };

        SDL_RenderCopy(ren, env->baton, NULL, &place);
    }
  }

  for (int i = 0; i <= nb_cases_h; i++){
    for(int j = 0; j< nb_cases_w; j++){
        SDL_Rect place;
        place.w = epaisseur;
        place.h = taille_baton;
        place.x = x + (j * taille_case) + (taille_case / 2) - off;
        place.y = y + (i * taille_case) - (taille_baton / 2);

        SDL_RenderCopyEx(ren, env->baton, NULL, &place, 90.0,NULL, SDL_FLIP_NONE);
    }
  }

    int marge_case = taille_case *0.1;
    for (int i = 0; i<nb_cases_w ;i++){
      for (int j = 0; j<nb_cases_h ;j++){
        SDL_Rect place;
        place.x = x + (j * taille_case) + marge_case;
        place.y = y + (i * taille_case) + marge_case;
        place.w = taille_case - (2 * marge_case);
        place.h = taille_case - (2 * marge_case);

        switch (game_get_square(env->g, i, j)){
          case TREE:
            SDL_RenderCopy(ren, env->tree, NULL, &place);
            break;
          case TENT:
            SDL_RenderCopy(ren, env->tent, NULL, &place);
            break;
          case GRASS:
            SDL_RenderCopy(ren, env->herbe, NULL, &place);
            break;
          case BIGTREE:
            SDL_RenderCopy(ren, env->bigtree, NULL, &place);
            break;
          default:
            break;
        }
      }
    }


//   SDL_Rect bg_grille = {x, y, grille_w, grille_h};
//   SDL_SetRenderDrawColor(ren, 255, 255, 255, 20);
//   SDL_RenderFillRect(ren, &bg_grille);

  // Dessin des cases

//   SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); // noir pour les lignes
//   for (int i = 0; i < nb_cases_h; i++) {
//     for (int j = 0; j <= nb_cases_w; j++) {
//         SDL_Rect cell;
//         cell.x = x + (j * taille_case);
//         cell.y = y + (i * taille_case);
//         cell.w = taille_case;
//         cell.h = taille_case;
        
//         // je dessine des rectangles
//         int epaisseur = taille_case * 0.8;
//         int gap = taille_case * 0.01;
//         int taille_baton = taille_case - (2*gap);

//         SDL_Rect rect_gauche = {cell.x, cell.y + gap, epaisseur, taille_baton};
//         SDL_RenderCopy(ren, env->baton, NULL, &rect_gauche);

//         SDL_Rect rect_droite = {cell.x + cell.w - epaisseur, cell.y + gap, epaisseur, taille_baton};
//         SDL_RenderCopyEx(ren, env->baton, NULL, &rect_droite, 0, NULL, SDL_FLIP_HORIZONTAL);

//         SDL_Rect rect_haut;
//         rect_haut.w = epaisseur;
//         rect_haut.h = taille_baton;
//         rect_haut.x = (cell.x + taille_case / 2) - (rect_haut.w / 2);
//         rect_haut.y = (cell.y + epaisseur / 2) - (rect_haut.h / 2);
//         SDL_RenderCopyEx(ren, env->baton, NULL, &rect_haut, 90, NULL, SDL_FLIP_NONE);

//         SDL_Rect rect_bas;
//         rect_bas.w = epaisseur;
//         rect_bas.h = taille_baton;
//         rect_bas.x = (cell.x + taille_case / 2) - (rect_bas.w / 2);
//         rect_bas.y = (cell.y + cell.x - epaisseur / 2) - (rect_bas.h / 2);
//         SDL_RenderCopyEx(ren, env->baton, NULL, &rect_bas, -90, NULL, SDL_FLIP_NONE);


        

//     }
//   }

   /* render text texture */
//   SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
//   rect.x = w / 2 - rect.w / 2;
//   rect.y = h / 2 - rect.h / 2;
//   SDL_RenderCopy(ren, env->text, NULL, &rect);

  /* render tent texture */
//   SDL_QueryTexture(env->tent, NULL, NULL, &rect.w, &rect.h);
//   rect.x = env->mario_x - rect.w / 2;
//   rect.y = env->mario_y - rect.h / 2;
//   SDL_RenderCopy(ren, env->mario, NULL, &rect);

  /* render bomb texture */
//   SDL_QueryTexture(env->bomb, NULL, NULL, &rect.w, &rect.h);
//   rect.x = env->bomb_x - rect.w / 2;
//   rect.y = env->bomb_y - rect.h / 2;
//   SDL_RenderCopy(ren, env->bomb, NULL, &rect);

  /* render a triangle with lines */
//   SDL_SetRenderDrawColor(ren, 255, 0, 0, SDL_ALPHA_OPAQUE); /* red */
//   SDL_RenderDrawLine(ren, 320, 200, 300, 240);
//   SDL_RenderDrawLine(ren, 300, 240, 340, 240);
//   SDL_RenderDrawLine(ren, 340, 240, 320, 200);

  /* render a circle with points */
//   const float pi = 3.14159265358979323846264338327950288419716939937510;
//   float step = 2 * pi / 300; /* smoothing */
//   for (float theta = 0.0; theta <= 2 * pi; theta += step) {
//     int x1 = 400 + 50 * cosf(theta) + 0.5;
//     int y1 = 400 + 50 * sinf(theta) + 0.5;
//     SDL_RenderDrawPoint(ren, x1, y1);
//   }
}

/* **************************************************************** */

/* PUT YOUR CODE HERE TO PROCESS EVENTS */
bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  int nb_cases_w = 5;
  int nb_cases_h = 5;
  float marge = 0.05;
  int taille_case = (fmin(h, w))/((nb_cases_h + nb_cases_w) /2) - fmin(h, w)*marge; // marge a adapter
  int grille_w = nb_cases_w * taille_case;
  int grille_h = nb_cases_h * taille_case;

  int x = (w - grille_w) / 2;
  int y = (h - grille_h) / 2;

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }
  /* Android events */
// #ifdef __ANDROID__
//   else if (e->type == SDL_FINGERDOWN) {
//     env->mario_x = e->tfinger.x * w; /* tfinger.x, normalized in [0..1] */
//     env->mario_y = e->tfinger.y * h; /* tfinger.y, normalized in [0..1] */
//   }
//   /* other events */
// #else
  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (mouse.x >= x && mouse.x < x + grille_w && mouse.y >= y && mouse.y < y + grille_h){
      int j = (mouse.x - x) / taille_case;
      int i = (mouse.y - y) / taille_case;
      if (e->button.button == SDL_BUTTON_LEFT){
        game_play_move(env->g, i, j, TENT);
      }
      else if (e->button.button == SDL_BUTTON_RIGHT){
        game_play_move(env->g, i, j, GRASS);
      } 
    }
  } 
  // else if (e->type == SDL_KEYDOWN) {
  //   switch (e->key.keysym.sym) {
  //     case SDLK_LEFT:
  //       env->bomb_x -= 10;
  //       break;
  //     case SDLK_RIGHT:
  //       env->bomb_x += 10;
  //       break;
  //     case SDLK_UP:
  //       env->bomb_y -= 10;
  //       break;
  //     case SDLK_DOWN:
  //       env->bomb_y += 10;
  //       break;
  //     case SDLK_ESCAPE:
  //       return true;
  //       break;
  //   }
  // }
// #endif

  return false; /* don't quit */
}

/* **************************************************************** */

/* PUT YOUR CODE HERE TO CLEAN MEMORY */
void clean(SDL_Window* win, SDL_Renderer* ren, Env* env) {
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->tent);
  SDL_DestroyTexture(env->tree);
  SDL_DestroyTexture(env->bigtree);
  SDL_DestroyTexture(env->baton);

  free(env);
}

/* **************************************************************** */
