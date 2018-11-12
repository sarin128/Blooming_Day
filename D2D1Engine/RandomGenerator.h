#pragma once
class RandomGenerator
{
private:
	std::default_random_engine engine;
public:
	RandomGenerator();
	~RandomGenerator();

	int nextInt(int from, int to);
	float nextFloat(float from, float to);
	double nextDouble(double from, double to);
};

