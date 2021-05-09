#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "../include/game.hpp"

// Clamp a given speed to boundaries set by moveSpeedCap
#define CLAMP_SPEED(speed) std::clamp((float)(speed), -moveSpeedCap, moveSpeedCap)

static int clampMirrored(int a, int b) {
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
      // Move left
    case SDLK_a:
    case SDLK_l:
    case SDLK_LEFT:
      startMoving(-moveAcceleration);
      break;

      // Move right
    case SDLK_d:
    case SDLK_m:
    case SDLK_RIGHT:
      startMoving(moveAcceleration);
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

void Game::collisionCheck() {
  auto surpassedObstacles = sdl.getSurpassedObstacles();

  // Check collision for each surpassed obstacle
  for (auto obstacle : surpassedObstacles) {
    // Get obstacle boundaries
    int obstacleLeft = obstacle.bottomLeft;
    int obstacleRight = obstacleLeft + obstacle.width;

    // Get player boundaries
    int playerLeft = playerX - playerWidth / 2;
    int playerRight = playerX + playerWidth / 2;

    // If the boundaries overlap
    if ((obstacleRight > playerLeft) && (obstacleLeft < playerRight)) {
      // Collision is detected
      collide();
    }
  }
}

void Game::applyMovement() {
  // Lateral acceleration
  if (accelerationX) {
    // std::cout << accelerationX * frameTime << std::endl;
    speedX = CLAMP_SPEED(speedX + accelerationX * frameTime);
  }
  // If no acceleration, start halting (as long as player is still moving)
  else if (speedX) {
    // Get move direction
    short direction = speedX > 0 ? 1 : -1;
    // Reduce speed
    float speed = std::max(std::abs(speedX) - haltAcceleration * frameTime, (float)0.0);
    // Restore direction
    speedX = speed * direction;
  }
  // std::cout << speedX << std::endl;

  // For whatever reason, this seems to smooth out player acceleration tremendously
  SDL_Delay(1);

  // Lateral movement
  if (speedX) movePlayer(speedX * frameTime);

  // Frontal movement
  playerProgress += (int)(speedZ * frameTime);
}

void Game::movePlayer(int offset) {
  // Apply movement
  // Restrict to sidewalk boundaries
  playerX = clampMirrored(playerX + offset, sideWalkWidth);
}

// Sets time until next stumble
void Game::setStumbleTimer(float time) {
  // If a valid time was provided, we stick to it
  if (time > 0.0) {
    stumbleTimer = time;
    return;
  }

  // Get random stumble countdown
  auto countdown = randomFloat(averageStumbleInterval, stumbleIntervalStandardDeviation);

  // Sets a lower boundary
  stumbleTimer = std::max(countdown, (float)0.2);
  // std::cout << stumbleTimer << std::endl;
}

void Game::stumble() {
  // Discount elapsed time from stumble timer
  stumbleTimer -= frameTime;

  // If timer ticks, stumble!
  if (stumbleTimer <= 0.0) {
    // Set up next stumble
    setStumbleTimer();

    // Get stumble speed
    auto stumbleSpeed = randomFloat(averageStumbleIntensity, stumbleIntensityStandardDeviation);

    // Get a stumble direction
    auto stumbleDirection = (randomGenerator() % 2 == 1) ? 1 : -1;

    // Apply stumble speed
    speedX = CLAMP_SPEED(speedX + stumbleSpeed * stumbleDirection);
  }
}

void Game::speedUp() {
  // Discount elapsed time from speed up timer
  speedUpTimer -= frameTime;

  // If timer ticks, speed up!
  if (speedUpTimer <= 0.0) {
    // Set up next speed up
    speedUpTimer = speedUpRate;

    // Apply speed up
    speedZ += speedUpAmount;
    // std::cout << speedZ << std::endl;
  }
}

void Game::handleObstacles() {
  // Verify if player has advanced enough for game to generate more obstacles
  while (nextObstacleZ <= playerProgress) {
    // Generate this obstacle
    obstacleGenerator.generate(currentDepthOfView());

    // Get next obstacle Z
    nextObstacleZ += obstacleGenerator.getSpacing();
  }
}

void Game::generateInitialObstacles() {
  // Indicates the position of the next obstacle to be generated
  int obstaclePin = nextObstacleZ;

  // Store current DoV
  const int DoV = currentDepthOfView();

  // Keeps generating until generation reaches DoV
  while (obstaclePin <= DoV) {
    // Generate this obstacle
    obstacleGenerator.generate(obstaclePin);

    // Get next obstacle Z
    obstaclePin += obstacleGenerator.getSpacing();
  }
}

void Game::startGame() {
  // Open window
  sdl.openWindow();

  // Set game loop to true
  gameActive = true;

  // Initialize obstacles
  generateInitialObstacles();

  // Initialize game loop
  while (gameActive) {
    // Handle input
    handleUserInput();

    // When player has collided, the running cycle stops
    if (playerCollided) continue;

    // Get elapsed frame time
    frameTime = sdl.elapsedTime() / 1000.0;
    // std::cout << frameTime << std::endl;

    // Update player movement
    applyMovement();

    // Handle random stumbling
    stumble();

    // Handle player speed up
    speedUp();

    // Update camera to player position
    sdl.setCamera(playerX, playerProgress);

    // Check for collision
    collisionCheck();

    // If there was collision, skip this cycle
    if (playerCollided) continue;

    // Procedural obstacle generation
    handleObstacles();

    // Render the game screen frame
    sdl.renderFrame(playerScore());
  }
}
