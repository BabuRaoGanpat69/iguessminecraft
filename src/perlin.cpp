#include "../include/perlin.h"
#include <cmath>
#include <raylib.h>

Perlin::Perlin(int seed) {
  shuffle(permutation, permutation + 255, std::default_random_engine(seed));
  for (int i{}; i < 256; i++) {
    unqtable[i] = permutation[i];
    // 0<x+y<512
    unqtable[i + 256] = permutation[i];
  }
};

// ease in graph
float fade(float t) { return ((6 * t - 15) * t + 10) * t * t * t; };
// defines how much effect each influence has on the pixel
float Lerp(float t, float a1, float a2) { return (a1 + t * (a2 - a1)); };

// returns index of influence vector of all corners
Vector4 Perlin::Gvert(int &x, int &y) {
  Vector4 Nvec4;
  Nvec4.x = (unqtable[unqtable[x] + y + 1]) & 7;
  Nvec4.y = (unqtable[unqtable[x + 1] + y + 1]) & 7;
  Nvec4.z = (unqtable[unqtable[x] + y]) & 7;
  Nvec4.w = (unqtable[unqtable[x + 1] + y]) & 7;
  return Nvec4;
}

float Perlin::gradindx(float x, float y) {
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;
  float ix = x - (int)floor(x);
  float jy = y - (int)floor(y);
  Vector4 grads = Gvert(X, Y);
  Vector3 topleft, topright, bottomleft, bottomright;
  Vector2 vecPtoTL, vecPtoTR, vecPtoBL, vecPtoBR;

  // calculating the vectors to the corners
  vecPtoTL.x = ix;
  vecPtoTL.y = jy - 1;

  vecPtoTR.x = ix - 1;
  vecPtoTR.y = jy - 1;

  vecPtoBL.x = ix;
  vecPtoBL.y = jy;

  vecPtoBR.x = ix - 1;
  vecPtoBR.y = jy;

  // get the value of the influece vector using index
  topleft.x = Perlin::dgrad[(int)grads.x][0];
  topleft.y = Perlin::dgrad[(int)grads.x][1];

  topright.x = Perlin::dgrad[(int)grads.y][0];
  topright.y = Perlin::dgrad[(int)grads.y][1];

  bottomleft.x = Perlin::dgrad[(int)grads.z][0];
  bottomleft.y = Perlin::dgrad[(int)grads.z][1];

  bottomright.x = Perlin::dgrad[(int)grads.w][0];
  bottomright.y = Perlin::dgrad[(int)grads.w][1];

  // finding dotproduct
  float dotTopLef = vecPtoTL.x * topleft.x + vecPtoTL.y * topleft.y;
  float dotTopRig = vecPtoTR.x * topright.x + vecPtoTR.y * topright.y;
  float dotBotLef = vecPtoBL.x * bottomleft.x + vecPtoBL.y * bottomleft.y;
  float dotBotRig = vecPtoBR.x * bottomright.x + vecPtoBR.y * bottomright.y;

  float const u = fade(ix);
  float const v = fade(jy);

  float result =
      Lerp(u, Lerp(v, dotBotLef, dotTopLef), Lerp(v, dotBotRig, dotTopRig));
  // to keep rangle from 0 to 1;
  result += 1;
  result /= 2;
  return result;
};

float Perlin::FractalBrownianMotion(int x, int y, int numOctaves) {
  float result = 0.0;
  float amplitude = 1.0;
  float frequency = 0.005; // handles density

  for (int octave = 0; octave < numOctaves; octave++) {
    float n = amplitude * gradindx(x * frequency, y * frequency);
    result += n;

    amplitude *= 0.5; // handles zoom(very sensitive)
    frequency *= 2.0; // handles fractels
  }
  return result;
}