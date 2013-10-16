/*
 * FourierCCR.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef FOURIERCCR_H_
#define FOURIERCCR_H_

class FourierCCR : public CCRStrategy {

public:
	FourierCCR(FFTStrategy *s);
	void changeStrategy(FFTStrategy* s);

private:
	FFTStrategy *fft;

	static std::vector<std::complex<float> > mul(
			std::vector<std::complex<float> >,
			std::vector<std::complex<float> >);

};
#endif /* FOURIERCCR_H_ */
