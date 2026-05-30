
#include "include/perlin.h"
#include "raylib.h"
#include <iostream>
int main() {
  const int screenWidth = 800;
  const int screenHeight = 450;
  InitWindow(screenWidth, screenHeight, "test");
  Perlin perlin(1234865648);
  SetTargetFPS(2);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    for (int y = 0; y < 450; y++) {
      for (int x = 0; x < 800; x++) {
        float n = perlin.FractalBrownianMotion(x, y, 13);
        int c = round(255 * n);
        Color color;
        color.r = c;
        color.g = c;
        color.b = c;
        color.a = 255;
        DrawPixel(x, y, color);
      }
    }

    EndDrawing();
  }

  return 0;
}