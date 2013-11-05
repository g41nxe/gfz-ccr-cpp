/*
 * RecursiveFFT.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef RECURSIVEFFT_H_
#define RECURSIVEFFT_H_

#include "FFTStrategy.h"

class RecursiveFFT : public FFTStrategy
{

public:
	RecursiveFFT() {};
	void fft(std::vector<std::complex<float> >*);
	void ifft(std::vector<std::complex<float> >*);
};

#endif /* RECURSIVEFFT_H_ */
