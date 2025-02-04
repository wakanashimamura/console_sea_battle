#include "Random.h"

std::random_device Random::randomDevice;
std::mt19937_64 Random::generator(Random::randomDevice());

int Random::GenerateInt(int min, int max)
{
	if (min > max)
	{
		std::swap(min, max);
	}

	std::uniform_int_distribution <int> distribution(min, max);

	return distribution(generator);
}

size_t Random::GenerateSize_t(size_t max)
{
	if (max < 0)
	{
		return 0;
	}

	std::uniform_int_distribution <size_t> distribution(0, max);

	return distribution(generator);
}

double Random::GenerateDouble(double min, double max)
{
	if (min > max)
	{
		std::swap(min, max);
	}

	std::uniform_real_distribution <double> distribution(min, std::nextafter(max, std::numeric_limits<double>::max()));

	return distribution(generator);
}