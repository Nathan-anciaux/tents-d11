// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

#define BACKGROUND "res/background.png"
#define FONT "res/arial.ttf"
#define GRAPH_TENT "res/tent.png"
#define GRAPH_TREE "res/tree.png"
#define GRAPH_BIGTREE "res/bigtree.png"

/* **************************************************************** */

struct Env_t {
  SDL_Texture* background;
  SDL_Texture* tent;
  SDL_Texture* tree;
  SDL_Texture* bigtree;
};

/* **************************************************************** */

Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[])
{
  Env* env = malloc(sizeof(struct Env_t));

  PRINT("Right help to show commands.\n");
  PRINT("Press ESC to quit.\n");
  w

      /* get current window size */
      int w,
      h;
  SDL_GetWindowSize(win, &w, &h);

  /* init background texture from PNG image */
  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  /* init tent texture from PNG image */
  env->tent = IMG_LoadTexture(ren, GRAPH_TENT);
  if (!env->tent) ERROR("IMG_LoadTexture: %s\n", GRAPH_TENT);

  /* init tree texture from PNG image */
  env->tree = IMG_LoadTexture(ren, GRAPH_TREE);
  if (!env->tree) ERROR("IMG_LoadTexture: %s\n", GRAPH_TREE);

  /* init bigtree texture from PNG image */
  env->bigtree = IMG_LoadTexture(ren, GRAPH_BIGTREE);
  if (!env->bigtree) ERROR("IMG_LoadTexture: %s\n", GRAPH_BIGTREE);

  /* init text texture using Arial font */
  SDL_Color color = {0, 0, 255, 255}; /* blue color in RGBA */
  TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);                                // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  SDL_Surface* surf = TTF_RenderText_Blended(font, "TENT GAME", color);  // blended rendering for ultra nice text
  env->text = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);
  TTF_CloseFont(font);

  return env;
}

/* **************************************************************** */

void render(SDL_Window* win, SDL_Renderer* ren, Env* env)
{ /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
}

/* **************************************************************** */

bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e)
{
  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env* env)
{
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}

/* **************************************************************** */

/* **************************************************************** */

int game_sdl(game g)
{
  /* initialize SDL2 and some extensions */
  if (SDL_Init(SDL_INIT_VIDEO) != 0) ERROR("Error: SDL_Init VIDEO (%s)", SDL_GetError());
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) ERROR("Error: IMG_Init PNG (%s)", SDL_GetError());
  if (TTF_Init() != 0) ERROR("Error: TTF_Init (%s)", SDL_GetError());

  /* create window and renderer */
  SDL_Window* win = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!win) ERROR("Error: SDL_CreateWindow (%s)", SDL_GetError());
  SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!ren) ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  if (!ren) ERROR("Error: SDL_CreateRenderer (%s)", SDL_GetError());

  /* initialize your environment */
  Env* env = init(win, ren, argc, argv);

  /* main render loop */
  SDL_Event e;
  bool quit = false;
  while (!quit) {
    /* manage events */
    while (SDL_PollEvent(&e)) {
      /* process your events */
      quit = process(win, ren, env, &e);
      if (quit) break;
    }

    /* background in gray */
    SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF);
    SDL_RenderClear(ren);

    /* render all what you want */
    render(win, ren, env);
    SDL_RenderPresent(ren);
    SDL_Delay(DELAY);
  }

  /* clean your environment */
  clean(win, ren, env);

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return EXIT_SUCCESS;
}
