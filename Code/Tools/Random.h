#pragma once
#include <random>

class Random
{
public:
	Random() = delete;

	static int GenerateInt(int min, int max);
private:
	static std::random_device randomDevice;
	static std::mt19937_64 generator;
};