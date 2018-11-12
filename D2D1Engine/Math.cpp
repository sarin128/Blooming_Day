#include"pch.h"
#include "Math.h"
#include"RandomGenerator.h"
static RandomGenerator random_;// = new RandomGenerator();

double Random(double from, double to)
{

	return random_.nextDouble(from, to);
}
float Random(float from, float to)
{

	return random_.nextFloat(from, to);
}

int Random(int from, int to)
{

	return random_.nextInt(from, to);
}


