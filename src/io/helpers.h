/*
 * helpers.h
 *
 *  Created on: 04.11.2013
 *      Author: Dan
 */

#ifndef HELPERS_H_
#define HELPERS_H_

 #include <iostream>

template<typename TYPE> void print(std::vector<TYPE> a) {
	std::cout << "[ ";
	for (unsigned int i = 0; i < a.size(); i++) {
		std::cout << a[i] << " ";
	}
	std::cout << "]" << std::endl;
}

void equals(std::vector<std::complex<float> >, 
	std::vector<std::complex<float> >);

#endif /* HELPERS_H_ */