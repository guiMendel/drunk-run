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

  Game() : sdl(SCREEN_WIDTH, SCREEN_HEIGHT) {}

  // Opens window and starts main game loop
  void startGame();

  // Function that takes an SDL event and resolves it
  void eventHandler(SDL_Event& event);

private:
  // Handles current SDL event queue
  void handleUserInput() { sdl.resolveEvents(eventHandlerWrapper, this); }

  // SDL Wrapper instance
  SDLWrapper sdl;

  // Defines if the main game loop is ongoing
  bool gameActive{ false };
};

#endif