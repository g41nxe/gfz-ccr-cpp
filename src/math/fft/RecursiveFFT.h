/*
 * IterativeFFT.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef ITERATIVEFFT_H_
#define ITERATIVEFFT_H_

class RecursiveFFT : public FFTStrategy
{

public:
	RecursiveFFT() {};
	std::vector<std::complex<float> > fft(std::vector<std::complex<float> >);
	std::vector<std::complex<float> > ifft(std::vector<std::complex<float> >);
};

#endif /* ITERATIVEFFT_H_ */
