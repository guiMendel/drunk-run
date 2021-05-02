#include <SDL.h>
#include "algorithm"
#include "../include/game.hpp"

int clampMirrored(int a, int b) {
  return std::clamp(a, -b / 2, b / 2);
}

void eventHandlerWrapper(void* context, SDL_Event& event) {
  static_cast<Game*>(context)->eventHandler(event);
}

void Game::eventHandler(SDL_Event& event) {
  // User requests quit
  if (event.type == SDL_QUIT) {
    gameActive = false;
  }

  // Player input start
  else if (event.type == SDL_KEYDOWN) {
    switch (event.key.keysym.sym) {
      // Stumble left
    case SDLK_a:
    case SDLK_LEFT:
      startMoving(-playerMoveSpeed);
      break;

      // Stumble right
    case SDLK_d:
    case SDLK_RIGHT:
      startMoving(playerMoveSpeed);
      break;
    }
  }

  // Player input stop
  else if (event.type == SDL_KEYUP) {
    switch (event.key.keysym.sym) {
      // Stop left
    case SDLK_a:
    case SDLK_LEFT:
      if (speedX < 0) haltMovement();
      break;

      // Stop right
    case SDLK_d:
    case SDLK_RIGHT:
      if (speedX > 0) haltMovement();
      break;
    }
  }
}

void Game::applyMovement(Uint32 frameTime) {
  // Lateral movement
  if (speedX) movePlayer(speedX * frameTime);

  // Front movement
  playerProgress += (playerMoveSpeed * frameTime);
}

void Game::movePlayer(int offset) {
  // Apply movement
  // Restrict to sidewalk boundaries
  playerX = clampMirrored(playerX + offset, sideWalkWidth);
}

void Game::startGame() {
  // Open window
  sdl.openWindow();

  // Set game loop to true
  gameActive = true;

  // Initialize game loop
  while (gameActive) {
    // Get elapsed frame time
    Uint32 frameTime = sdl.elapsedTime();

    // Handle input
    handleUserInput();

    // Update player movement
    applyMovement(frameTime);

    // Update camera to player position
    sdl.setCamera(playerX, playerProgress);

    sdl.drawShapes();
  }
}
