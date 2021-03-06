/*
 * IterativeFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>
#include <assert.h>
#include "../../io/helpers.h"

#include "IterativeFFT.h"

void IterativeFFT::fft(std::vector<std::complex<float> > *a) {

	using namespace std;

	unsigned int n = a->size(); 
	assert(FFTStrategy::isPowerOf2(n)); // n must be power of 2

	// permutate data by bit-reversing the log2(n)+1 bits of the indices
	for (unsigned int i = 0; i < n; i++) {
		unsigned int rev_i = bit_rev(i, ceil(log2(n)));
		//swap
		if (i < rev_i) {
			complex<float> tmp = (*a)[i];
			(*a)[i] = (*a)[rev_i];
			(*a)[rev_i] = tmp;
		}
	}

	// iterative fft
	complex<float> omega, omega_m;	
	double m, theta;
	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		m = pow(2, s);
		theta = 2 * M_PI / m;
		omega_m = complex<double>(cos(theta), sin(theta));
		omega = complex<double>(1, 0); // twiddle factors	
		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			for (unsigned int k = j; k <= n - 1; k += m) {
				complex<double> t = omega * (*a)[k + (m / 2)];
				complex<double> u = (*a)[k];
				(*a)[k] = u + t;  // butterfly operations
				(*a)[k + (m / 2)] = u - t; // ...
			}
			omega *= omega_m;
		}
	}
}

void IterativeFFT::ifft(std::vector<std::complex<float> > *a) {
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

// function to process pure real vectors for double speed
std::vector<std::complex<float> > IterativeFFT::fft(std::vector<float> *a) {
	using namespace std;

	unsigned int n = a->size();

	// 1st: split a in 2 parts and build complex vector of size n/2
	vector<complex<float> > ac(n/2, 0.0);
	for (unsigned int i = 0; i < n/2; i++) {
		ac[i].real((*a)[2*i]);
		ac[i].imag((*a)[2*i+1]);
	}

	// 2nd: do normal fft
	fft(&ac);
	
	// 3rd: postprecessing to recover real values
	double theta = 2 * M_PI / n;
	complex<float> omega_n = complex<float>(cos(theta), sin(theta));
	complex<float> omega = complex<float>(1, 0);

	vector<complex<float> > res(n, 0.0);
	for (unsigned int m = 0; m < n/2; m++) {
		res[m] = complex<float>(ac[m].real()) + omega * complex<float>(ac[m].imag());
		res[n-m] = conj(res[m]);
		omega *= omega_n;
	}

	res[n/2] = ac[0].real() - ac[0].imag();
	return res;

}

unsigned int IterativeFFT::bit_rev(unsigned int num, int count) {
	unsigned int reverse_num = 0;
	int i;
	for (i = 0; i < count; i++) {
		if ((num & (1 << i)))
		reverse_num |= 1 << ((count - 1) - i);
	}
	return reverse_num;
}


