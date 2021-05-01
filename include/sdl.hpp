#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <memory>
#include <string>
#include <stdexcept>

#define CAMERA_HEIGHT 300

// Wrapper for SDL
class SDLWrapper {
public:
  // Initialize window's & renderer's unique_ptr deleter
  SDLWrapper(int width, int height) :
    screenWidth(width),
    screenHeight(height),
    window(nullptr, SDL_DestroyWindow),
    renderer(nullptr, SDL_DestroyRenderer) {
    // Initialize SDL
    ensure(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL");
    // Calculate eye distance using tan(30) & screen width
    eyeDistance = (screenWidth / 2.0) / 0.57735026919;
  }

  ~SDLWrapper() {
    // Quit SDL subsystems
    SDL_Quit();
  }

  //////////////////////// WINDOW

  // Opens the window
  void openWindow();

  // Handles current event queue with provided function. Accepts a context to resolve methods.
  void resolveEvents(void(*eventHandler)(void*, SDL_Event&), void*);

  //////////////////////// GEOMETRY

  // Draw some random shapes (MEANT FOR TESTING)
  void drawShapes(float relativeDistance);

  // Draws a rectangle relative to it's distance from screen
  void drawObstacle(int bottomLeft, int width, int height, int distance);

  // Applies perspective to a point
  int perspective(int point, int distance) { return point * eyeDistance / (eyeDistance + distance); }

  // Converts horizontal positioning from SDL to window-centered
  int x(int value) { return screenWidth / 2 + value; }

  // Converts vertical positioning from SDL to window-centered
  int y(int value) { return screenHeight / 2 - value; }

  //////////////////////// HELPERS

  // If first parameter isn't truthy, throws error with provided message and SDL error log
  static void ensure(bool success, std::string errorMessage) {
    if (!success)
      throw std::runtime_error(errorMessage + "\n==> SDL error: " + SDL_GetError());
  }

private:
  // Simulated distance from eye to screen
  float eyeDistance;

  // Window dimensions
  int screenWidth;
  int screenHeight;

  // The window we'll be rendering to
  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;

  // Renderer for the window
  std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;
};

#endif