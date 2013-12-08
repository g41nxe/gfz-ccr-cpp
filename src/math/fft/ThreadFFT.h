/*
 * ThreadFFT.h
 *
 *  Created on: 04.11.2013
 *      Author: Dan
 */

#ifndef THREADFFT_H_
#define THREADFFT_H_

#include "FFTStrategy.h"

typedef struct butterfly_parameter butterfly_parameter;
struct butterfly_parameter {
	std::vector<std::complex<float> >* a;
	unsigned int j;
	double m;
	std::complex<float> omega;
};

class ThreadFFT : public FFTStrategy
{

public:
	ThreadFFT() {};
	void fft(std::vector<std::complex<float> >*);
	void ifft(std::vector<std::complex<float> >*);
	static 
		void butterfly(struct butterfly_parameter*);
	static
		void* butterflyWorker(void*);

private:
	unsigned int bit_rev(unsigned int, int);
};

#endif /* THREADFFT_H_ */
