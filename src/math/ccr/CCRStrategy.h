/*
 * CCRStrategy.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef CCRSTRATEGY_H_
#define CCRSTRATEGY_H_

class CCRStrategy {

public:
	virtual std::vector<float> ccr(std::vector<float>*, std::vector<float>*) = 0;

};

#endif /* CCRSTRATEGY_H_ */