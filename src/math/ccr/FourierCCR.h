/*
 * FourierCCR.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef FOURIERCCR_H_
#define FOURIERCCR_H_

#include "CCRStrategy.h"

class FFTStrategy;
class FourierCCR : public CCRStrategy {

public:
	FourierCCR(FFTStrategy *s);
	void changeStrategy(FFTStrategy* s);
	std::vector<float> ccr(std::vector<float>, std::vector<float>);

	static std::vector<std::complex<float> > mul(
		std::vector<std::complex<float> >,
		std::vector<std::complex<float> >);

private:
	FFTStrategy *fft;
};

#endif /* FOURIERCCR_H_ */
