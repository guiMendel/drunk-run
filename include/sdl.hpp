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
    // Destroy img
    if (welcomeImg) SDL_FreeSurface(welcomeImg);

    // Destroy window
    if (window != NULL) SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
  }

  // Opens window and starts main game loop
  void startGame();

  // Fill the surface white
  void loadImg() {
    welcomeImg = SDL_LoadBMP("hello_world.bmp");

    // Ensure image loaded
    if (!welcomeImg) {
      throw std::runtime_error((std::string)"SDL could not load img! SDL_Error: " + SDL_GetError());
    }

    // Print image
    SDL_BlitSurface(welcomeImg, NULL, screenSurface, NULL);

    // Update the surface
    SDL_UpdateWindowSurface(window);
  }

private:
  // Opens the window
  void openWindow();

  // The window we'll be rendering to
  SDL_Window* window{ NULL };

  // The surface contained by the window
  SDL_Surface* screenSurface{ NULL };

  // Welcome image
  SDL_Surface* welcomeImg{ NULL };

  // Defines if the main game loop is ongoing
  bool gameLoop{ false };
};

#endif