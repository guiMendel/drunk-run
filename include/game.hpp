#ifndef __GAME__
#define __GAME__

#include <SDL2/SDL.h>
#include <random>
#include <ctime>
#include "sdl.hpp"
#include "obstacleGenerator.hpp"

// Wraps eventHandler method into a function, provided the object's pointer
void eventHandlerWrapper(void* context, SDL_Event& event);

class Game {
public:
  //////////////////////// PUBLIC CONSTANTS

  static const int SCREEN_WIDTH = 800;
  static const int SCREEN_HEIGHT = 600;

  //////////////////////// CONFIG CONSTANTS

  //////// General

  // Defines the maximum frames per second
  static const int frameRate = 30;

  // Space the player has to move around
  static const int sideWalkWidth = 2400;

  // Distance in which new obstacles are generated
  static const int depthOfView = 50000;

  // Number of units correspondent to 1 meter
  static const int unitsPerScore = 3000;

  // Units between starting position and the first obstacle
  static const int startingSlack = 10000;

  //////// Forward Movement

  // Starting speed in which player moves ahead
  static const int advanceSpeed = 2000;

  // Speed gained at each speed-up frame
  static const int speedUpAmount = 200;

  // Interval between speed-up frames, in seconds
  static constexpr float speedUpRate = 1.0;

  //////// Lateral Movement

  // Speed cap for player's lateral movement
  static constexpr float moveSpeedCap = 3000.0;
  // Acceleration for player's lateral movement
  static constexpr float moveAcceleration = 15000.0;
  // Acceleration for player's lateral halting
  static constexpr float haltAcceleration = 2000.0;

  //////// Stumbling

  // Time average between random player stumbles, in seconds
  static constexpr float averageStumbleInterval = 2.5;
  // Standard deviation for stumble intervals
  static constexpr float stumbleIntervalStandardDeviation = 1.5;

  // Average intensity of random stumbles, in speed
  static constexpr float averageStumbleIntensity = 800.0;
  // Standard deviation for stumble intensity
  static constexpr float stumbleIntensityStandardDeviation = 500.0;

  //////// Collision

  // Player width, impacts his area of collision hurtbox
  static const int playerWidth = SCREEN_WIDTH / 2;

  // Minimum height for an obstacle to be dangerous
  static const int dangerousObstacleHeight = 200;

  //////////////////////// GAME

  Game() : sdl(SCREEN_WIDTH, SCREEN_HEIGHT, sideWalkWidth), obstacleGenerator(randomGenerator, sdl) {}

  // Opens window and starts main game loop
  void startGame();

  //////////////////////// SDL INTERACTION

  // Function that takes an SDL event and resolves it
  void eventHandler(SDL_Event& event);

private:
  //////////////////////// GAME

  // Generates a new obstacle. The parameter indicates it's distance from the player
  void generateObstacle(int relativeZ) { obstacleGenerator.generate(relativeZ + playerProgress); }

  // Handles interaction with obstacleGenerator class
  void handleObstacles();

  // Generates obstacles from player's position until his relative DoV
  void generateInitialObstacles();

  // Caps frame time to maintain the frame rate cap
  void capFrameRate();

  //////////////////////// HELPERS

  // Return DoV relative to player's position
  int currentDepthOfView() { return depthOfView + playerProgress; }

  // Calculate scorerelative to player progress
  int playerScore() { return playerProgress / unitsPerScore; }

  float randomFloat(float average, float standardDeviation) {
    // Set up random interval
    std::uniform_real_distribution<double> distribution(
      average - standardDeviation,
      average + standardDeviation
    );

    return (float)distribution(randomGenerator);
  }

  //////////////////////// SDL INTERACTION

  // Handles current SDL event queue
  void handleUserInput() { sdl.resolveEvents(eventHandlerWrapper, this); }

  //////////////////////// PLAYER MOVEMENT

  // Updates the player position on the world based on it's speed
  // frameTime makes movement framerate-independent
  void applyMovement();

  // Sets lateral movement. The movement will be applied each frame
  void startMoving(float acceleration) { accelerationX = acceleration; }

  // Stops lateral acceleration
  void haltMovement() { accelerationX = 0.0; }

  // Sets time until next stumble
  void setStumbleTimer(float time = 0.0);

  // Instantly apply speed in a random direction, and set up next stumble
  void stumble();

  // Handles which frame to apply speed-up
  void speedUp();

  // Move player to the side (may overstep sidewalk boundaries)
  void movePlayer(int offset) { playerX += offset; }

  // Checks for collision with obstacles that were surpassed in the current frame
  void collisionCheck();

  // Handles logic for when a collision happens
  void collide() {
    // Registers collision
    playerCollided = true;

    // Show game over screen
    sdl.gameOverScreen(playerScore());
  }

  //////////////////////// MUTABLE STATE

  // Defines if the main game loop is ongoing
  bool gameActive{ false };

  // Turns to true after collision, to indicate the running cycle is over
  bool playerCollided{ false };

  // Indicates the amount of time elapsed from last frame to this frame, in seconds
  float frameTime;

  // Amount of units the player has advanced
  int playerProgress{ 0 };

  // Amount of units the player is offset on the x axis
  int playerX{ 0 };

  // Amount of units per second the player is moving on the x axis
  float speedX{ 0.0 };

  // Amount of speed the player is gaining on the x axis
  float accelerationX{ 0.0 };

  // Amount of units per second the player is moving on the z axis
  float speedZ{ advanceSpeed };

  // Time remaining until next stumble, in seconds
  float stumbleTimer{ averageStumbleInterval };

  // Time remaining until next speed-up, in seconds
  float speedUpTimer{ speedUpRate };

  // When player reaches this Z position, it's time to generate a new obstacle
  int nextObstacleZ{ startingSlack };

  //////////////////////// CONSTANT STATE

  // Random generator engine
  std::default_random_engine randomGenerator{ (unsigned)time(NULL) };

  // SDL Wrapper instance
  SDLWrapper sdl;

  // Obstacle generator instance
  ObstacleGenerator obstacleGenerator;
};

#endif
