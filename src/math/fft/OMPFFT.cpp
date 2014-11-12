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
#define DEBUG false

 void OMPFFT::fft(std::vector<std::complex<float> > *a) {
 	int thr_cnt;

 	if (getenv("THREADCOUNT") != NULL) {
 		thr_cnt = atoi(getenv("THREADCOUNT"));
 	} else { 
 		thr_cnt = THREADCOUNT;
 	}
 	omp_set_num_threads(thr_cnt);

 	using namespace std;

 	unsigned int n = a->size(); 
	assert(FFTStrategy::isPowerOf2(n)); // n must be power of 2

	// permutate data by bit-reversing the log2(n)+1 bits of the indices
	//#pragma omp parallel for
	for (unsigned int i = 0; i < n; i++) {
		unsigned int rev_i = bit_rev(i, ceil(log2(n)));
		//swap
		if (i < rev_i) {
			complex<float> tmp = (*a)[i];
			(*a)[i] = (*a)[rev_i];
			(*a)[rev_i] = tmp;
		}
	}

	vector<vector<complex<float>>> omegas;

	// precalculate all the twiddle factors to prevent race coditions
	for (unsigned int s = 0; s <= ceil(log2(n)); s++) {
		float m = pow(2,s);
		unsigned int limit = (m / 2) - 1;
		float theta = 2 * M_PI / m;

		// store twiddle factors in 2 dimensional array omegas(s,j)
		complex<float> omega_m = complex<float>(cos(theta), sin(theta));
		complex<float> omega = 1;

		vector<complex<float>> omega_s(limit+1, 1);
		for (unsigned int j = 0; j <= limit; j++) {
			omega_s[j] = omega;
			omega *= omega_m;
		}
		omegas.push_back(omega_s);
	}

	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		float m = pow(2, s);
		unsigned int limit = (m / 2) - 1;
		
		// run all butterfly operations in this stage in parallel
		#pragma omp for 
			for (unsigned int j = 0; j <= limit; j++) {
				for (unsigned int k  = j; k <= n - 1; k += m) {
					if (DEBUG) {
						#pragma omp critical 
						{
							cout << "t" << omp_get_thread_num() << " s: " << s;
							cout << " omega: " << omegas[s][j] << " el1: " << k+(m/2);
							cout << " el2: " << k << endl;
						}
					}
					// use precalculated omega
					complex<float> t = omegas[s][j] * (*a)[k + (m / 2)];
					complex<float> u = (*a)[k];
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
	#pragma omp parallel for
	for (unsigned int i = 0; i < a->size(); i++) {
		(*a)[i] = complex<float>((*a)[i].imag(), (*a)[i].real());
	}

	this->fft(a);

	// devide all elements by vector size and 
	// swap back imag and real part
	#pragma omp parallel for
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


