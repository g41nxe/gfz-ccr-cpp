#include <vector>
#include <complex>
#include <iostream>
#include <assert.h>
#include <cmath>
#include "helpers.h"

void equals(std::vector<std::complex<float> > a, 
	std::vector<std::complex<float> > b) {

	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		assert(a[i] == b[i]);
	}
}

float getRand(float hi, float lo) {
	return hi + (float) rand() / ((float) RAND_MAX / (hi - lo));
}

int nextPowerOf2(int n) {
    return (int)std::pow(2, std::floor(std::log2(n)) + 1);
}