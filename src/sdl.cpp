#include <SDL.h>
#include "../include/sdl.hpp"

void SDLWrapper::drawObstacle(int bottomLeft, int width, int height, int distance) {
  if (distance < 0) return;

  SDL_Rect outlineRect = {
    x(perspective(bottomLeft, distance)),
    // Assume y = bottom of screen - camera height
    y(perspective(height - (screenHeight / 2) - CAMERA_HEIGHT, distance)),
    perspective(width, distance),
    perspective(height, distance)
  };
  SDL_RenderFillRect(renderer.get(), &outlineRect);
}

void SDLWrapper::drawShapes(float relativeDistance) {
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
  drawObstacle(-200, 400, 1200, 10000 - relativeDistance);

  // Render rect
  drawObstacle(300, 300, 800, 15000 - relativeDistance);

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
