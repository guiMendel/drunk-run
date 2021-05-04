#include <iostream>
#include <algorithm>
#include "../include/obstacleGenerator.hpp"
#include "../include/game.hpp"

void ObstacleGenerator::generate(int z) {
  // Get one of the possible obstacle shapes
  auto shape = allowedShapes[randomGenerator() % TOTAL_SHAPES];

  // Set up random interval for X position
  std::uniform_int_distribution<int> distribution(
    // From left of sidewalk
    -(Game::sideWalkWidth / 2),
    // To right of sidewalk minus shape width
    Game::sideWalkWidth / 2 - shape.width
  );

  // Get X position
  auto bottomLeft = distribution(randomGenerator);

  // Generate obstacle
  sdl.newObstacle(bottomLeft, shape.width, shape.height, z);
}
