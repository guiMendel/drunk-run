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
  gameLoop = true;

  // Input event handler
  SDL_Event event;

  // Puts something on screen
  loadImg("hello_world.bmp");

  // Initialize game loop
  while (gameLoop) {
    // Handle events on queue
    while (SDL_PollEvent(&event) != 0) {
      // User requests quit
      if (event.type == SDL_QUIT) {
        gameLoop = false;
      }
    }
  }
}

void SDLWrapper::openWindow() {
  // Create window
  auto windowPtr = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  window.reset(windowPtr);

  // Ensure window was created
  ensure((bool)window, "Failed to create window");

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
    welcomeImg = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0),
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

  SDL_BlitScaled(welcomeImg, NULL, screenSurface, &stretchRect);

  // Update the surface
  SDL_UpdateWindowSurface(window.get());
}

void test() {
  SDLWrapper sdl;

  sdl.startGame();
}