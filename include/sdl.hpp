#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <iostream>

#define CAMERA_HEIGHT 650
#define OBSTACLE_DEPTH 500

// Wrapper for SDL
class SDLWrapper {
public:
  // Initialize window's & renderer's unique_ptr deleter
  SDLWrapper(int width, int height, int floorWidth) :
    screenWidth(width),
    screenHeight(height),
    floorWidth(floorWidth),
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

  // Update camera position
  void setCamera(int x, float z) { cameraX = x; cameraZ = z; }

  //////////////////////// GEOMETRY

  // Draw some random shapes (MEANT FOR TESTING)
  void drawShapes();

  // Draws a rectangle relative to it's distance from screen
  void drawObstacle(int bottomLeft, int width, int height, int distance);

  // Applies perspective to a point
  int perspective(int point, int distance) { return point * eyeDistance / (eyeDistance + distance); }

  // Converts horizontal positioning from SDL to window-centered, relative to cameraX
  int x(int value) { return screenWidth / 2 + value - cameraX; }

  // Converts vertical positioning from SDL to window-centered
  int y(int value) { return screenHeight / 2 - value; }

  //////////////////////// HELPERS

  // Returns amount of miliseconds elapsed from last call to this same method
  // First call is relative to object construction
  Uint32 elapsedTime() {
    auto lastStick = timeStick;
    timeStick = SDL_GetTicks();
    return timeStick - lastStick;
  }

  // If first parameter isn't truthy, throws error with provided message and SDL error log
  static void ensure(bool success, std::string errorMessage) {
    if (!success)
      throw std::runtime_error(errorMessage + "\n==> SDL error: " + SDL_GetError());
  }

private:
  // Make an SDL Rect with y fixed on the ground and applying perspective
  SDL_Rect makeRect(int bottomLeft, int width, int height, int distance);

  // Draw a line from a point to it's corresponding depth
  void drawEdge(int pointX, int pointY, int distance);

  // Draw the borders of the ground
  void drawFloor();

  // Keeps track of the last time the elapsedTime method was called
  Uint32 timeStick{ 0 };

  // Simulated distance from eye to screen
  float eyeDistance;

  // Window dimensions
  int screenWidth;
  int screenHeight;

  // Width of the floor
  int floorWidth;

  // The window we'll be rendering to
  std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;

  // Renderer for the window
  std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer;

  // Camera z position
  float cameraZ{ 0.0 };

  // Camera x position
  int cameraX{ 0 };
};

#endif