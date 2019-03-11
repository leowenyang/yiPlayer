#include <stdio.h>

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  int quit = 1;
  SDL_Window *window = NULL;
  SDL_Renderer *render = NULL;
  SDL_Texture *texture = NULL;

  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("could not initialize SDL - %s\n", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow("SDL2 Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window) {
    printf("could not create window - %s\n", SDL_GetError());
    return -1;
  }

  render = SDL_CreateRenderer(window, -1, 0);
  if (!render) {
    printf("could not create render  - %s\n", SDL_GetError());
    goto __dwindow;
  }

  texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);
  if (!texture) {
    printf("could not create texture - %s\n", SDL_GetError());
    goto __drender;
  }

  do {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_QUIT:
        quit = 0;
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            quit = 0;
        }
      default:
        SDL_Log("event type is %d", event.type);
    }

    SDL_Rect rect;
    rect.w = 30;
    rect.h = 30;
    rect.x = rand() % 600;
    rect.y = rand() % 400;

    // step 1 : render texture
    SDL_SetRenderTarget(render, texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    // step 2 : render texture->rect
    SDL_RenderDrawRect(render, &rect);
    SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
    SDL_RenderFillRect(render, &rect);

    // step 3 : copy data to VGA
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderCopy(render, texture, NULL, NULL);

    // step 4 : to display
    SDL_RenderPresent(render);

  } while(quit);

__texture:
  SDL_DestroyTexture(texture);

__drender:
  SDL_DestroyRenderer(render);

__dwindow:
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
