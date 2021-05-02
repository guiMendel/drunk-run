#ifndef __GAME__
#define __GAME__

#include <SDL.h>
#include "sdl.hpp"

// Wraps eventHandler method into a function, provided the object's pointer
void eventHandlerWrapper(void* context, SDL_Event& event);

class Game {
public:
  // Public constants
  static const int SCREEN_WIDTH = 800;
  static const int SCREEN_HEIGHT = 600;

  static const int sideWalkWidth = 2400;
  static const int playerMoveSpeed = 1;

  Game() : sdl(SCREEN_WIDTH, SCREEN_HEIGHT, sideWalkWidth) {}

  // Opens window and starts main game loop
  void startGame();

  // Function that takes an SDL event and resolves it
  void eventHandler(SDL_Event& event);

private:
  // Handles current SDL event queue
  void handleUserInput() { sdl.resolveEvents(eventHandlerWrapper, this); }

  // Updates the player position on the world based on it's speed
  void applyMovement(Uint32 frameTime);

  // Sets lateral movement. The movement will be applied each frame
  void startMoving(int speed) { speedX = speed; }

  // Stops lateral movement
  void haltMovement() { speedX = 0; }

  // Move player to the side
  void movePlayer(int offset);

  // SDL Wrapper instance
  SDLWrapper sdl;

  // Defines if the main game loop is ongoing
  bool gameActive{ false };

  // Amount of units the player has advanced
  float playerProgress{ 0.0 };

  // Amount of units per second the player is moving on the x axis
  int speedX{ 0 };

  // Amount of units the player is offset on the x axis
  int playerX{ 0 };
};

#endif