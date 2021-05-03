#ifndef __GAME__
#define __GAME__

#include <SDL.h>
#include <random>
#include <ctime>
#include "sdl.hpp"

// Wraps eventHandler method into a function, provided the object's pointer
void eventHandlerWrapper(void* context, SDL_Event& event);

class Game {
public:
  /////////////// PUBLIC CONSTANTS
  static const int SCREEN_WIDTH = 800;
  static const int SCREEN_HEIGHT = 600;

  /////////////// CONFIG CONSTANTS

  // Space the player has to move around
  static const int sideWalkWidth = 2400;

  // Speed in which player moves ahead
  static const int playerAdvanceSpeed = 1000;

  //////// Lateral Movement

  // Speed cap for player's lateral movement
  static constexpr float playerMoveSpeedCap = 2400.0;
  // Acceleration for player's lateral movement
  static constexpr float playerMoveAcceleration = 7000.0;
  // Acceleration for player's lateral halting
  static constexpr float playerHaltAcceleration = 2000.0;

  //////// Stumbles

  // Time average between random player stumbles, in seconds
  static constexpr float averageStumbleInterval = 1.5;
  // Standard deviation for stumble intervals
  static constexpr float stumbleIntervalStandardDeviation = 1.0;

  // Average intensity of random stumbles, in speed
  static constexpr float averageStumbleIntensity = 1300.0;
  // Standard deviation for stumble intensity
  static constexpr float stumbleIntensityStandardDeviation = 800.0;


  Game() : sdl(SCREEN_WIDTH, SCREEN_HEIGHT, sideWalkWidth) {}

  // Opens window and starts main game loop
  void startGame();

  // Function that takes an SDL event and resolves it
  void eventHandler(SDL_Event& event);

private:
  // Handles current SDL event queue
  void handleUserInput() { sdl.resolveEvents(eventHandlerWrapper, this); }

  // Updates the player position on the world based on it's speed
  // frameTime makes movement framerate-independent
  void applyMovement(float frameTime);

  // Sets lateral movement. The movement will be applied each frame
  void startMoving(float acceleration) { accelerationX = acceleration; }

  // Stops lateral acceleration
  void haltMovement() { accelerationX = 0.0; }

  // Sets time until next stumble
  void setStumbleTimer(float time = 0.0);

  // Instantly apply speed in a random direction, and set up next stumble
  void stumble(float frameTime);

  // Move player to the side
  void movePlayer(int offset);

  // SDL Wrapper instance
  SDLWrapper sdl;

  // Random generator engine
  std::default_random_engine randomGenerator{ (unsigned)time(NULL) };

  // Defines if the main game loop is ongoing
  bool gameActive{ false };

  // Amount of units the player has advanced
  float playerProgress{ 0.0 };

  // Amount of units the player is offset on the x axis
  int playerX{ 0 };

  // Amount of units per second the player is moving on the x axis
  float speedX{ 0.0 };

  // Amount of speed the player is gaining on the x axis
  float accelerationX{ 0.0 };

  // Time remaining until next stumble, in seconds
  float stumbleTimer;
};

#endif