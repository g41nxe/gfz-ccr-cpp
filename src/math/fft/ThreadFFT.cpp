/*
 * IterativeFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>
#include <assert.h>
#include <thread>

#include "ThreadFFT.h"

void ThreadFFT::fft(std::vector<std::complex<float> > *a) {

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
	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		float m = pow(2, s);
		float theta = 2 * M_PI / m;
		omega_m = complex<float>(cos(theta), sin(theta));
		omega = complex<float>(1, 0); // twiddle factors

		// spawn threads to calc butterfly operations in parallel
		vector<thread*> threads;
		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			thread t(butterfly, a, s, j, m, omega);
			threads.push_back(&t);
			omega *= omega_m;
		}
		
		// collect for results
		while(threads.size() > 0) {
			for (unsigned int k = 0; k < threads.size(); k++) {
				if((*threads[k]).joinable()){
					(*threads[k]).join();
					threads.erase(threads.begin()+k);
				}
			}
		}
		
	
	}

}

void ThreadFFT::butterfly(std::vector<std::complex<float> > *a, 
	int s, int j, float m, std::complex<float> omega){
	using namespace std;

	unsigned int n = a->size();
	for (unsigned int k = j; k <= n - 1; k += m) {
		complex<float> t = omega * (*a)[k + (m / 2)];
		complex<float> u = (*a)[k];
		(*a)[k] = u + t;  // butterfly operations
		(*a)[k + (m / 2)] = u - t; // ...
	} 
} 

void ThreadFFT::ifft(std::vector<std::complex<float> > *a) {
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

unsigned int ThreadFFT::bit_rev(unsigned int num, int count) {
	unsigned int reverse_num = 0;
	int i;
	for (i = 0; i < count; i++) {
		if ((num & (1 << i)))
		reverse_num |= 1 << ((count - 1) - i);
	}
	return reverse_num;
}


