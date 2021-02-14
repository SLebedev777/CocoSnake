#pragma once
#include <vector>
#include <map>
#include <random>

class CategoricalDistribution
{
public:
	CategoricalDistribution();
	CategoricalDistribution(const std::map<int, float>& cat_probas);
	CategoricalDistribution& operator=(const CategoricalDistribution& other);
	int drawOnce();

private:
	std::mt19937 gen;
	std::vector<float> cdf;
	std::vector<int> categories;

};