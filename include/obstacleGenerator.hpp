#ifndef __OBSTACLE_GENERATOR__
#define __OBSTACLE_GENERATOR__

#include <random>
#include "obstacle.hpp"

// Responsible for obstacle creation timing and procedure
class ObstacleGenerator {
public:
  //////////////////////// CONFIG

  // Average distance between obstacles
  static const int averageObstacleSpacing = 3000;
  // Standard deviation for distance between obstacles
  static const int obstacleSpacingStandardDeviation = 1000;

  //////////////////////// INTERFACE

  // Initialize random engine
  ObstacleGenerator(std::default_random_engine& randomGenerator)
    : randomGenerator(randomGenerator) {
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
};

#endif