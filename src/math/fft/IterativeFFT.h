/*
 * IterativeFFT.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef ITERATIVEFFT_H_
#define ITERATIVEFFT_H_

#include "FFTStrategy.h"

class IterativeFFT : public FFTStrategy {

public:
	IterativeFFT() {};
	std::vector<std::complex<float> > fft(std::vector<std::complex<float> >);
	std::vector<std::complex<float> > ifft(std::vector<std::complex<float> >);

private:
	unsigned int bit_rev(unsigned int, int);
};

#endif /* ITERATIVEFFT_H_ */
