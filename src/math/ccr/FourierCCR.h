/*
 * FourierCCR.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef FOURIERCCR_H_
#define FOURIERCCR_H_

#include "CCRStrategy.h"
#include "../fft/FFTStrategy.h"

class FourierCCR : public CCRStrategy {

public:
	FourierCCR(FFTStrategy *s);
	void changeStrategy(FFTStrategy* s);
	std::vector<float> ccr(std::vector<float> a, std::vector<float> b);


private:
	FFTStrategy *fft;

	std::vector<std::complex<float> > mul(
		std::vector<std::complex<float> >,
		std::vector<std::complex<float> >);

};

#endif /* FOURIERCCR_H_ */
