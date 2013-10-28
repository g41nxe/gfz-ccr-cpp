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
	std::vector<std::complex<float> > fft(std::vector<std::complex<float> >);
	std::vector<std::complex<float> > ifft(std::vector<std::complex<float> >);
};

#endif /* RECURSIVEFFT_H_ */
