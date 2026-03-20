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
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "game_tools.h"

/* **************************************************************** */

#define FONT "res/menu/arial.ttf"
#define FONTSIZE 36
#define RES_BACKGROUND "res/background.png"
#define RES_TENT "res/tent.png"
#define RES_RED_TENT "res/red_tent.png"
#define RES_BIGTREE "res/bigtree.png"
#define RES_TREE "res/tree.png"
#define RES_WARNING "res/warning.png"
#define RES_BATON "res/baton.png"
#define RES_HERBE "res/HERBE.png"
#define RES_WINNER "res/congratulation.png"

/* **************************************************************** */

#define RES_ACCUEIL_BACK "res/menu/accueil.png"
#define RES_ACCUEIL_WRAP "res/menu/menu_wrap.png"
#define RES_ACCUEIL_WRAP_BIGTREE "res/menu/menu_wrap_bigtree.png"
#define RES_ACCUEIL_BIGTREE "res/menu/menu_bigtree.png"
#define RES_ACCUEIL_CLEAN "res/menu/menu_rien.png"

/* **************************************************************** */

/* PUT YOUR VARIABLES HERE */
typedef struct Frame {
  int x;
  int y;
  int height;
  int width;
  double height_proportion;
  double width_proportion;
  SDL_Rect rect;
  struct Frame* parent;

} Frame;

struct Env_t {
  SDL_Texture* background;
  SDL_Texture* tent;
  SDL_Texture* red_tent;
  SDL_Texture* tree;
  SDL_Texture* bigtree;
  SDL_Texture* warning;
  SDL_Texture* herbe;
  // SDL_Texture* text;
  SDL_Texture* baton;
  SDL_Texture* winner;
  Frame** frame_list;
  int nb_frame;
  int back;
  game g;
  //   int bomb_x, bomb_y;
  //   int mario_x, mario_y;

  /* menu */
  char input_col[256];
  char input_row[256];
  bool is_typing;
  bool write_col;
  SDL_Texture* text_col;
  SDL_Texture* text_row;
  SDL_Texture* accueil_clean;
  SDL_Texture* accueil_wrap;
  SDL_Texture* accueil_wrap_bigtree;
  SDL_Texture* accueil_bigtree;
  bool bigtree_case;
  bool wrap_case;
  TTF_Font* font;

};

/* **************************************************************** */
Frame* init_frame(int x, int y, int w, int h, double height_proportion, double width_proportion, Frame* parent)
{
  Frame* f = malloc(sizeof(Frame));
  f->x = x;
  f->y = y;

  if (parent == NULL) {  // Si il n'y a pas de parent, alors le parent est la fenetre
    f->height = h * height_proportion;
    f->width = w * width_proportion;
  } else {
    f->height = parent->height * height_proportion;
    f->width = parent->width * width_proportion;
  }
  SDL_Rect r = {x, y, f->width, f->height};
  f->height_proportion = height_proportion;
  f->width_proportion = width_proportion;
  f->rect = r;
  f->parent = parent;
  return f;
}

void update_frame(SDL_Window* win, SDL_Renderer* ren, Env* env, int w, int h)
{
  for (int i = 0; i < env->nb_frame; i++) {
    Frame* f = env->frame_list[i];

    if (f->parent == NULL) {  // Si il n'y a pas de parent, alors le parent est la fenetre
      f->height = h * f->height_proportion;
      f->width = w * f->width_proportion;
    } else {
      f->height = f->parent->height * f->height_proportion;
      f->width = f->parent->width * f->width_proportion;
    }
    SDL_Rect r = {f->x, f->y, f->width, f->height};
    f->rect = r;
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &f->rect);
  }
}

