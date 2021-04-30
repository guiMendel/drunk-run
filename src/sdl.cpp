#include <SDL.h>
#include "../include/sdl.hpp"

void SDLWrapper::drawShapes() {
  // Get renderer pointer
  auto rendererPtr = renderer.get();

  // Clear screen
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rendererPtr);

  // Render red filled quad
  SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderFillRect(rendererPtr, &fillRect);

  //Render green outlined quad
  SDL_Rect outlineRect = { screenWidth / 6, screenHeight / 6, screenWidth * 2 / 3, screenHeight * 2 / 3 };
  SDL_SetRenderDrawColor(rendererPtr, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderDrawRect(rendererPtr, &outlineRect);

  //Draw blue horizontal line
  SDL_SetRenderDrawColor(rendererPtr, 0x00, 0x00, 0xFF, 0xFF);
  SDL_RenderDrawLine(rendererPtr, 0, screenHeight / 2, screenWidth, screenHeight / 2);

  //Draw vertical line of yellow dots
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0x00, 0xFF);
  for (int i = 0; i < screenHeight; i += 4) {
    SDL_RenderDrawPoint(rendererPtr, screenWidth / 2, i);
  }

  //Update screen
  SDL_RenderPresent(rendererPtr);
}


void SDLWrapper::resolveEvents(void(*eventHandler)(void*, SDL_Event&), void *context) {
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
