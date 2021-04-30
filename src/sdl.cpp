#include <SDL.h>
#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include "../include/sdl.hpp"

static void drawShapes(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
  // Clear screen
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);

  // Render red filled quad
  SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  SDL_RenderFillRect(renderer, &fillRect);

  //Render green outlined quad
  SDL_Rect outlineRect = { screenWidth / 6, screenHeight / 6, screenWidth * 2 / 3, screenHeight * 2 / 3 };
  SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
  SDL_RenderDrawRect(renderer, &outlineRect);

  //Draw blue horizontal line
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
  SDL_RenderDrawLine(renderer, 0, screenHeight / 2, screenWidth, screenHeight / 2);

  //Draw vertical line of yellow dots
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
  for (int i = 0; i < screenHeight; i += 4) {
    SDL_RenderDrawPoint(renderer, screenWidth / 2, i);
  }

  //Update screen
  SDL_RenderPresent(renderer);
}

void SDLWrapper::startGame() {
  // Open window
  openWindow();

  // Set game loop to true
  gameActive = true;

  // Input event handler
  SDL_Event event;

  // Initialize game loop
  while (gameActive) {
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      // User requests quit
      if (event.type == SDL_QUIT) {
        gameActive = false;
      }
    }

    drawShapes(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
  }
}

void SDLWrapper::openWindow() {
  // Aux local pointers
  SDL_Window* windowPtr;
  SDL_Renderer* rendererPtr;

  // Create window & renderer
  ensure(
    SDL_CreateWindowAndRenderer(
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_SHOWN,
      &windowPtr,
      &rendererPtr
    ) == 0,
    "Failed to create window"
  );

  // auto windowPtr = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  window.reset(windowPtr);
  // renderer.reset(rendererPtr);
  renderer = rendererPtr;

  //Initialize renderer color
  SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void test() {
  SDLWrapper sdl;

  sdl.startGame();
}