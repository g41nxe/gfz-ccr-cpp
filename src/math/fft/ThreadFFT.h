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
	RecursiveFFT() {};
	std::vector<std::complex<float> > fft(std::vector<std::complex<float> >);
	std::vector<std::complex<float> > ifft(std::vector<std::complex<float> >);
};

#endif /* THREADFFT_H_ */
