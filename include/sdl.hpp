#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <memory>
#include <string>
#include <stdexcept>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void test();

// Wrapper for SDL
class SDLWrapper {
public:
  // Initialize unique_ptr deleter
  SDLWrapper() : window(nullptr, SDL_DestroyWindow) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error((std::string)"SDL could not initialize! SDL_Error: " + SDL_GetError());
    }
  }

  // We should switch to unique_ptr for these things ASAP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ~SDLWrapper() {
    // Destroy img
    if (welcomeImg) SDL_FreeSurface(welcomeImg);

    // Quit SDL subsystems
    SDL_Quit();
  }

  // Opens window and starts main game loop
  void startGame();

  // Loads an image (wont be in final product, only here for testing)
  void loadImg(std::string path);

private:
  // Opens the window
  void openWindow();

  // The window we'll be rendering to
  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;

  // The surface contained by the window
  SDL_Surface* screenSurface{ NULL };

  // Welcome image
  SDL_Surface* welcomeImg{ NULL };

  // Defines if the main game loop is ongoing
  bool gameLoop{ false };
};

#endif