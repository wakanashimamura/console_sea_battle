#include "Random.h"

std::random_device Random::randomDevice;
std::mt19937_64 Random::generator(Random::randomDevice());

int Random::GenerateInt(int min, int max)
{
	if (min > max)
		std::swap(min, max);

	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}