#ifndef __OBSTACLE_GENERATOR__
#define __OBSTACLE_GENERATOR__

#include <random>
#include "obstacle.hpp"
#include "sdl.hpp"

#define TOTAL_SHAPES 9

// Prefabs for obstacle shapes
struct ObstacleShape {
  // Width of obstacle
  int width;
  // Height of obstacle
  int height;
  // Depth of the obstacle
  int depth;
};

// Responsible for obstacle creation timing and procedure
class ObstacleGenerator {
public:
  //////////////////////// CONFIG

  // Average distance between obstacles
  static const int averageObstacleSpacing = 3000;
  // Standard deviation for distance between obstacles
  static const int obstacleSpacingStandardDeviation = 1000;

  // Possible obstacle shapes
  static constexpr ObstacleShape allowedShapes[TOTAL_SHAPES] = {
    // {width, height, depth}
    {350, 1300, 400},
    {250, 1300, 400},
    {300, 1200, 400},
    {300, 1100, 400},
    {70, 70, 90},
    {40, 70, 100},
    {50, 20, 120},
    {70, 5000, 70},
    {500, 400, 120}
  };

  //////////////////////// INTERFACE

  // Initialize random engine & sdl wrapper
  ObstacleGenerator(std::default_random_engine& randomGenerator, SDLWrapper& sdl)
    : randomGenerator(randomGenerator), sdl(sdl) {
  }

  // Return the spacing to be covered before creating the next obstacle
  int getSpacing() { return randomInt(averageObstacleSpacing, obstacleSpacingStandardDeviation); }

  // Generates properties for a new obstacle and create it throught the SDL Wrapper
  // It's Z position must be provided
  void generate(int z);

private:
  //////////////////////// HELPERS

  int randomInt(int average, int standardDeviation) {
    // Set up random interval
    std::uniform_int_distribution<int> distribution(
      average - standardDeviation,
      average + standardDeviation
    );

    return (int)distribution(randomGenerator);
  }

  //////////////////////// CONSTANT STATE

  // Reference to game's random generator engine
  std::default_random_engine& randomGenerator;

  // Reference to game's SDL Wrapper instance
  SDLWrapper& sdl;
};

#endif