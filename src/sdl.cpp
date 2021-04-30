#include <SDL.h>
#include <stdio.h>
#include <stdexcept>
#include <string>
#include "../include/sdl.hpp"

SDLWrapper::SDLWrapper() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error((std::string)"SDL could not initialize! SDL_Error: " + SDL_GetError());
  }

  // Create window
  window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  // Ensure window was created
  if (window == NULL) {
    throw std::runtime_error((std::string)"Window could not be created! SDL_Error: " + SDL_GetError());
  }

  // Get window surface
  screenSurface = SDL_GetWindowSurface(window);
}

void test() {
  SDLWrapper sdl;

  sdl.fillWhite();

  // Wait two seconds
  SDL_Delay(2000);
}