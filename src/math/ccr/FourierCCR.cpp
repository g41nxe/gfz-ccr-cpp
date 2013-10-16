/*
 * FourierCCR.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <vector>
#include <complex>

#include "CCRStrategy.h"

#include "../fft/FFTStrategy.h"
#include "FourierCCR.h"

FourierCCR::FourierCCR(FFTStrategy *s) {
	fft = s;
}

std::vector<float> FourierCCR::ccr(std::vector<float> a, std::vector<float> b) {
	// a.size = b.size and a power of 2
	using namespace std;

	vector<complex<float> > ac, bc;

	// build complex vector (no fft function for real numbers yet)
	for (unsigned int i = 0; i < a.size(); i++) {
		ac.push_back(a[i]);
		bc.push_back(b[i]);
	}

	vector<complex<float> > afft, res;

	afft = fft->fft(ac);

	for (unsigned int i; i < afft.size(); i++) {
		afft[i] = conj(afft[i]);
	}

	res = fft->ifft(mul(afft, fft->fft(bc)));
	return res;
}

void FourierCCR::changeStrategy(FFTStrategy* s) {
	fft = s;
}

static std::vector<std::complex<float> > FourierCCR::mul(
		std::vector<std::complex<float> > a,
		std::vector<std::complex<float> > b) {
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] *= b[i];
	}
	return a;
}
