#pragma once

#include <random>

class Random
{
public:

	Random() = delete;

	static int GenerateInt(int min, int max);
	static size_t GenerateSize_t(size_t max);
	static double GenerateDouble(double min, double max);

	template <typename It>
	static void Shuffle(It begin, It end);

private:

	static std::random_device randomDevice;
	static std::mt19937_64 generator;
};

template<typename It>
inline void Random::Shuffle(It begin, It end)
{
	std::shuffle(begin, end, generator);
}