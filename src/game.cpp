#include <SDL.h>
#include "../include/game.hpp"

void eventHandlerWrapper(void* context, SDL_Event& event) {
  static_cast<Game*>(context)->eventHandler(event);
}

void Game::eventHandler(SDL_Event& event) {
  // User requests quit
  if (event.type == SDL_QUIT) {
    gameActive = false;
  }
}

void Game::startGame() {
  // Open window
  sdl.openWindow();

  // Set game loop to true
  gameActive = true;

  // Initialize game loop
  while (gameActive) {
    handleUserInput();

    // Player walk
    playerProgress += 1;

    // Update camera to player position
    sdl.setCamera(playerProgress);

    sdl.drawShapes();
  }
}
