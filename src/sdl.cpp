#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/sdl.hpp"
#include <iostream>

#define GROUND_Y height - (screenHeight / 2)

/******************************************************************************
 * Public methods
******************************************************************************/

void SDLWrapper::openWindow() {
  // Aux local pointers
  SDL_Window* windowPtr;
  SDL_Renderer* rendererPtr;

  // Create window & renderer
  ensure(
    SDL_CreateWindowAndRenderer(
      // Window width & height
      screenWidth,
      screenHeight,
      // Window flags
      SDL_WINDOW_SHOWN,
      // Pointer refs
      &windowPtr,
      &rendererPtr
    ) == 0,
    "Failed to create window and renderer"
  );

  // Store pointers
  window.reset(windowPtr);
  renderer.reset(rendererPtr);

  //Initialize renderer color
  SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void SDLWrapper::resolveEvents(void(*eventHandler)(void*, SDL_Event&), void* context) {
  // Input event receiver
  static SDL_Event event;

  // Handle events on queue
  while (SDL_PollEvent(&event) != 0) {
    eventHandler(context, event);
  }
}

void SDLWrapper::setCamera(int x, int z) {
  cameraX = x;
  cameraZ = z;

  // Empties surpassedObstacles list from last frame
  surpassedObstacles.clear();

  // Destroy all objects that were left behind
  while (obstacles.size() > 0 && obstacles.front()->z < cameraZ) {
    // Adds copy of this obstacle to surpassed obtacles list
    surpassedObstacles.push_back(*(obstacles.front()));
    
    // Destroys this obstacle
    obstacles.pop_front();
  }
}

void SDLWrapper::renderFrame(int score) {
  // Get renderer pointer
  auto rendererPtr = renderer.get();

  // Clear screen
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rendererPtr);

  // Draw floor
  drawFloor();

  // Draw each one of the obstacles
  for (auto obstacle = obstacles.rbegin();
    obstacle != obstacles.rend(); obstacle++) {
    // Dereferentiate iterator and unique_ptr
    drawObstacle(**obstacle);
  }

  // Render score
  displayScore(score);

  // Update screen
  SDL_RenderPresent(rendererPtr);

  // std::cout << obstacles.size() << std::endl;
}

void SDLWrapper::displayScore(int meter) {
  // Get score
  std::string scoreMessage = std::to_string(meter) + " m";

  // Get text asset
  Text score(scoreMessage.c_str(), screenWidth / 2 - 50, screenHeight / 40, 100, 70);

  // Render on screen with default font
  Font(renderer.get()).render(score);

  // Update screen
  SDL_RenderPresent(renderer.get());
}

/******************************************************************************
 * Private methods
******************************************************************************/

void SDLWrapper::gameOverScreen(int finalScore) {
  // Get renderer
  auto rendererPtr = renderer.get();

  // Clear screen
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rendererPtr);

  // Font color
  SDL_Color yellow = SDL_Color{ 247, 216, 39, 255 };

  // Make fonts
  Font gameOverFont("Fonts/game_over.ttf", 128, yellow, rendererPtr);
  Font defaultFont(rendererPtr);

  // Create "Game Over" message
  Text gameOverText("Game Over", screenWidth / 2 - 250, screenHeight / 2 - 150, 500, 200);

  // Create message to exit the game
  Text exitText("Press 'q' to exit", screenWidth / 2 - 175, screenHeight / 2 + 60, 350, 80);

  // Create final score message
  std::string finalScoreMessage = (std::string)"Final score: " + std::to_string(finalScore) + "m";
  Text scoreText(finalScoreMessage, screenWidth / 2 - 150, screenHeight / 40 + 40, 300, 60);

  // Display messages
  gameOverFont.render(gameOverText);
  defaultFont.render(exitText);
  defaultFont.render(scoreText);

  // Update screen
  SDL_RenderPresent(rendererPtr);
}

SDL_Rect SDLWrapper::makeRect(int bottomLeft, int width,
  int height, int distance) {
  return SDL_Rect({
    x(perspective(bottomLeft, distance, AXIS_X)),
    y(perspective(GROUND_Y, distance, AXIS_Y)),
    perspective(width + 4, distance, DISTANCE),
    perspective(height, distance, DISTANCE)
    });
}

void SDLWrapper::drawObstacle(Obstacle& obstacle) {
  // Relative distance to camera
  int distance = obstacle.z - cameraZ;

  // Ignore obstacles beyond the camera (there shouldnt be any though)
  if (distance < 0) return;

  // Get renderer
  auto rendererPtr = renderer.get();

  // Draw sides
  drawSides(obstacle.bottomLeft, obstacle.width, obstacle.height, obstacle.depth, distance);

  // Paint color
  SDL_SetRenderDrawColor(rendererPtr, OBSTACLE_COLOR);

  // Draw front face
  SDL_Rect outlineRect = makeRect(obstacle.bottomLeft, obstacle.width, obstacle.height, distance);

  // Filled
  if (!wireframesOnly) SDL_RenderFillRect(rendererPtr, &outlineRect);
  // Wireframe
  else {
    SDL_RenderDrawRect(rendererPtr, &outlineRect);

    // Draw back face
    outlineRect = makeRect(obstacle.bottomLeft, obstacle.width, obstacle.height, distance + obstacle.depth);
    SDL_RenderDrawRect(rendererPtr, &outlineRect);
  }
}

void SDLWrapper::drawSides(int bottomLeft, int width, int height, int depth, int z) {
  const int startingY = -(screenHeight / 2) + 5;
  const int finalY = -(screenHeight / 2) + height;
  const int startingX = bottomLeft;
  const int finalX = bottomLeft + width;

  // Paint color
  SDL_SetRenderDrawColor(renderer.get(), OBSTACLE_COLOR);

  // Draw edges
  drawEdge(startingX, startingY, z, depth);
  drawEdge(startingX, finalY, z, depth);
  drawEdge(finalX, finalY, z, depth);
  drawEdge(finalX, startingY, z, depth);

  // Draw filled sides
  if (!wireframesOnly) {
    // Paint color
    SDL_SetRenderDrawColor(renderer.get(), OBSTACLE_SIDE_COLOR);

    // Draw left and right sides
    for (int y = startingY + 1; y < finalY; y++) {
      // Left
      drawEdge(startingX, y, z, depth);
      // Right
      drawEdge(finalX, y, z, depth);
    }

    // Draw top side
    for (int x = startingX + 1; x < finalX; x++) drawEdge(x, finalY, z, depth);
  }
}

void SDLWrapper::drawEdge(int pointX, int pointY, int z, int depth) {
  SDL_RenderDrawLine(
    renderer.get(),
    x(perspective(pointX, z, AXIS_X)),
    y(perspective(pointY, z, AXIS_Y)),
    x(perspective(pointX, z + depth, AXIS_X)),
    y(perspective(pointY, z + depth, AXIS_Y))
  );
}

void SDLWrapper::drawFloor() {
  // Get renderer
  auto rendererPtr = renderer.get();

  SDL_SetRenderDrawColor(rendererPtr, OBSTACLE_COLOR);

  // Draw left border
  SDL_RenderDrawLine(
    rendererPtr,
    x(-floorWidth / 2 - cameraX),
    y(-screenHeight / 2 - CAMERA_HEIGHT),
    x(0),
    y(0)
  );

  // Draw right border
  SDL_RenderDrawLine(
    rendererPtr,
    x(floorWidth / 2 - cameraX),
    y(-screenHeight / 2 - CAMERA_HEIGHT),
    x(0),
    y(0)
  );
}
