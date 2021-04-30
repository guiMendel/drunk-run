#include <SDL.h>
#include <stdio.h>
#include <stdexcept>
#include <memory>
#include <string>
#include "../include/sdl.hpp"

void SDLWrapper::startGame() {
  // Open window
  openWindow();

  // Set game loop to true
  gameLoop = true;

  // Input event handler
  SDL_Event event;

  // Puts something on screen
  loadImg();

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
  // window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  auto windowPtr = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  window.reset(windowPtr);

  // Ensure window was created
  if (!window) {
    throw std::runtime_error((std::string)"Window could not be created! SDL_Error: " + SDL_GetError());
  }

  // Get window surface
  screenSurface = SDL_GetWindowSurface(window.get());
}

void test() {
  SDLWrapper sdl;

  sdl.startGame();
}