/* PUT YOUR CODE HERE TO INIT TEXTURES, ... */
Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[])
{
  Env* env = malloc(sizeof(struct Env_t));

  PRINT("Jeu créé par Evan, Maxence et Nathan.\n");
  PRINT("Sélectionne la taille de ta grille et clic sur play pour commencer a jouer.\n");
  PRINT("Press ESC to quit. Enjoy this Tents and trees game!\n");

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  env->wrap_case = false;
  env->bigtree_case = false;

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

  /* init red tent texture from PNG image */
  env->red_tent = IMG_LoadTexture(ren, RES_RED_TENT);
  if (!env->red_tent) ERROR("IMG_LoadTexture: %s\n", RES_RED_TENT);

  /* init warning texture from PNG image */
  env->warning = IMG_LoadTexture(ren, RES_WARNING);
  if (!env->warning) ERROR("IMG_LoadTexture: %s\n", RES_WARNING);

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

  /* init Winner texture from PNG image */
  env->winner = IMG_LoadTexture(ren, RES_WINNER);
  if (!env->winner) ERROR("IMG_LoadTexture: %s\n", RES_WINNER);

  /* init ACCUEIL texture from PNG image */
  env->accueil_clean = IMG_LoadTexture(ren, RES_ACCUEIL_CLEAN);
  if (!env->accueil_clean) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_CLEAN);

  env->accueil_wrap = IMG_LoadTexture(ren, RES_ACCUEIL_WRAP);
  if (!env->accueil_wrap) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_WRAP);

  env->accueil_wrap_bigtree = IMG_LoadTexture(ren, RES_ACCUEIL_WRAP_BIGTREE);
  if (!env->accueil_wrap_bigtree) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_WRAP_BIGTREE);

  env->accueil_bigtree = IMG_LoadTexture(ren, RES_ACCUEIL_BIGTREE);
  if (!env->accueil_bigtree) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_BIGTREE);

  env->font = TTF_OpenFont(FONT, FONTSIZE);
  if(!env->font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(env->font, TTF_STYLE_BOLD);

  env->g = game_new_empty();
  env->back = 0;

  env->nb_frame = 2;
  env->frame_list = malloc(sizeof(Frame*) * env->nb_frame);
  env->frame_list[0] = init_frame(w / 20, h / 20, w, h, 0.9, 0.9, NULL);
  env->frame_list[1] = init_frame(w / 15, h / 15, w, h, 0.5, 0.5, env->frame_list[0]);

  env->input_col[0] = '\0';
  env->input_row[0] = '\0';
  env->is_typing = false;
  env->text_col = NULL;
  env->text_row = NULL;
  env->write_col = false;
  
  /* init text texture using Arial font */
  // ça sert juste a écrire le hello world moche au milieu
  //   SDL_Color color = {0, 0, 255, 255}; /* blue color in RGBA */
  //   TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
  //   if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  //   TTF_SetFontStyle(font, TTF_STYLE_BOLD);                                   // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  //   SDL_Surface* surf = TTF_RenderText_Blended(font, "Hello World!", color);  // blended rendering for ultra nice
  //   text env->text = SDL_CreateTextureFromSurface(ren, surf); SDL_FreeSurface(surf); TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

// static void update_numero(SDL_Renderer* ren, TTF_Font* font, const char* input, SDL_Texture** texture){
//   SDL_Color color = {0, 0, 0, 255};
//   if (*texture != NULL){
//     SDL_DestroyTexture(*texture);
//     *texture = NULL; 
//   }
//   if (strlen(input) > 0){
//     SDL_Surface* test = TTF_RenderText_Blended(font, input, color);
//     *texture = SDL_CreateTextureFromSurface(ren, test);
//     SDL_FreeSurface(test);
//   }
// }

static void update_number(SDL_Renderer* ren, Env* env, int nb_case, int taille_case, bool col, int x, int y){
  SDL_Color color = {255,255,255,255};
  for (int k = 0; k<nb_case ;k++){
    int expected;
    if (col) expected = game_get_expected_nb_tents_col(env->g, k);
    else expected = game_get_expected_nb_tents_row(env->g, k);
    char num[10];
    sprintf(num, "%d",expected);
    SDL_Surface* test = TTF_RenderText_Blended(env->font, num, color);
    if (test!=NULL){
      SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, test);
      
      SDL_Rect number;
      SDL_QueryTexture(texture, NULL, NULL, &number.w, &number.h);
      if (col) {
        number.x = x + (k * taille_case) + (taille_case /2) - number.w /2;
        number.y = y - number.h - taille_case * 0.1;
      }
      else{
        number.x = x - number.w - taille_case * 0.1;
        number.y = y + (k * taille_case) + (taille_case /2) - number.h /2;
      }
      SDL_RenderCopy(ren, texture, NULL, &number);
      SDL_DestroyTexture(texture);
    }
    SDL_FreeSurface(test);
  }
}

/* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
void render(SDL_Window* win, SDL_Renderer* ren, Env* env)
{
  // SDL_Rect rect;

  /* get current window size */
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* render background texture */
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* stretch it */

  if (env->back == 1) {
    // pas de code en dur donc je met des variables pour chosisir
    int nb_cases_w = env->g->nb_cols;
    int nb_cases_h = env->g->nb_rows;
    float marge = 0.05;
    int taille_case = (fmin(h, w)) / ((nb_cases_h + nb_cases_w) / 2) - fmin(h, w) * marge;  // marge a adapter
    int grille_w = nb_cases_w * taille_case;
    int grille_h = nb_cases_h * taille_case;

    int x = (w - grille_w) / 2;
    int y = (h - grille_h) / 2;

    int epaisseur = taille_case * 1;
    if (epaisseur < 4) epaisseur = 4;
    int gap = taille_case * 0.45;
    int taille_baton = taille_case + gap;

    int off = epaisseur / 1.8;

    for (int i = 0; i < nb_cases_h; i++) {
      for (int j = 0; j <= nb_cases_w; j++) {
        SDL_Rect place = {x + (j * taille_case) - off, y + (i * taille_case) - (gap / 2), epaisseur, taille_baton};

        SDL_RenderCopy(ren, env->baton, NULL, &place);
      }
    }

    for (int i = 0; i <= nb_cases_h; i++) {
      for (int j = 0; j < nb_cases_w; j++) {
        SDL_Rect place;
        place.w = epaisseur;
        place.h = taille_baton;
        place.x = x + (j * taille_case) + (taille_case / 2) - off;
        place.y = y + (i * taille_case) - (taille_baton / 2);

        SDL_RenderCopyEx(ren, env->baton, NULL, &place, 90.0, NULL, SDL_FLIP_NONE);
      }
    }
    update_frame(win, ren, env, w, h);

    update_number(ren, env, nb_cases_w, taille_case, 1, x, y);
    update_number(ren, env, nb_cases_h, taille_case, 0, x, y);


    int marge_case = taille_case * 0.1;
    for (int i = 0; i < nb_cases_h; i++) {
      for (int j = 0; j < nb_cases_w; j++) {
        SDL_Rect place;
        place.x = x + (j * taille_case) + marge_case;
        place.y = y + (i * taille_case) + marge_case;
        place.w = taille_case - (2 * marge_case);
        place.h = taille_case - (2 * marge_case);

        switch (game_get_square(env->g, i, j)) {
          case TREE:
            SDL_RenderCopy(ren, env->tree, NULL, &place);
            break;
          case TENT:
            if ((game_get_current_nb_tents_col(env->g, j) <= game_get_expected_nb_tents_col(env->g, j)) &&
                (game_get_current_nb_tents_row(env->g, i) <= game_get_expected_nb_tents_row(env->g, i))) {
              if (game_is_neigh(env->g, i, j, TENT, true)) {
                SDL_RenderCopy(ren, env->warning, NULL, &place);
              } else {
                SDL_RenderCopy(ren, env->tent, NULL, &place);
              }
            } else {
              if (game_is_neigh(env->g, i, j, TENT, true)) {
                SDL_RenderCopy(ren, env->warning, NULL, &place);
              } else {
                SDL_RenderCopy(ren, env->red_tent, NULL, &place);
              }
            }
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

    if (game_won(env->g)) {
      SDL_Rect place;
      int w1, h1;
      SDL_QueryTexture(env->winner, NULL, NULL, &w1, &h1);
      place.w = w * 0.5;
      place.h = place.w * h1 / w1;
      place.x = (w) - (place.w);
      place.y = (h / 2) - (place.h / 2);
      SDL_RenderCopy(ren, env->winner, NULL, &place);

      SDL_Rect retour = {w * 0.7, h* 0.56, w * 0.1, h*0.1};
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
      SDL_RenderDrawRect(ren, &retour);
    }
    SDL_Rect solveur = {w * 0.9, h* 0.9, w * 0.1, h*0.1};
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &solveur);

    SDL_Rect restart = {w * 0.9, h* 0, w * 0.1, h*0.1};
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &restart);
  }

  if (env->back == 0){
    SDL_Rect place;
    place.x = 0;
    place.y = 0;
    place.w = w;
    place.h = h;
    SDL_Rect wrap = {w * 0.9, h* 0.9, w * 0.1, h*0.1};
    SDL_Rect bigtree = {w * 0.01, h* 0.9, w * 0.1, h*0.1};
    SDL_Rect play = {w * 0.01, h* 0.01, w * 0.1, h*0.1};
    // SDL_Rect load = {w * 0.9, h* 0.01, w * 0.1, h*0.1};
    SDL_Rect col = {w * 0.3, h* 0.5, w * 0.1, h*0.1};
    SDL_Rect row = {w * 0.6, h* 0.5, w * 0.1, h*0.1};
    if (env->bigtree_case && env->wrap_case) SDL_RenderCopy(ren, env->accueil_wrap_bigtree, NULL, &place);
    else if (env->bigtree_case) SDL_RenderCopy(ren, env->accueil_bigtree, NULL, &place);
    else if (env->wrap_case) SDL_RenderCopy(ren, env->accueil_wrap, NULL, &place);
    else SDL_RenderCopy(ren, env->accueil_clean, NULL, NULL);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &col);
    SDL_RenderDrawRect(ren, &row);
    SDL_RenderDrawRect(ren, &play);
    SDL_RenderDrawRect(ren, &bigtree);
    SDL_RenderDrawRect(ren, &wrap);
    
    /* --------  on affiche le text --------*/
    if (env->text_col != NULL){
      SDL_Rect dest;
      SDL_QueryTexture(env->text_col, NULL, NULL, &dest.w, &dest.h);
      dest.x = w*0.3;
      dest.y = h * 0.5;
      SDL_RenderCopy(ren, env->text_col, NULL, &dest);
    }
    if (env->text_row != NULL){
      SDL_Rect dest;
      SDL_QueryTexture(env->text_row, NULL, NULL, &dest.w, &dest.h);
      dest.x = w*0.6;
      dest.y = h * 0.5;
      SDL_RenderCopy(ren, env->text_row, NULL, &dest);
    }
  }
}

