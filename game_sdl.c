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
#define RES_SAVE "res/save.png"

/*-----------------------------------------------------------------*/

#define RES_RESTART "res/restart.png"
#define RES_REDO "res/redo.png"
#define RES_UNDO "res/undo.png"
#define RES_SOLVE "res/solve.png"
#define RES_RETOUR "res/retour.png"

/* **************************************************************** */

#define RES_ACCUEIL_BACK "res/menu/accueil.png"
#define RES_ACCUEIL_WRAP "res/menu/menu_wrap.png"
#define RES_ACCUEIL_WRAP_BIGTREE "res/menu/menu_wrap_bigtree.png"
#define RES_ACCUEIL_BIGTREE "res/menu/menu_bigtree.png"
#define RES_ACCUEIL_CLEAN "res/menu/menu_rien.png"

#define RES_LOAD_GAME "res/menu/load_game.png"

/* **************************************************************** */

/* PUT YOUR VARIABLES HERE */
typedef struct Frame {
  double x;
  double y;
  double height;
  double width;
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

  /* bouton */
  SDL_Texture* retour;
  SDL_Texture* restart;
  SDL_Texture* undo;
  SDL_Texture* redo;
  SDL_Texture* solve;

  /* save */
  SDL_Texture* save;
  char input[256];
  bool is_typing_s;
  SDL_Texture* text_s;
  bool retour_press;

  /* menu */
  char input_col[256];
  char input_row[256];
  bool is_typing_c;
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

  /* load */
  SDL_Texture* load_game;
  SDL_Texture* text_load;
  char input_load[256];
  bool write_load;
  bool is_typing_load;
};

/* **************************************************************** */
Frame* init_frame(double x, double y, double w, double h, double height_proportion, double width_proportion,
                  Frame* parent)
{
  Frame* f = malloc(sizeof(Frame));

  if (parent == NULL) {  // Si il n'y a pas de parent, alors le parent est la fenetre
    f->height = h * height_proportion;
    f->width = w * width_proportion;
  } else {
    f->height = parent->height * height_proportion;
    f->width = parent->width * width_proportion;
  }
  f->x = x;
  f->y = y;
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

    int real_x;
    int real_y;
    if (f->parent == NULL) {  // Si il n'y a pas de parent, alors le parent est la fenetre
      f->height = h * f->height_proportion;
      f->width = w * f->width_proportion;
      real_x = (int)(f->x * w);
      real_y = (int)(f->y * h);
    } else {
      f->height = f->parent->height * f->height_proportion;
      f->width = f->parent->width * f->width_proportion;
      real_x = (int)(f->x * f->parent->width);
      real_y = (int)(f->y * f->parent->height);
    }

    if (f->parent != NULL) {
      real_x += f->parent->rect.x;
      real_y += f->parent->rect.y;
    }

    SDL_Rect r = {real_x, real_y, f->width, f->height};
    f->rect = r;

    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    // SDL_RenderDrawRect(ren, &f->rect);
  }
}

