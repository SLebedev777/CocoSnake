#include "CategoricalDistribution.h"
#include <stdexcept>
#include <algorithm>

CategoricalDistribution::CategoricalDistribution() :
	gen((std::random_device())())
{}


CategoricalDistribution::CategoricalDistribution(const std::map<int, float>& cat_probas) :
	gen((std::random_device())())
{
	if (cat_probas.empty())
		throw std::invalid_argument("empty categories probas");

	float sum_probas = 0.0f;
	std::vector<float> probas;
	for (const auto& p : cat_probas)
	{
		categories.push_back(p.first);
		probas.push_back(p.second);
		sum_probas += p.second;
	}
	
	// normalize probas to range [0.0; 1.0]
	for (auto p = probas.begin(); p != probas.end(); ++p)
		*p /= sum_probas;

	// make cumulative distr function
	float cum_p = 0.0f;
	cdf.push_back(cum_p);
	for (const auto& p : probas)
	{
		cum_p += p;
		cdf.push_back(cum_p);
	}
}

CategoricalDistribution::CategoricalDistribution(const CategoricalDistribution& other) :
	gen((std::random_device())()),
	categories(other.categories),
	cdf(other.cdf)
{}

CategoricalDistribution& CategoricalDistribution::operator=(const CategoricalDistribution& other)
{
	if (this == &other)
		return *this;

	this->gen = other.gen;
	this->categories = other.categories;
	this->cdf = other.cdf;

	return *this;
}

int CategoricalDistribution::drawOnce()
{
	if (cdf.empty())
		return 0;

	std::uniform_real_distribution<float> dis(0.0, 1.0);
	float r = dis(gen);
	auto lb = std::lower_bound(cdf.begin(), cdf.end(), r);
	int index = lb - cdf.begin() - 1;
	return categories[index];
}