/*
 * BruteforceCCR.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <vector>
#include "BruteforceCCR.h"

std::vector<float> BruteforceCCR::ccr(std::vector<float> *a, std::vector<float> *b) {
	using namespace std;

	vector<float> c;

	for (int i = -b->size() + 1; i < (int) b->size(); i++) {
		c.push_back(dot((*a), shift((*b), i)));
	}

	return c;
}

std::vector<float> BruteforceCCR::shift(std::vector<float> a, int s) {
	using namespace std;

	vector<float> c(a.size());

	for (unsigned int i = 0; i < c.size(); i++) {
		if ((s + i >= 0) && (s + i < a.size()))
			c[i] = a[s + i];
	}

	return c;
}

float BruteforceCCR::dot(std::vector<float> a, std::vector<float> b) {

	float sum = 0;
	for (unsigned int i = 0; i < a.size(); i++) {
		sum += a[i] * b[i];
	}

	return sum;
}

