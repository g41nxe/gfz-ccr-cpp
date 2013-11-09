/*
 * IterativeFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "../../io/helpers.h"

#include "ThreadFFT.h"

volatile int running_threads;
pthread_mutex_t running_mutex, a_mutex;

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
	double m, theta;
	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		m = pow(2, s);
		theta = 2 * M_PI / m;
		omega_m = complex<double>(cos(theta), sin(theta));
		omega = complex<double>(1, 0); // twiddle factors

		// spawn threads to calc butterfly operations in parallel
		running_threads = (int) m/2;
		running_mutex = a_mutex = PTHREAD_MUTEX_INITIALIZER;		
		pthread_t threads[running_threads];
		struct butterfly_parameter bp[running_threads];

		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			bp[j] = {a, j, m, omega, s};
			pthread_create(&(threads[j]), NULL, butterfly, &bp[j]);
			omega *= omega_m;
		}
		
		// collect results
  		while (running_threads > 0) {
     		sleep(1);
  		}
	}

	return;
}

void *ThreadFFT::butterfly(void *arg){
	using namespace std;

	butterfly_parameter* p = (butterfly_parameter*) arg;

//	cout << "thread" << ": " << pthread_self(); print(*p->a);
//	cout << "omega: " << p->omega << " j: " << p->j << " m: " << p->m << endl; 
	
	unsigned int n = (p->a)->size();
	for (unsigned int k = p->j; k <= n - 1; k += p->m) {
		complex<double> t = p->omega * (*p->a)[k + (p->m / 2)];
		complex<double> u = (*p->a)[k];
		(*p->a)[k] = u + t;  // butterfly operations
		(*p->a)[k + (p->m / 2)] = u - t; // ...

//		cout << "i1: " << k << " i2: " << k+(p->m)/2 << endl;
	} 
	
   pthread_mutex_lock(&running_mutex);
   running_threads--;
   pthread_mutex_unlock(&running_mutex);

   return NULL;
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


