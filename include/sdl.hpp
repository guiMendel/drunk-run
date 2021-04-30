#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <string>
#include <stdexcept>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void test();

// Wrapper for SDL
class SDLWrapper {
public:

  SDLWrapper() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error((std::string)"SDL could not initialize! SDL_Error: " + SDL_GetError());
    }
  }

  ~SDLWrapper() {
    // Destroy window
    if (window != NULL) SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
  }

  // Opens window and starts main game loop
  void startGame();

  void fillWhite() {
    // Fill the surface white
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

    // Update the surface
    SDL_UpdateWindowSurface(window);
  }

private:
  // Opens thw window
  void openWindow();

  // The window we'll be rendering to
  SDL_Window* window {NULL};

  // The surface contained by the window
  SDL_Surface* screenSurface {NULL};

  // Defines if the main game loop is ongoing
  bool gameLoop {false};
};

#endif