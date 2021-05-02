#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "../include/game.hpp"

int clampMirrored(int a, int b) {
  return std::clamp(a, -b / 2, b / 2);
}

void eventHandlerWrapper(void* context, SDL_Event& event) {
  static_cast<Game*>(context)->eventHandler(event);
}

void Game::eventHandler(SDL_Event& event) {
  // User requests quit
  if (event.type == SDL_QUIT) gameActive = false;

  // Player input start
  else if (event.type == SDL_KEYDOWN) {
    // Quit on q
    if (event.key.keysym.sym == SDLK_q) gameActive = false;

    // std::cout << "Hit" << std::endl;
    switch (event.key.keysym.sym) {
      // Stumble left
    case SDLK_a:
    case SDLK_LEFT:
      startMoving(-playerMoveAcceleration);
      break;

      // Stumble right
    case SDLK_d:
    case SDLK_RIGHT:
      startMoving(playerMoveAcceleration);
      break;

      // Toggle wireframes
    case SDLK_w:
      sdl.toggleWireframe();
      break;
    }
  }

  // Player input stop
  else if (event.type == SDL_KEYUP) {
    // Only halt if released key corresponds to currently active key
    switch (event.key.keysym.sym) {
      // Stop left
    case SDLK_a:
    case SDLK_LEFT:
      if (accelerationX < 0) haltMovement();
      break;

      // Stop right
    case SDLK_d:
    case SDLK_RIGHT:
      if (accelerationX > 0) haltMovement();
      break;
    }
  }
}

void Game::applyMovement(float frameTime) {
  // Lateral acceleration
  if (accelerationX) {
    // std::cout << accelerationX * frameTime << std::endl;
    speedX = std::clamp(
      speedX + accelerationX * frameTime,
      -playerMoveSpeedCap,
      playerMoveSpeedCap
    );
  }
  // If no acceleration, start halting (as long as player is still moving)
  else if (speedX) {
    // Get move direction
    short direction = speedX > 0 ? 1 : -1;
    // Reduce speed
    float speed = std::max(std::abs(speedX) - playerHaltAcceleration * frameTime, (float)0.0);
    // Restore direction
    speedX = speed * direction;
  }
  // std::cout << speedX << std::endl;

  // For whatever reason, this seems to smooth out player acceleration tremendously
  SDL_Delay(1);

  // Lateral movement
  if (speedX) movePlayer(speedX * frameTime);

  // Frontal movement
  playerProgress += (playerAdvanceSpeed * frameTime);
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
    float frameTime = sdl.elapsedTime() / 1000.0;
    // std::cout << frameTime << std::endl;

    // Handle input
    handleUserInput();

    // Update player movement
    applyMovement(frameTime);

    // Update camera to player position
    sdl.setCamera(playerX, playerProgress);

    sdl.drawShapes();
  }
}
