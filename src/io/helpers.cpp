#include <vector>
#include <complex>
#include <iostream>
#include <assert.h>

#include "helpers.h"

void equals(std::vector<std::complex<float> > a, 
	std::vector<std::complex<float> > b) {

	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		assert(a[i] == b[i]);
	}
}