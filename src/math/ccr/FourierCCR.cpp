/*
 * FourierCCR.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <vector>
#include <complex>
#include <assert.h>

#include "../fft/FFTStrategy.h"
#include "FourierCCR.h"

FourierCCR::FourierCCR(FFTStrategy *s) {
	fft = s;
}

std::vector<float> FourierCCR::ccr(std::vector<float> *a, std::vector<float> *b) {
	// a.size = b.size and a power of 2
	assert(FFTStrategy::isPowerOf2(a->size()) 
		&& (a->size() == b->size()));

	using namespace std;

	// build complex vector (no fft function for real numbers yet)
	// zero pad vectors to length 2n
	complex<float> zero(complex<float>(0.0));
	vector<complex<float> > ac(a->size()*2, zero), bc(b->size()*2, zero);
	for (unsigned int i = 0; i < a->size(); i++) {
		ac[i] = (*a)[i];
		bc[i] = (*b)[i];
	}

	vector<float> res;
	vector<complex<float> > tmp;

	// 1st: fft transform both vectors
	fft->fft(&ac);
	fft->fft(&bc);

	// 2nd: build complex conjugate of vector a
	for (unsigned int i = 0; i < ac.size(); i++) {
		ac[i] = conj(ac[i]);
	}

	// 3rd: multiply a and b componentvice
	tmp = mul(ac, bc);

	// 4th: inverse fft and convert to real
	fft->ifft(&tmp);

	for (unsigned int i = 0; i < tmp.size(); i++) {
		res.push_back(tmp[i].real());
	}

	centerize(&res);

	return res;
}

void FourierCCR::centerize(std::vector<float> *vec) {
	unsigned int size = vec->size();
	for (unsigned int i = 0; i < size/2; i++) {
		std::swap((*vec)[i], (*vec)[size/2+i]);
	}
	vec->erase(vec->begin());
}

void FourierCCR::changeStrategy(FFTStrategy *s) {
	fft = s;
}

// componentvise multiplication
std::vector<std::complex<float> > FourierCCR::mul( std::vector<std::complex<float> > a,
		std::vector<std::complex<float> > b) {

	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] *= b[i];
	}

	return a;
}

