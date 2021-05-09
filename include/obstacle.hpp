#ifndef __OBSTACLE__
#define __OBSTACLE__

// Defines an obstacle
struct Obstacle {
  // X position of this obstacle's (fontal face) bottom left corner
  int bottomLeft;
  // Width of obstacle
  int width;
  // Height of obstacle
  int height;
  // Depth of obstacle
  int depth;
  // Z position of this obstacle (frontal face)
  int z;

  Obstacle(int bottomLeft, int width, int height, int depth, int z) :
    bottomLeft(bottomLeft), width(width), height(height), depth(depth), z(z) {
  }
};

#endif