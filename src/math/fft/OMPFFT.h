/*
 * ThreadFFT.h
 *
 *  Created on: 04.11.2013
 *      Author: Dan
 */

#ifndef OMPFFT_H_
#define OMPFFT_H_

#include <queue>
#include "FFTStrategy.h"

typedef struct bf_params bf_params;
struct bf_params {
	std::vector<std::complex<float> >* a;
	unsigned int j;
	double m;
	std::complex<float> omega;
};

class OMPFFT : public FFTStrategy
{

public:
	OMPFFT() {};
	void fft(std::vector<std::complex<float> >*);
	void ifft(std::vector<std::complex<float> >*);
	static 
		void butterfly(std::queue<struct bf_params>*);

private:
	unsigned int bit_rev(unsigned int, int);
};

#endif /* OMPFFT_H_ */
