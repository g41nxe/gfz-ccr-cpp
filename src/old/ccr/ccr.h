/*
 * ccr.h
 *
 *  Created on: 15.09.2013
 *      Author: Dan
 */

#ifndef CCR_H_
#define CCR_H_

std::vector<float> shift(std::vector<float> a, int s);

float dot(std::vector<float> a, std::vector<float> b);

std::vector<float> ccr(std::vector<float> a, std::vector<float> b);

#endif /* CCR_H_ */
