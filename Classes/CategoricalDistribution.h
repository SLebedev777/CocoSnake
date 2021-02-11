#pragma once
#include <vector>
#include <random>

class CategoricalDistribution
{
public:
	CategoricalDistribution();
	CategoricalDistribution(const std::vector<float>& cat_probas);
	CategoricalDistribution& operator=(const CategoricalDistribution& other);
	int drawOnce();

private:
	std::mt19937 gen;
	std::vector<float> cdf;

};