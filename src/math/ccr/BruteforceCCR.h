/*
 * BruteforceCCR.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef BRUTEFORCECCR_H_
#define BRUTEFORCECCR_H_

#include "CCRStrategy.h"

class BruteforceCCR : public CCRStrategy {

public:
	BruteforceCCR() {};
	std::vector<float> ccr(std::vector<float>, std::vector<float>);

private:
	static std::vector<float> shift(std::vector<float> , int);
	static float dot(std::vector<float>, std::vector<float>);
};

#endif /* BRUTEFORCECCR_H_ */
