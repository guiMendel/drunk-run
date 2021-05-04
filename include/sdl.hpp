#ifndef __SDLWRAPPER__
#define __SDLWRAPPER__

#include <SDL.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <iostream>
#include <deque>
#include <utility>
#include "obstacle.hpp"

// Config params

#define CAMERA_HEIGHT 650

#define OBSTACLE_COLOR 0x00, 0x00, 0x00, 0xFF
#define OBSTACLE_SIDE_COLOR 0x33, 0x33, 0x33, 0xFF

// Macros

#define DISTANCE 0
#define AXIS_X 1
#define AXIS_Y 2

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
  void setCamera(int x, int z);

  // Renders a new frame
  void renderFrame();

  //////////////////////// OBSTACLE INTERFACE

  // Create a new obstacle and add it to the queue
  // Passes all arguments directly to obstacle's constructor
  template <typename... Args>
  void newObstacle(Args&&... args) { obstacles.push_back(std::make_unique<Obstacle>(std::forward<Args>(args)...)); }

  // Toggles between drawing wireframes or filled cubes
  void toggleWireframe() { wireframesOnly = !wireframesOnly; }

  //////////////////////// HELPERS

  // Returns amount of miliseconds elapsed from last frame to now
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
  //////////////////////// LOGIC

  // Verifies if camera is colliding in this frame with the front obstacle in the deque 
  void collisionCheck(); // TODO
  
  //////////////////////// GEOMETRY

  // Applies perspective to a point
  // offsetType is used to make the final position relative to the camera position
  int perspective(int point, int distance, short offsetType) {
    // If X axis, offset to camera X position. If Y axis, offset to camera height.
    int offset = 0;
    if (offsetType) offset = (offsetType == AXIS_X ? cameraX : CAMERA_HEIGHT);
    return (point - offset) * eyeDistance / (eyeDistance + distance);
  }

  // Converts horizontal positioning from SDL to window-centered
  int x(int value) { return screenWidth / 2 + value; }

  // Converts vertical positioning from SDL to window-centered
  int y(int value) { return screenHeight / 2 - value; }

  // Make an SDL Rect with y fixed on the ground and applying perspective
  SDL_Rect makeRect(int bottomLeft, int width, int height, int distance);

  // Draws a rectangle relative to it's distance from screen
  void drawObstacle(Obstacle& obstacle);

  // Draw the sides of an obstacle
  void drawSides(int bottomLeft, int width, int height, int depth, int z);

  // Draw a line from a point to it's corresponding depth
  void drawEdge(int pointX, int pointY, int z, int depth);

  // Draw the borders of the ground
  void drawFloor();

  //////////////////////// MUTABLE STATE

  // Deque of all active obstacles, ordered in Z position
  std::deque<std::unique_ptr<Obstacle>> obstacles;

  // If turned on, draws only wireframes
  bool wireframesOnly{ false };

  // Keeps track of the time elapsed between frames
  Uint32 timeStick{ 0 };

  // Camera z position
  int cameraZ{ 0 };

  // Camera x position
  int cameraX{ 0 };

  //////////////////////// CONSTANT STATE

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
};

#endif