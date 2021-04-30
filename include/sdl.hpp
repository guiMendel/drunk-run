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

  // Initialize window's & renderer's unique_ptr deleter
  SDLWrapper() : window(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer) {
    // Initialize SDL
    ensure(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL");
  }

  ~SDLWrapper() {
    // Quit SDL subsystems
    SDL_Quit();
  }

  // Opens window and starts main game loop
  void startGame();

  // If first parameter isn't truthy, throws error with provided message and SDL error log
  static void ensure(bool success, std::string errorMessage) {
    if (!success) throw std::runtime_error(errorMessage + "\n==> SDL error: " + SDL_GetError());
  }

private:
  // Opens the window
  void openWindow();

  // The window we'll be rendering to
  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;

  // Renderer for the window
  std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;

  // Defines if the main game loop is ongoing
  bool gameActive{ false };
};

#endif