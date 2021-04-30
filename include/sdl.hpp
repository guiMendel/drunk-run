#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <memory>
#include <string>
#include <stdexcept>

void test();

// Wrapper for SDL
class SDLWrapper {
public:
  // Public constants
  static const int SCREEN_WIDTH = 800;
  static const int SCREEN_HEIGHT = 600;

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
    if (imageBuffer) SDL_FreeSurface(imageBuffer);

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

  // Texture renderer for the window
  SDL_Renderer* windowRenderer{ NULL };

  // The surface contained by the window
  SDL_Surface* screenSurface{ NULL };

  // Image buffer
  SDL_Surface* imageBuffer{ NULL };

  // Defines if the main game loop is ongoing
  bool gameActive{ false };
};

#endif