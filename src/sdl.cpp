#include <SDL.h>
#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include "../include/sdl.hpp"

// Wraps throwing a runtime error with SDL error concatenated to msg
void throwError(std::string message) {
  throw std::runtime_error(message + "\n==> SDL error: " + SDL_GetError());
}

// If first parameter isn't truthy, throws error with provided message
void ensure(bool success, std::string errorMessage) {
  if (!success) throwError(errorMessage);
}

void SDLWrapper::startGame() {
  // Open window
  openWindow();

  // Set game loop to true
  gameActive = true;

  // Input event handler
  SDL_Event event;

  // // Puts something on screen
  // loadImg("hello_world.bmp");

  // Initialize game loop
  while (gameActive) {
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      // User requests quit
      if (event.type == SDL_QUIT) {
        gameActive = false;
      }
    }

    // Clear screen
    SDL_SetRenderDrawColor(windowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(windowRenderer);

    // Render red filled quad
    SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    SDL_SetRenderDrawColor(windowRenderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(windowRenderer, &fillRect);

    //Render green outlined quad
    SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
    SDL_SetRenderDrawColor(windowRenderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(windowRenderer, &outlineRect);

    //Draw blue horizontal line
    SDL_SetRenderDrawColor(windowRenderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(windowRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    //Draw vertical line of yellow dots
    SDL_SetRenderDrawColor(windowRenderer, 0xFF, 0xFF, 0x00, 0xFF);
    for (int i = 0; i < SCREEN_HEIGHT; i += 4)                 {
      SDL_RenderDrawPoint(windowRenderer, SCREEN_WIDTH / 2, i);
    }

    //Update screen
    SDL_RenderPresent(windowRenderer);
  }
}

void SDLWrapper::openWindow() {
  // Create window
  auto windowPtr = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  window.reset(windowPtr);

  // Ensure window was created
  ensure((bool)window, "Failed to create window");

  // Get window rederer
  ensure(
    windowRenderer = SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
    "Failed to get window renderer"
  );

  //Initialize renderer color
  SDL_SetRenderDrawColor(windowRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Get window surface
  screenSurface = SDL_GetWindowSurface(window.get());
}

void SDLWrapper::loadImg(std::string path) {
  SDL_Surface* loadedSurface;

  // Load image
  ensure(
    loadedSurface = SDL_LoadBMP(path.c_str()),
    "Failed to load image"
  );

  // Optimize image
  ensure(
    imageBuffer = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0),
    "Invalid image"
  );

  // Get rid of temp surface
  SDL_FreeSurface(loadedSurface);

  // Stretch image
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  stretchRect.w = SCREEN_WIDTH;
  stretchRect.h = SCREEN_HEIGHT;

  SDL_BlitScaled(imageBuffer, NULL, screenSurface, &stretchRect);

  // Update the surface
  SDL_UpdateWindowSurface(window.get());
}

void test() {
  SDLWrapper sdl;

  sdl.startGame();
}