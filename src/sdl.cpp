#include <SDL.h>
#include "../include/sdl.hpp"
#include <iostream>

#define GROUND_Y height - (screenHeight / 2)

void SDLWrapper::setCamera(int x, int z) {
  cameraX = x;
  cameraZ = z;

  // Destroy all objects that were left behind
  while (obstacles.size() > 0 && obstacles.front()->z < cameraZ) {
    // Checks for collision
    // collisionCheck();

    // Destroys this obstacle
    obstacles.pop_front();
  }
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

SDL_Rect SDLWrapper::makeRect(int bottomLeft, int width, int height, int distance) {
  return SDL_Rect({
    x(perspective(bottomLeft, distance, AXIS_X)),
    y(perspective(GROUND_Y, distance, AXIS_Y)),
    perspective(width + 4, distance, DISTANCE),
    perspective(height, distance, DISTANCE)
    });
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

void SDLWrapper::renderFrame() {
  // Get renderer pointer
  auto rendererPtr = renderer.get();

  // Clear screen
  SDL_SetRenderDrawColor(rendererPtr, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(rendererPtr);

  // Draw floor
  drawFloor();

  // Draw each one of the obstacles

  for (auto obstacle = obstacles.rbegin(); obstacle != obstacles.rend(); obstacle++) {
    // Dereferentiate iterator and unique_ptr
    drawObstacle(**obstacle);
  }

  // Update screen
  SDL_RenderPresent(rendererPtr);

  // std::cout << obstacles.size() << std::endl;
}


void SDLWrapper::resolveEvents(void(*eventHandler)(void*, SDL_Event&), void* context) {
  // Input event receiver
  static SDL_Event event;

  // Handle events on queue
  while (SDL_PollEvent(&event) != 0) {
    eventHandler(context, event);
  }
}

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
