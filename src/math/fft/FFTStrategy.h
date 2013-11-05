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
	virtual void fft(std::vector<std::complex<float> > *) = 0;

	virtual void ifft(std::vector<std::complex<float> > *) = 0;

	// http://en.wikipedia.org/wiki/Power_of_two#Fast_algorithm_to_check_if_a_positive_number_is_a_power_of_two
	static bool isPowerOf2(unsigned int x) {
		if (x == 0) 
			return false;

		// x is power of 2 <=> x & (x-1) = 0
		return ((x & (x - 1)) == 0);
	}
};

#endif /* FFTSTRATEGY_H_*/