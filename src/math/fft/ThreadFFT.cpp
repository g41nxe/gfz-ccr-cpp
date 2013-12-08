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

 #define THREADCOUNT 1

volatile int running_threads;

pthread_mutex_t todo_mutex = PTHREAD_MUTEX_INITIALIZER;
std::queue<struct butterfly_parameter> todo, empty;

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
		vector<pthread_t*> threads;
		std::queue<butterfly_parameter> current_todo;
		pthread_mutex_init(&todo_mutex, NULL);		

		m = pow(2, s);
		theta = 2 * M_PI / m;
		omega_m = complex<double>(cos(theta), sin(theta));
		omega = complex<double>(1, 0); // twiddle factors

		// fill parameter queue		
		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			current_todo.push ({a, j, m, omega});
			omega *= omega_m;
		}
		swap(current_todo, todo);

		// start threads
		while (threads.size() < THREADCOUNT) {
			pthread_t* t = new pthread_t;
			int retval = pthread_create(t, NULL, &butterflyWorker, NULL);
			if (retval)
				break;
			threads.push_back(t);
		} 	

		// wait for result
		for (pthread_t* t : threads) {
			pthread_join(*t, NULL);
		}

		pthread_mutex_destroy(&todo_mutex);
	}
	
	return;
}

void* ThreadFFT::butterflyWorker(void* args) {
   
   for (;;) {
   		struct butterfly_parameter* bp = NULL;
        pthread_mutex_lock(&todo_mutex);

        if (todo.empty()) {
        	pthread_mutex_unlock(&todo_mutex);
        	break;
        }
        	
        bp = &todo.front(); 
        todo.pop();

        pthread_mutex_unlock(&todo_mutex);
 
 		if (bp != NULL)
 			butterfly(bp);
    }
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


