/*
 * ThreadFFT.h
 *
 *  Created on: 04.11.2013
 *      Author: Dan
 */

#ifndef THREADFFT_H_
#define THREADFFT_H_

#include "FFTStrategy.h"

class ThreadFFT : public FFTStrategy
{

public:
	ThreadFFT() {};
	void fft(std::vector<std::complex<float> >*);
	void ifft(std::vector<std::complex<float> >*);
	static 
		void butterfly(std::vector<std::complex<float> > *, 
			int, int, float, std::complex<float>);

private:
	unsigned int bit_rev(unsigned int, int);
};
#endif /* THREADFFT_H_ */
