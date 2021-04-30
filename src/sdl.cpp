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
  if (!window) {
    throw std::runtime_error((std::string)"Window could not be created! SDL_Error: " + SDL_GetError());
  }

  // Get window surface
  screenSurface = SDL_GetWindowSurface(window.get());
}

void SDLWrapper::loadImg(std::string path) {
  welcomeImg = SDL_LoadBMP(path.c_str());

  // Ensure image loaded
  if (!welcomeImg) {
    throw std::runtime_error((std::string)"SDL could not load img! SDL_Error: " + SDL_GetError());
  }

  // Print image
  SDL_BlitSurface(welcomeImg, NULL, screenSurface, NULL);

  // Update the surface
  SDL_UpdateWindowSurface(window.get());
}

void test() {
  SDLWrapper sdl;

  sdl.startGame();
}