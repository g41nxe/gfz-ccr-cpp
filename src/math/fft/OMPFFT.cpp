/*
 * IterativeFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>
#include <queue>
#include <assert.h>
#include <omp.h>
#include <unistd.h>
#include "../../io/helpers.h"

#include "OMPFFT.h"

#define THREADCOUNT 4

omp_lock_t todo_lock;

void OMPFFT::fft(std::vector<std::complex<float> > *a) {

	int thr_cnt;
	
	if (getenv("THREADCOUNT") != NULL) {
		thr_cnt = atoi(getenv("THREADCOUNT"));
	} else { 
		thr_cnt = THREADCOUNT;
	}

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
		
		complex<float> omegas[(int)m/2]; 

		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			omegas[j] = omega;
			omega *= omega_m;
		}

		omp_set_num_threads(thr_cnt);
		
		int end = (m/2) - 1;

		#pragma omp parallel for shared(a)
		for (int j = 0; j <= end; j++) {

			for (unsigned int k = j; k <= n -1; k += m) {
				complex<double> t = omegas[j] * (*a)[k + (m / 2)];
				complex<double> u = (*a)[k];	

				(*a)[k] = u + t;  // butterfly operations
				(*a)[k + (m / 2)] = u - t; // ...
			}
		}
	}
	
	return;
}

void OMPFFT::ifft(std::vector<std::complex<float> > *a) {
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

unsigned int OMPFFT::bit_rev(unsigned int num, int count) {
	unsigned int reverse_num = 0;
	int i;
	for (i = 0; i < count; i++) {
		if ((num & (1 << i)))
		reverse_num |= 1 << ((count - 1) - i);
	}
	return reverse_num;
}


