#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void test();

// Wrapper for SDL
class SDLWrapper {
public:

  SDLWrapper();

  ~SDLWrapper() {
    // Destroy window
    if (window != NULL) SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
  }

  void fillWhite() {
    // Fill the surface white
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(window);
  }

private:
  // The window we'll be rendering to
  SDL_Window* window = NULL;

  // The surface contained by the window
  SDL_Surface* screenSurface = NULL;
};

#endif