static void update_text(SDL_Renderer* ren, TTF_Font* font, const char* input, SDL_Texture** texture)
{
  SDL_Color color = {0, 0, 0, 255};
  if (*texture != NULL) {
    SDL_DestroyTexture(*texture);
    *texture = NULL;
  }
  if (strlen(input) > 0) {
    SDL_Surface* test = TTF_RenderText_Blended(font, input, color);
    *texture = SDL_CreateTextureFromSurface(ren, test);
    SDL_FreeSurface(test);
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
  env->retour_press = false;

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

  /*------  Import des boutons du jeu -------*/
  env->restart = IMG_LoadTexture(ren, RES_RESTART);
  if (!env->restart) ERROR("IMG_LoadTexture: %s\n", RES_RESTART);

  env->undo = IMG_LoadTexture(ren, RES_UNDO);
  if (!env->undo) ERROR("IMG_LoadTexture: %s\n", RES_UNDO);

  env->redo = IMG_LoadTexture(ren, RES_REDO);
  if (!env->redo) ERROR("IMG_LoadTexture: %s\n", RES_REDO);

  env->solve = IMG_LoadTexture(ren, RES_SOLVE);
  if (!env->solve) ERROR("IMG_LoadTexture: %s\n", RES_SOLVE);

  /* init Winner texture from PNG image */
  env->retour = IMG_LoadTexture(ren, RES_RETOUR);
  if (!env->retour) ERROR("IMG_LoadTexture: %s\n", RES_RETOUR);

  /* init save texture from PNG image */
  env->save = IMG_LoadTexture(ren, RES_SAVE);
  if (!env->save) ERROR("IMG_LoadTexture: %s\n", RES_SAVE);

  /* ------- init ACCUEIL texture from PNG image --------------*/
  env->accueil_clean = IMG_LoadTexture(ren, RES_ACCUEIL_CLEAN);
  if (!env->accueil_clean) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_CLEAN);

  env->accueil_wrap = IMG_LoadTexture(ren, RES_ACCUEIL_WRAP);
  if (!env->accueil_wrap) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_WRAP);

  env->accueil_wrap_bigtree = IMG_LoadTexture(ren, RES_ACCUEIL_WRAP_BIGTREE);
  if (!env->accueil_wrap_bigtree) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_WRAP_BIGTREE);

  env->accueil_bigtree = IMG_LoadTexture(ren, RES_ACCUEIL_BIGTREE);
  if (!env->accueil_bigtree) ERROR("IMG_LoadTexture: %s\n", RES_ACCUEIL_BIGTREE);

  env->font = TTF_OpenFont(FONT, FONTSIZE);
  if (!env->font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(env->font, TTF_STYLE_BOLD);

  env->g = game_new_empty();
  env->back = 0;

  /*GESTION DES FRAMES*/
  env->nb_frame = 16;
  env->frame_list = malloc(sizeof(Frame*) * env->nb_frame);
  // frame_list[0] = fenetre
  env->frame_list[0] = init_frame(0, 0, w, h, 1, 1, NULL);

  env->frame_list[1] = init_frame(0.9, 0, w, h, 0.5, 0.1, env->frame_list[0]);

  /*bouton plateau*/
  env->frame_list[3] = init_frame(0.0, 0.0, w, h, 0.18, 1.0, env->frame_list[1]);

  env->frame_list[2] = init_frame(0.0, 0.2, w, h, 0.18, 1.0, env->frame_list[1]);

  env->frame_list[4] = init_frame(0.0, 0.4, w, h, 0.18, 1.0, env->frame_list[1]);

  env->frame_list[5] = init_frame(0.0, 0.6, w, h, 0.18, 1.0, env->frame_list[1]);

  env->frame_list[6] = init_frame(0.0, 0.8, w, h, 0.18, 1.0, env->frame_list[1]);

  /* panneau save*/
  env->frame_list[7] = init_frame(0.25, 0.2, w, h, 0.4, 0.5, env->frame_list[0]);
  env->frame_list[8] = init_frame(0.1, 0.15, w, h, 0.3, 0.8, env->frame_list[7]);
  env->frame_list[9] = init_frame(0.1, 0.6, w, h, 0.25, 0.35, env->frame_list[7]);
  env->frame_list[10] = init_frame(0.55, 0.6, w, h, 0.25, 0.35, env->frame_list[7]);

  // wraping
  env->frame_list[11] = init_frame(0.3, 0.73, w, h, 0.04, 0.12, env->frame_list[0]);
  // bigtree
  env->frame_list[12] = init_frame(0.3, 0.78, w, h, 0.04, 0.12, env->frame_list[0]);
  // Play
  env->frame_list[13] = init_frame(0.42, 0.35, w, h, 0.10, 0.18, env->frame_list[0]);
  // col
  env->frame_list[14] = init_frame(0.388, 0.543, w, h, 0.065, 0.087, env->frame_list[0]);
  //
  env->frame_list[15] = init_frame(0.525, 0.543, w, h, 0.065, 0.087, env->frame_list[0]);

  env->input[0] = '\0';
  env->is_typing_s = false;
  env->text_s = NULL;

  strcpy(env->input_col, "8");
  strcpy(env->input_row, "8");
  env->is_typing_c = false;
  env->text_col = NULL;
  env->text_row = NULL;
  env->write_col = false;

  update_text(ren, env->font, env->input_col, &env->text_col);
  update_text(ren, env->font, env->input_row, &env->text_row);

  /* ------- init LOAD texture from PNG image -------- */

  env->load_game = IMG_LoadTexture(ren, RES_LOAD_GAME);
  if (!env->load_game) ERROR("IMG_LoadTexture: %s\n", RES_LOAD_GAME);

  strcpy(env->input_load, "");
  env->write_load = false;
  env->text_load = NULL;
  env->is_typing_load = false;

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

static void update_number(SDL_Renderer* ren, Env* env, int nb_case, int other_nb, int taille_case, bool col, int x,
                          int y)
{
  SDL_Color color = {255, 255, 255, 255};
  for (int k = 0; k < nb_case; k++) {
    int expected;
    if (col)
      expected = game_get_expected_nb_tents_col(env->g, k);
    else
      expected = game_get_expected_nb_tents_row(env->g, k);
    char num[16];
    char i[16];
    sprintf(num, "%d", expected);
    sprintf(i, "%d", k);
    SDL_Surface* test = TTF_RenderText_Blended(env->font, num, color);
    SDL_Surface* test1 = TTF_RenderText_Blended(env->font, i, color);
    if (test != NULL) {
      SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, test);
      SDL_Texture* texture1 = SDL_CreateTextureFromSurface(ren, test1);

      SDL_Rect number_ex;
      SDL_Rect number;
      SDL_QueryTexture(texture, NULL, NULL, &number_ex.w, &number_ex.h);
      SDL_QueryTexture(texture1, NULL, NULL, &number.w, &number.h);

      if (col) {
        number.x = x + (k * taille_case) + (taille_case / 2) - number.w / 2;
        number.y = y - number.h - taille_case * 0.1;

        number_ex.x = x + (k * taille_case) + (taille_case / 2) - number.w / 2;
        number_ex.y = y + (other_nb * taille_case) + (taille_case / 2) - number.h / 2;

      } else {
        number.x = x - number.w - taille_case * 0.1;
        number.y = y + (k * taille_case) + (taille_case / 2) - number.h / 2;

        number_ex.x = x + (other_nb * taille_case) + (taille_case / 2) - number_ex.w / 2;
        number_ex.y = y + (k * taille_case) + (taille_case / 2) - number_ex.h / 2;
      }
      SDL_RenderCopy(ren, texture1, NULL, &number);
      SDL_RenderCopy(ren, texture, NULL, &number_ex);
      SDL_DestroyTexture(texture);
      SDL_DestroyTexture(texture1);
    }
    SDL_FreeSurface(test);
    SDL_FreeSurface(test1);
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
    int taille_case = (fmin(env->frame_list[0]->height, env->frame_list[0]->width)) / ((nb_cases_h + nb_cases_w) / 2) -
                      fmin(env->frame_list[0]->height, env->frame_list[0]->width) * marge;  // marge a adapter
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

    update_number(ren, env, nb_cases_w, nb_cases_h, taille_case, 1, x, y);
    update_number(ren, env, nb_cases_h, nb_cases_w, taille_case, 0, x, y);

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

    SDL_Rect solveur = env->frame_list[2]->rect;
    SDL_Rect retour = env->frame_list[3]->rect;
    SDL_Rect restart = env->frame_list[4]->rect;
    SDL_Rect undo = env->frame_list[5]->rect;
    SDL_Rect redo = env->frame_list[6]->rect;
    SDL_RenderCopy(ren, env->retour, NULL, &retour);
    SDL_RenderDrawRect(ren, &retour);
    SDL_RenderCopy(ren, env->solve, NULL, &solveur);
    SDL_RenderDrawRect(ren, &solveur);
    SDL_RenderCopy(ren, env->restart, NULL, &restart);
    SDL_RenderDrawRect(ren, &restart);
    SDL_RenderCopy(ren, env->undo, NULL, &undo);
    SDL_RenderDrawRect(ren, &undo);
    SDL_RenderCopy(ren, env->redo, NULL, &redo);
    SDL_RenderDrawRect(ren, &redo);

    if (game_won(env->g)) {
      SDL_Rect place;
      int w1, h1;
      SDL_QueryTexture(env->winner, NULL, NULL, &w1, &h1);
      place.w = w * 0.5;
      place.h = place.w * h1 / w1;
      place.x = (w) - (place.w);
      place.y = (h / 2) - (place.h / 2);
      SDL_RenderCopy(ren, env->winner, NULL, &place);

      SDL_Rect retour = {w * 0.7, h * 0.56, w * 0.1, h * 0.1};
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
      SDL_RenderDrawRect(ren, &retour);
    }
    if (env->retour_press) {
      SDL_Rect place = env->frame_list[7]->rect;
      SDL_Rect cadre_text = env->frame_list[8]->rect;
      SDL_Rect bouton_save = env->frame_list[9]->rect;
      SDL_Rect bouton_non_save = env->frame_list[10]->rect;

      SDL_RenderCopy(ren, env->save, NULL, &place);

      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
      SDL_RenderDrawRect(ren, &bouton_save);
      SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
      SDL_RenderDrawRect(ren, &bouton_non_save);
      SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
      SDL_RenderDrawRect(ren, &cadre_text);
      if (env->text_s != NULL) {
        SDL_Rect dest;
        SDL_QueryTexture(env->text_s, NULL, NULL, &dest.w, &dest.h);
        dest.x = cadre_text.x + (cadre_text.w - dest.w) / 2;
        dest.y = cadre_text.y + (cadre_text.h - dest.h) / 2;
        SDL_RenderCopy(ren, env->text_s, NULL, &dest);
      }
    }
  }

  if (env->back == 0) {
    SDL_Rect place;
    place.x = 0;
    place.y = 0;
    place.w = w;
    place.h = h;

    // SDL_Rect wrap = {w * 0.3, h * 0.73, w * 0.12, h * 0.04};
    // SDL_Rect bigtree = {w * 0.3, h * 0.78, w * 0.12, h * 0.04};
    // SDL_Rect play = {w * 0.41, h * 0.37, w * 0.18, h * 0.10};
    // SDL_Rect col = {w * 0.388, h * 0.543, w * 0.087, h * 0.065};
    // SDL_Rect row = {w * 0.525, h * 0.543, w * 0.087, h * 0.065};

    SDL_Rect wrap = env->frame_list[11]->rect;
    SDL_Rect bigtree = env->frame_list[12]->rect;
    SDL_Rect play = env->frame_list[13]->rect;
    SDL_Rect col = env->frame_list[14]->rect;
    SDL_Rect row = env->frame_list[15]->rect;

    if (env->bigtree_case && env->wrap_case)
      SDL_RenderCopy(ren, env->accueil_wrap_bigtree, NULL, &place);
    else if (env->bigtree_case)
      SDL_RenderCopy(ren, env->accueil_bigtree, NULL, &place);
    else if (env->wrap_case)
      SDL_RenderCopy(ren, env->accueil_wrap, NULL, &place);
    else
      SDL_RenderCopy(ren, env->accueil_clean, NULL, NULL);
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &col);
    SDL_RenderDrawRect(ren, &row);
    SDL_RenderDrawRect(ren, &play);
    SDL_RenderDrawRect(ren, &bigtree);
    SDL_RenderDrawRect(ren, &wrap);

    /* --------  on affiche le text --------*/
    if (env->text_col != NULL) {
      SDL_Rect dest;
      SDL_QueryTexture(env->text_col, NULL, NULL, &dest.w, &dest.h);
      dest.x = col.x + (col.w - dest.w) / 2;
      dest.y = col.y + (col.h - dest.h) / 2;
      SDL_RenderCopy(ren, env->text_col, NULL, &dest);
    }
    if (env->text_row != NULL) {
      SDL_Rect dest;
      SDL_QueryTexture(env->text_row, NULL, NULL, &dest.w, &dest.h);
      dest.x = row.x + (row.w - dest.w) / 2;
      dest.y = row.y + (row.h - dest.h) / 2;
      SDL_RenderCopy(ren, env->text_row, NULL, &dest);
    }
  }
  if (env->back == 2) {
    SDL_Rect place;
    place.x = 0;
    place.y = 0;
    place.w = w;
    place.h = h;

    SDL_RenderCopy(ren, env->load_game, NULL, &place);

    SDL_Rect charger = {w * 0.6, h * 0.45, w * 0.1, h * 0.08};
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &charger);

    SDL_Rect input_box = {w * 0.44, h * 0.45, w * 0.15, h * 0.08};
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderDrawRect(ren, &input_box);

    if (env->text_load != NULL) {
      SDL_Rect dest;
      SDL_QueryTexture(env->text_load, NULL, NULL, &dest.w, &dest.h);
      dest.x = input_box.x + 10;
      dest.y = input_box.y + (input_box.h - dest.h) / 2;
    }
  }
  update_frame(win, ren, env, w, h);
}

