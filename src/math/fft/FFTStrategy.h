/*
 * FFTStrategy.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef FFTSTRATEGY_H_
#define FFTSTRATEGY_H_

class FFTStrategy {

public:
	virtual std::vector<std::complex<float> > fft(
			std::vector<std::complex<float> >) = 0;

	virtual std::vector<std::complex<float> > ifft(
			std::vector<std::complex<float> >) = 0;
};

#endif /* FFTSTRATEGY_H_ */