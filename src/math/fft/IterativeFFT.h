/*
 * IterativeFFT.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef ITERATIVEFFT_H_
#define ITERATIVEFFT_H_

class IterativeFFT : public FFTStrategy {

public:
	IterativeFFT() {};

private:
	unsigned int bit_rev(unsigned int, int);
};

#endif /* ITERATIVEFFT_H_ */
