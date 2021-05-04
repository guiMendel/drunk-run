#ifndef __GAME__
#define __GAME__

#include <SDL.h>
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

  // Space the player has to move around
  static const int sideWalkWidth = 2400;

  // Distance in which new obstacles are generated
  static const int depthOfView = 10000;

  //////// Forward Movement

  // Starting speed in which player moves ahead
  static const int advanceSpeed = 1000;

  // Speed gained at each speed-up frame
  static const int speedUpAmount = 100;

  // Interval between speed-up frames, in seconds
  static constexpr float speedUpRate = 1.0;

  //////// Lateral Movement

  // Speed cap for player's lateral movement
  static constexpr float moveSpeedCap = 2400.0;
  // Acceleration for player's lateral movement
  static constexpr float moveAcceleration = 7000.0;
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

  //////////////////////// GAME

  Game() : sdl(SCREEN_WIDTH, SCREEN_HEIGHT, sideWalkWidth), obstacleGenerator(randomGenerator, sdl) {}

  // Opens window and starts main game loop
  void startGame();

  // Handles interaction with obstacleGenerator class
  void generateObstacles();

  //////////////////////// SDL INTERACTION

  // Function that takes an SDL event and resolves it
  void eventHandler(SDL_Event& event);

private:
  //////////////////////// HELPERS

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

  // Move player to the side
  void movePlayer(int offset);

  //////////////////////// MUTABLE STATE

  // Defines if the main game loop is ongoing
  bool gameActive{ false };

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
  int nextObstacleZ{ ObstacleGenerator::averageObstacleSpacing };

  //////////////////////// CONSTANT STATE

  // Random generator engine
  std::default_random_engine randomGenerator{ (unsigned)time(NULL) };

  // SDL Wrapper instance
  SDLWrapper sdl;

  // Obstacle generator instance
  ObstacleGenerator obstacleGenerator;
};

#endif