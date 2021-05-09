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

  // Destroy all objects that were left behind
  while (obstacles.size() > 0 && obstacles.front()->z < cameraZ) {
    // Checks for collision, TODO if collision : need to stop the game
    collisionCheck();

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

  if (collision) {
    gameOver();
  }
  else {
    //Display the current score
    meterCount(score);
    m_score = score;

    // Draw floor
    drawFloor();

    // Draw each one of the obstacles
    for (auto obstacle = obstacles.rbegin();
      obstacle != obstacles.rend(); obstacle++) {
      // Dereferentiate iterator and unique_ptr
      drawObstacle(**obstacle);
    }
  }
  // Update screen
  SDL_RenderPresent(rendererPtr);

  // std::cout << obstacles.size() << std::endl;
}

void SDLWrapper::meterCount(int meter) {
  auto rendererPtr = renderer.get();

  std::string s = std::to_string(meter) + " m";
  char const* text = s.c_str();
  Text score(text, screenWidth / 2 - 50, screenHeight / 40, 70, 100);

  Font font;
  font.RenderText(rendererPtr, score);
}

/******************************************************************************
 * Private methods
******************************************************************************/

void SDLWrapper::collisionCheck() {
  int x1 = obstacles.front().get()->bottomLeft;
  int x2 = x1 + obstacles.front().get()->width;
  int h = obstacles.front().get()->height;

  //The width of the player is screenWidth/2
  int playerX1 = cameraX - screenWidth / 4;
  int playerX2 = cameraX + screenWidth / 4;

  if ((h > 70) && !(x2 < playerX1) && !(x1 > playerX2)) {
    collision = true;
  }
}

void SDLWrapper::gameOver() {
  // Get renderer
  auto rendererPtr = renderer.get();

  // Create "Game Over" message
  Text text1("Game Over", screenWidth / 2 - 250, screenHeight / 2 - 150, 200, 500);
  // Color
  SDL_Color yellow = SDL_Color{ 247, 216, 39, 255 };
  // Font
  Font font1("Fonts/game_over.ttf", 128, yellow);
  // Display it
  font1.RenderText(rendererPtr, text1);

  // Create message to exit the game
  Text text2("Press 'q' to exit", screenWidth / 2 - 150, screenHeight / 2 + 60, 80, 300);
  // Color
  SDL_Color black = SDL_Color{ 0x00, 0x00, 0x00, 0xFF };
  // Font
  Font font2("Fonts/Roboto-Bold.ttf", 128, black);
  // Display it
  font2.RenderText(rendererPtr, text2);

  // Create final score message
  Text score("Final score: " + std::to_string(m_score) + " m", screenWidth / 2 - 100, screenHeight / 40 + 40, 60, 200);
  // Font
  Font font3("Fonts/Roboto-Bold.ttf", 128, black);
  // Display it
  font2.RenderText(rendererPtr, score);
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
