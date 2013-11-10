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
#include <pthread.h>
#include <unistd.h>
#include "../../io/helpers.h"

#include "ThreadFFT.h"

 #define THREADCOUNT 16

volatile int running_threads;

pthread_barrier_t all_done;
pthread_mutex_t todo_mutex = PTHREAD_MUTEX_INITIALIZER;
std::queue<struct butterfly_parameter> todo;

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

		// fill parameter queue
		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			todo.push({a, j, m, omega, s});
			omega *= omega_m;
		}		

		// start threads
		pthread_mutex_init(&todo_mutex, NULL);
		pthread_barrier_init(&all_done, NULL, THREADCOUNT+1);

		int retval = 0;
		for (int i=0; i < THREADCOUNT && (retval == 0); i++) {
			retval = pthread_create(new pthread_t, NULL, &butterflyWorker, NULL);
			if(retval)
			printf("ERROR: %d\n", retval);				
		} 
	}

	pthread_barrier_wait(&all_done);
	pthread_mutex_destroy(&todo_mutex);
	pthread_barrier_destroy(&all_done);
	
	return;
}

void* ThreadFFT::butterflyWorker(void* args) {
    struct butterfly_parameter bp;
    while (!todo.empty()) {
        pthread_mutex_lock(&todo_mutex);
        if(!todo.empty())
        	bp = todo.front(); todo.pop();
        pthread_mutex_unlock(&todo_mutex);
 
        butterfly(&bp);
    }

    pthread_barrier_wait(&all_done);
    return NULL;
}

void ThreadFFT::butterfly(struct butterfly_parameter* p){
	using namespace std;
	unsigned int n = (p->a)->size();
	for (unsigned int k = p->j; k <= n - 1; k += p->m) {
		complex<double> t = p->omega * (*p->a)[k + (p->m / 2)];
		complex<double> u = (*p->a)[k];
		(*p->a)[k] = u + t;  // butterfly operations
		(*p->a)[k + (p->m / 2)] = u - t; // ...
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


