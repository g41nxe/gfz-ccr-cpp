/*
 * RekursiveFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>
#include <assert.h>

#include "RecursiveFFT.h"

void RecursiveFFT::fft(std::vector<std::complex<float> > *a) {

	using namespace std;

	unsigned int n = a->size(); 
	assert(FFTStrategy::isPowerOf2(n)); // n must be power of 2

	if (n == 1)
		return;

	vector<complex<float> > a_odd;
	vector<complex<float> > a_even;

	for (unsigned int i = 0; i < a->size(); i++)
		if (i % 2 == 0)
			a_even.push_back((*a)[i]);
		else
			a_odd.push_back((*a)[i]);

	fft(&a_odd);
	fft(&a_even);
	
	complex<float> omega, omega_n;

	double theta = 2 * M_PI / n;
	omega_n = complex<double>(cos(theta), sin(theta));
	omega = complex<double>(1, 0);

	for (unsigned int k = 0; k <= n / 2 - 1; k++) {
		(*a)[k] = a_even[k] + omega * a_odd[k];
		(*a)[k + (n / 2)] = a_even[k] - omega * a_odd[k];
		omega *= omega_n;
	}

	return;
}

void RecursiveFFT::ifft(std::vector<std::complex<float> > *a) {
	using namespace std;

	assert(isPowerOf2(a->size()));
		
	// swap imag and real part
	for (unsigned int i = 0; i < a->size(); i++) {
		(*a)[i] = complex<float>((*a)[i].imag(), (*a)[i].real());
	}
		
	this->fft(a);

	// devide all elements by vector size and 
	// swap imag and real part back
	for (unsigned int i = 0; i < a->size(); i++) {
		(*a)[i] = complex<float>((*a)[i].imag() / a->size(), (*a)[i].real() / a->size());
	}
	
	return;
}
