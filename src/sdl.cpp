#include <SDL.h>
#include "../include/sdl.hpp"

#define GROUND_Y height - (screenHeight / 2) - CAMERA_HEIGHT

SDL_Rect SDLWrapper::makeRect(int bottomLeft, int width, int height, int distance) {
  return SDL_Rect({
    x(perspective(bottomLeft, distance)),
    // Assume y = bottom of screen - camera height
    y(perspective(GROUND_Y, distance)),
    perspective(width, distance),
    perspective(height, distance)
    });
}

void SDLWrapper::drawEdge(int pointX, int pointY, int distance) {
  SDL_RenderDrawLine(
    renderer.get(),
    x(perspective(pointX, distance)),
    y(perspective(pointY, distance)),
    x(perspective(pointX, distance + OBSTACLE_DEPTH)),
    y(perspective(pointY, distance + OBSTACLE_DEPTH))
  );
}

void SDLWrapper::drawObstacle(int bottomLeft, int width, int height, int distance) {
  // Relative distance to camera
  distance -= cameraZ;

  // Ignore obstacles beyond the camera
  if (distance < 0) return;

  // Get renderer
  auto rendererPtr = renderer.get();

  // Draw front face
  SDL_Rect outlineRect = makeRect(bottomLeft, width, height, distance);
  SDL_RenderDrawRect(rendererPtr, &outlineRect);

  // Draw back face
  outlineRect = makeRect(bottomLeft, width, height, distance + OBSTACLE_DEPTH);
  SDL_RenderDrawRect(rendererPtr, &outlineRect);

  // Draw edges
  drawEdge(bottomLeft, -(screenHeight / 2) - CAMERA_HEIGHT, distance);
  drawEdge(bottomLeft + width - 2, -(screenHeight / 2) - CAMERA_HEIGHT, distance);
  drawEdge(bottomLeft, -(screenHeight / 2) - CAMERA_HEIGHT + height, distance);
  drawEdge(bottomLeft + width - 2, -(screenHeight / 2) - CAMERA_HEIGHT + height, distance);
}

void SDLWrapper::drawShapes() {
  // Get renderer pointer
  auto rendererPtr = renderer.get();

  // Clear screen
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rendererPtr);

  // Paint black
  SDL_SetRenderDrawColor(rendererPtr, 0x00, 0x00, 0x00, 0xFF);

  // //Render outlined quad
  // SDL_Rect outlineRect = { x(-200), y(200), 400, 400 };
  // SDL_RenderDrawRect(rendererPtr, &outlineRect);

  // Render rect
  drawObstacle(-400, 300, 1200, 3000);

  // Render rect
  drawObstacle(-350, 500, 1000, 10000);

  // Render rect
  drawObstacle(-50, 100, 200, 4800);

  // Render rect
  drawObstacle(300, 200, 700, 6500);

  // Render rect
  // drawObstacle(300, 300, 800, 6000);

  // //Draw blue horizontal line
  // SDL_RenderDrawLine(rendererPtr, 0, screenHeight / 2, screenWidth, screenHeight / 2);

  // // Draw dot
  // SDL_RenderDrawPoint(rendererPtr, x(0), y(100));

  //Update screen
  SDL_RenderPresent(rendererPtr);
}


void SDLWrapper::resolveEvents(void(*eventHandler)(void*, SDL_Event&), void* context) {
  // Input event receiver
  static SDL_Event event;

  // Handle events on queue
  while (SDL_PollEvent(&event) != 0) {
    eventHandler(context, event);
  }
}

void SDLWrapper::openWindow() {
  // Aux local pointers
  SDL_Window* windowPtr;
  SDL_Renderer* rendererPtr;

  // Create window & renderer
  ensure(
    SDL_CreateWindowAndRenderer(
      // Window width & height
      screenWidth,
      screenHeight,
      // Window flags
      SDL_WINDOW_SHOWN,
      // Pointer refs
      &windowPtr,
      &rendererPtr
    ) == 0,
    "Failed to create window and renderer"
  );

  // Store pointers
  window.reset(windowPtr);
  renderer.reset(rendererPtr);

  //Initialize renderer color
  SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
}