/* **************************************************************** */

/* PUT YOUR CODE HERE TO PROCESS EVENTS */
bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e)
{
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  /* generic events */
  if (e->type == SDL_QUIT) {
    return true;
  } else if (e->type == SDL_MOUSEBUTTONDOWN) {
    SDL_Point mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    if (env->back == 0) {
      if (e->button.button == SDL_BUTTON_LEFT) {
        SDL_Rect load = {w * 0.42, h * 0.72, w * 0.16, h * 0.08};

        SDL_Rect wrap = env->frame_list[11]->rect;
        SDL_Rect bigtree = env->frame_list[12]->rect;
        SDL_Rect play = env->frame_list[13]->rect;
        SDL_Rect col = env->frame_list[14]->rect;
        SDL_Rect row = env->frame_list[15]->rect;

        if (mouse.x >= wrap.x && mouse.x <= wrap.x + wrap.w && mouse.y >= wrap.y && mouse.y <= wrap.y + wrap.h)
          env->wrap_case = !env->wrap_case;
        else if (mouse.x >= bigtree.x && mouse.x <= bigtree.x + bigtree.w && mouse.y >= bigtree.y &&
                 mouse.y <= bigtree.y + bigtree.h)
          env->bigtree_case = !env->bigtree_case;
        else if ((mouse.x >= col.x && mouse.x <= col.x + col.w && mouse.y >= col.y && mouse.y <= col.y + col.h)) {
          env->is_typing_c = true;
          SDL_StartTextInput();
          env->write_col = true;
        } else if ((mouse.x >= row.x && mouse.x <= row.x + row.w && mouse.y >= row.y && mouse.y <= row.y + row.h)) {
          env->is_typing_c = true;
          SDL_StartTextInput();
          env->write_col = false;
        } else if (mouse.x >= play.x && mouse.x <= play.x + play.w && mouse.y >= play.y && mouse.y <= play.y + play.h) {
          // valeurs default
          int col = 8;
          int row = 8;
          // je regarde si on a mit des colonnes et des lignes
          if (strlen(env->input_col) > 0 && strlen(env->input_row) > 0) {
            col = atoi(env->input_col);
            row = atoi(env->input_row);
          }
          if (col >= 2 && row >= 2) {
            int nb_tree = col * row / 5;
            int nb_bigtree = 0;
            if (env->bigtree_case) nb_bigtree = rand() % 2 + 1;
            if (env->g != NULL) game_delete(env->g);
            env->g = game_random(row, col, env->wrap_case, nb_tree, nb_bigtree, false);
            env->back = 1;
          } else
            printf("error");

        } else if (mouse.x >= load.x && mouse.x <= load.x + load.w && mouse.y >= load.y && mouse.y <= load.y + load.h) {
          env->back = 2;
        }
      } else {
        env->is_typing_c = false;
        SDL_StopTextInput();
      }
    } else if (env->back == 1) {
      SDL_Rect retour = env->frame_list[3]->rect;
      SDL_Rect restart = env->frame_list[4]->rect;
      SDL_Rect solveur = env->frame_list[2]->rect;
      SDL_Rect undo = env->frame_list[5]->rect;
      SDL_Rect redo = env->frame_list[6]->rect;

      int nb_cases_w = env->g->nb_cols;
      int nb_cases_h = env->g->nb_rows;
      float marge = 0.05;
      int taille_case = (fmin(h, w)) / ((nb_cases_h + nb_cases_w) / 2) - fmin(h, w) * marge;  // marge a adapter
      int grille_w = nb_cases_w * taille_case;
      int grille_h = nb_cases_h * taille_case;

      int x = (w - grille_w) / 2;
      int y = (h - grille_h) / 2;
      if (game_won(env->g)) {
        if (e->button.button == SDL_BUTTON_LEFT) {
          SDL_Rect place;
          int w1, h1;
          SDL_QueryTexture(env->winner, NULL, NULL, &w1, &h1);
          place.w = w * 0.5;
          place.h = place.w * h1 / w1;
          place.x = (w / 2) - (place.w / 2);
          place.y = (h / 2) - (place.h / 2);

          env->bigtree_case = false;
          env->wrap_case = false;
          strcpy(env->input_col, "8");
          strcpy(env->input_row, "8");

          update_text(ren, env->font, env->input_col, &env->text_col);
          update_text(ren, env->font, env->input_row, &env->text_row);

          SDL_Rect bouton_retour;
          bouton_retour.w = place.w * 0.26;
          bouton_retour.h = place.h * 0.15;
          bouton_retour.x = w - place.w + (place.w * 0.37);
          bouton_retour.y = place.y + (place.h * 0.78) - 1.4 * (bouton_retour.h / 1.8);

          if (mouse.x >= bouton_retour.x && mouse.x <= bouton_retour.x + bouton_retour.w &&
              mouse.y >= bouton_retour.y && mouse.y <= bouton_retour.y + bouton_retour.h) {
            // revenir à l'écran d'accueil
            // env->g = game_default();
            env->back = 0;
          }
        }
      } else if (env->retour_press) {
        if (e->button.button == SDL_BUTTON_LEFT) {
          SDL_Rect cadre_text = env->frame_list[8]->rect;
          SDL_Rect bouton_save = env->frame_list[9]->rect;
          SDL_Rect bouton_non_save = env->frame_list[10]->rect;

          if (mouse.x >= cadre_text.x && mouse.x <= cadre_text.x + cadre_text.w && mouse.y >= cadre_text.y &&
              mouse.y <= cadre_text.y + cadre_text.h) {
            env->is_typing_s = true;
            SDL_StartTextInput();
          } else if (mouse.x >= bouton_non_save.x && mouse.x <= bouton_non_save.x + bouton_non_save.w &&
                     mouse.y >= bouton_non_save.y && mouse.y <= bouton_non_save.y + bouton_non_save.h) {
            env->retour_press = false;
            env->back = 0;
          } else if (mouse.x >= bouton_save.x && mouse.x <= bouton_save.x + bouton_save.w && mouse.y >= bouton_save.y &&
                     mouse.y <= bouton_save.y + bouton_save.h) {
            if (strlen(env->input) > 0) {
              game_save(env->g, env->input);
              env->retour_press = false;
              env->back = 0;
            }
          } else {
            env->is_typing_s = false;
            SDL_StopTextInput();
          }
        }
      } else {
        if (mouse.x >= x && mouse.x < x + grille_w && mouse.y >= y && mouse.y < y + grille_h) {
          int j = (mouse.x - x) / taille_case;
          int i = (mouse.y - y) / taille_case;
          if (e->button.button == SDL_BUTTON_LEFT) {
            if (game_get_square(env->g, i, j) == TENT) {
              game_play_move(env->g, i, j, EMPTY);
            } else if (game_check_move(env->g, i, j, TENT)) {
              game_play_move(env->g, i, j, TENT);
            }
          } else if (e->button.button == SDL_BUTTON_RIGHT) {
            if (game_get_square(env->g, i, j) == GRASS) {
              game_play_move(env->g, i, j, EMPTY);
            } else if (game_check_move(env->g, i, j, GRASS)) {
              game_play_move(env->g, i, j, GRASS);
            }
          }
        } else if (mouse.x >= solveur.x && mouse.x <= solveur.x + solveur.w && mouse.y >= solveur.y &&
                   mouse.y <= solveur.y + solveur.h) {
          game_restart(env->g);
          game_solve(env->g);
        } else if (mouse.x >= restart.x && mouse.x <= restart.x + restart.w && mouse.y >= restart.y &&
                   mouse.y <= restart.y + restart.h) {
          game_restart(env->g);
        } else if (mouse.x >= retour.x && mouse.x <= retour.x + retour.w && mouse.y >= retour.y &&
                   mouse.y <= retour.y + retour.h) {
          if (e->button.button == SDL_BUTTON_LEFT) {
            env->retour_press = true;
            env->input[0] = '\0';
            if (env->text_s != NULL) {
              SDL_DestroyTexture(env->text_s);
              env->text_s = NULL;
            }
          }
        } else if (mouse.x >= undo.x && mouse.x <= undo.x + undo.w && mouse.y >= undo.y && mouse.y <= undo.y + undo.h) {
          game_print(env->g);
          game_undo(env->g);
          game_print(env->g);
        } else if (mouse.x >= redo.x && mouse.x <= redo.x + redo.w && mouse.y >= redo.y && mouse.y <= redo.y + redo.h) {
          game_redo(env->g);
        }
      }
    } else if (env->back == 2) {
      SDL_Rect retour = {w * 0.41, h * 0.74, w * 0.18, h * 0.08};
      SDL_Rect charger = {w * 0.3, h * 0.73, w * 0.12, h * 0.04};
      SDL_Rect input_box = {w * 0.44, h * 0.45, w * 0.15, h * 0.08};

      if (e->button.button == SDL_BUTTON_LEFT) {
        if (mouse.x >= retour.x && mouse.x <= retour.x + retour.w && mouse.y >= retour.y &&
            mouse.y <= retour.y + retour.h) {
          env->back = 0;
          env->bigtree_case = false;
          env->wrap_case = false;
          strcpy(env->input_col, "8");
          strcpy(env->input_row, "8");

          update_text(ren, env->font, env->input_col, &env->text_col);
          update_text(ren, env->font, env->input_row, &env->text_row);
        }
      } else if (mouse.x >= input_box.x && mouse.x <= input_box.x + input_box.w && mouse.y >= input_box.y &&
                 mouse.y <= input_box.y + input_box.h) {
        env->is_typing_load = true;
        SDL_StartTextInput();
        env->write_load = true;
      } else if (mouse.x >= charger.x && mouse.x <= charger.x + charger.w && mouse.y >= charger.y &&
                 mouse.y <= charger.y + charger.h) {
        if (strlen(env->input_load) > 0) {
          if (env->g != NULL) {
            game_delete(env->g);
          }
          env->g = game_load(env->input_load);
          if (env->g != NULL) {
            env->back = 1;
          } else {
            printf("impossible load %s", env->input_load);
            env->g = game_default();
          }
        }
      } else {
        env->is_typing_load = false;
        SDL_StopTextInput();
        env->write_load = false;
      }
    }
  } else if (e->type == SDL_TEXTINPUT) {
    if (env->is_typing_c) {
      if (env->write_col) {
        if (strlen(env->input_col) + strlen(e->text.text) < 255) {
          strcat(env->input_col, e->text.text);
          update_text(ren, env->font, env->input_col, &env->text_col);
        }
      } else if (strlen(env->input_row) + strlen(e->text.text) < 255) {
        strcat(env->input_row, e->text.text);
        update_text(ren, env->font, env->input_row, &env->text_row);
      }
    } else if (env->is_typing_s) {
      if (strlen(env->input) + strlen(e->text.text) < 255) {
        strcat(env->input, e->text.text);
        update_text(ren, env->font, env->input, &env->text_s);
      }
    } else if (env->is_typing_load) {
      if (strlen(env->input_load) + strlen(e->text.text) < 255) {
        strcat(env->input_load, e->text.text);
        update_text(ren, env->font, env->input_load, &env->text_load);
      }
    }
  }
  // je gere la suppression de truc
  else if (e->type == SDL_KEYDOWN) {
    // bool valide = false;
    // if (env->write_col){
    //   if (strlen(env->write_col) > 0) valide = true;
    // }
    // else if (strlen(env->write_row) > 0) valide = true;
    if (e->key.keysym.sym == SDLK_BACKSPACE) {
      if (env->is_typing_c) {
        if (env->write_col && strlen(env->input_col) > 0) {
          env->input_col[strlen(env->input_col) - 1] = '\0';
          update_text(ren, env->font, env->input_col, &env->text_col);
        } else if (!env->write_col && strlen(env->input_row) > 0) {
          env->input_row[strlen(env->input_row) - 1] = '\0';
          update_text(ren, env->font, env->input_row, &env->text_row);
        }
      } else if (env->is_typing_s) {
        if (strlen(env->input) > 0) {
          env->input[strlen(env->input) - 1] = '\0';
          update_text(ren, env->font, env->input, &env->text_s);
        }
      } else if (env->is_typing_load) {
        if (strlen(env->input_load) > 0) {
          env->input_load[strlen(env->input_load) - 1] = '\0';
          update_text(ren, env->font, env->input_load, &env->text_load);
        }
      }
    } else if (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_KP_ENTER) {
      env->is_typing_c = false;
      env->is_typing_s = false;
      env->is_typing_load = false;
      SDL_StopTextInput();
    }
  }

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
  if (env->text_s) SDL_DestroyTexture(env->text_s);
  if (env->font) TTF_CloseFont(env->font);

  free(env);
}

/* **************************************************************** */