/* **************************************************************** */

static void update_text(SDL_Renderer* ren, TTF_Font* font, const char* input, SDL_Texture** texture){
  SDL_Color color = {0, 0, 0, 255};
  if (*texture != NULL){
    SDL_DestroyTexture(*texture);
    *texture = NULL; 
  }
  if (strlen(input) > 0){
    SDL_Surface* test = TTF_RenderText_Blended(font, input, color);
    *texture = SDL_CreateTextureFromSurface(ren, test);
    SDL_FreeSurface(test);
  }
}


/* PUT YOUR CODE HERE TO PROCESS EVENTS */
bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  }
  else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if(env->back == 0){
      if (e->button.button == SDL_BUTTON_LEFT){
      SDL_Rect wrap = {w * 0.9, h* 0.9, w * 0.1, h*0.1};
      SDL_Rect bigtree = {w * 0.01, h* 0.9, w * 0.1, h*0.1};
      SDL_Rect play = {w * 0.01, h* 0.01, w * 0.1, h*0.1};
      // SDL_Rect load = {w * 0.9, h* 0.01, w * 0.1, h*0.1};
      SDL_Rect col = {w * 0.3, h* 0.5, w * 0.1, h*0.1};
      SDL_Rect row = {w * 0.6, h* 0.5, w * 0.1, h*0.1};
      if (mouse.x >= wrap.x && mouse.x <= wrap.x + wrap.w && mouse.y >= wrap.y && mouse.y <= wrap.y + wrap.h) env->wrap_case = !env->wrap_case;
      else if (mouse.x >= bigtree.x && mouse.x <= bigtree.x + bigtree.w && mouse.y >= bigtree.y && mouse.y <= bigtree.y + bigtree.h) env->bigtree_case = !env->bigtree_case;
      else if ((mouse.x >= col.x && mouse.x <= col.x + col.w && mouse.y >= col.y && mouse.y <= col.y + col.h)) {
        env->is_typing = true;
        SDL_StartTextInput();
        env->write_col = true;
      }
      else if ((mouse.x >= row.x && mouse.x <= row.x + row.w && mouse.y >= row.y && mouse.y <= row.y + row.h)) {
        env->is_typing = true;
        SDL_StartTextInput();
        env->write_col = false;
      }
      else if (mouse.x >= play.x && mouse.x <= play.x + play.w && mouse.y >= play.y && mouse.y <= play.y + play.h) {
        // je regarde si on a mit des colonnes et des lignes
        if (strlen(env->input_col) > 0 && strlen(env->input_row)>0){
          int col = atoi(env->input_col);
          int row = atoi(env->input_row);
          if (col >=2 && row>=2){
            int nb_tree = col * row / 5;
            int nb_bigtree = 0;
            if (env->bigtree_case) nb_bigtree = rand() % 2 + 1;
            if (env->g != NULL) game_delete(env->g);
            env->g = game_random(row, col, env->wrap_case, nb_tree, nb_bigtree, false);
            env->back = 1;
          }
          else printf("error");
        }
        }
      }
      else{
        env->is_typing = false;
        SDL_StopTextInput();
      }
    }
    else if (env->back == 1){
      SDL_Rect restart = {w * 0.9, 0, w * 0.1, h*0.1};
      SDL_Rect solveur = {w * 0.9, h* 0.9, w * 0.1, h*0.1};
      int nb_cases_w = env->g->nb_cols;
      int nb_cases_h = env->g->nb_rows;
      float marge = 0.05;
      int taille_case = (fmin(h, w))/((nb_cases_h + nb_cases_w) /2) - fmin(h, w)*marge; // marge a adapter
      int grille_w = nb_cases_w * taille_case;
      int grille_h = nb_cases_h * taille_case;

      int x = (w - grille_w) / 2;
      int y = (h - grille_h) / 2;
      if (game_won(env->g)){
        if (e->button.button == SDL_BUTTON_LEFT){
          SDL_Rect place;
          int w1, h1;
          SDL_QueryTexture(env->winner, NULL, NULL, &w1, &h1);
          place.w = w * 0.5;
          place.h = place.w * h1 / w1;
          place.x = (w / 2) - (place.w /2);
          place.y = (h / 2) - (place.h/2);

          env->bigtree_case = false;
          env->wrap_case = false;
          env->input_col[0] = '\0';
          env->input_row[0] = '\0';
          
          SDL_DestroyTexture(env->text_col);
          env->text_col = NULL;
          SDL_DestroyTexture(env->text_row);
          env->text_row = NULL;

          SDL_Rect bouton_retour;
          bouton_retour.w = place.w * 0.26;
          bouton_retour.h = place.h * 0.15;
          bouton_retour.x = w - place.w + (place.w * 0.37);
          bouton_retour.y = place.y + (place.h * 0.78) - 1.4 * (bouton_retour.h/1.8);

          if (mouse.x >= bouton_retour.x && mouse.x <= bouton_retour.x + bouton_retour.w && mouse.y >= bouton_retour.y && mouse.y <= bouton_retour.y + bouton_retour.h){
            // revenir à l'écran d'accueil
            // env->g = game_default();
            env->back = 0;
          }
        }
      }
      else {
        if (mouse.x >= x && mouse.x < x + grille_w && mouse.y >= y && mouse.y < y + grille_h){
          int j = (mouse.x - x) / taille_case;
          int i = (mouse.y - y) / taille_case;
          if (e->button.button == SDL_BUTTON_LEFT){
            if (game_get_square(env->g, i, j) == TENT){
              game_play_move(env->g,i,j, EMPTY);
            }
            else if (game_check_move(env->g,i,j,TENT)){
              if ((game_get_current_nb_tents_col(env->g,j) <= game_get_expected_nb_tents_col(env->g,j)) && (game_get_current_nb_tents_row(env->g,i) <= game_get_expected_nb_tents_row(env->g,i))) {
                game_play_move(env->g, i, j, TENT);
              }
            }
          } else if (e->button.button == SDL_BUTTON_RIGHT) {
            if (game_get_square(env->g, i, j) == GRASS){
              game_play_move(env->g,i,j, EMPTY);
            }
            else if (game_check_move(env->g,i,j,GRASS)){
              game_play_move(env->g, i, j, GRASS);
            }
          }
        }
        else if (mouse.x >= solveur.x && mouse.x <= solveur.x + solveur.w && mouse.y >= solveur.y && mouse.y <= solveur.y + solveur.h){
          game_restart(env->g);
          game_solve(env->g);
        }
        else if (mouse.x >= restart.x && mouse.x <= restart.x + restart.w && mouse.y >= restart.y && mouse.y <= restart.y + restart.h){
          game_restart(env->g);
        }
      }
    }
  }
  else if (e->type == SDL_TEXTINPUT && env->is_typing){
    if (env->write_col){
      if(strlen(env->input_col) + strlen(e->text.text)<255){
        strcat(env->input_col, e->text.text);
        update_text(ren, env->font, env->input_col, &env->text_col);
      }
    }
    else{
      if(strlen(env->input_row) + strlen(e->text.text)<255){
        strcat(env->input_row, e->text.text);
        update_text(ren, env->font, env->input_row, &env->text_row);
      }
    }
  } 
  // je gere la suppression de truc
  else if (e->type == SDL_KEYDOWN && env->is_typing){
    // bool valide = false;
    // if (env->write_col){
    //   if (strlen(env->write_col) > 0) valide = true;
    // }
    // else if (strlen(env->write_row) > 0) valide = true;
    if(e->key.keysym.sym == SDLK_BACKSPACE){
      if (env->write_col && strlen(env->input_col)>0){
        env->input_col[strlen(env->input_col)-1] = '\0';
        update_text(ren, env->font, env->input_col, &env->text_col);
      }
      else if (!env->write_col && strlen(env->input_row)>0) {
        env->input_row[strlen(env->input_row)-1] = '\0';
        update_text(ren, env->font, env->input_row, &env->text_row);
      }
    }
    else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER){
      env->is_typing = false;
      SDL_StopTextInput();
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
void clean(SDL_Window* win, SDL_Renderer* ren, Env* env)
{
  SDL_DestroyTexture(env->background);
  SDL_DestroyTexture(env->tent);
  SDL_DestroyTexture(env->tree);
  SDL_DestroyTexture(env->red_tent);
  SDL_DestroyTexture(env->bigtree);
  SDL_DestroyTexture(env->baton);
  SDL_DestroyTexture(env->herbe);
  SDL_DestroyTexture(env->winner);
  SDL_DestroyTexture(env->accueil_clean);
  SDL_DestroyTexture(env->accueil_wrap);
  SDL_DestroyTexture(env->accueil_bigtree);
  SDL_DestroyTexture(env->accueil_wrap_bigtree);

  if (env->text_col) SDL_DestroyTexture(env->text_col);
  if (env->text_row) SDL_DestroyTexture(env->text_row);
  if (env->font) TTF_CloseFont(env->font);

  free(env);
}

/* **************************************************************** */
