#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H


#include <stdio.h>
#include <math.h>
float perlin2d(float x, float y, float freq, int depth, int seed);
float perlin(float x, float y);


#endif /* !PERLIN_NOISE_H */