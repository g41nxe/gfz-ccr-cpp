#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <limits>

#include "../math/ccr/BruteforceCCR.h"
#include "../math/ccr/FourierCCR.h"
#include "../math/fft/OMPFFT.h"
#include "../io/helpers.h"

std::vector<float> testfunc(unsigned int length, float b) {
	using namespace std;

	float a = 4.0, c = 0.75;

	vector<float> y;

	for (unsigned int i=0; i < length; i++) {
		float t = (1.0/length) * i;
		y.push_back(c * sin(a * M_PI * t + b));
	}

	return y;
}

void printarray(std::vector<float> vec) {
	using namespace std;
	int t = -vec.size()/2 + 1;
	for (unsigned int i=0; i < vec.size(); i++){
		cout << t++ << "\t" << vec[i] << endl;
	}
}


void printarray(std::vector<std::complex<float>> vec) {
	using namespace std;

	for (unsigned int i=0; i < vec.size(); i++)
		cout << vec[i].real() << "\t\t" << vec[i].imag() << endl;
}

void printarray(std::vector<float> vec, std::vector<float> vec2) {
	using namespace std;

	assert(vec.size() == vec2.size());

	for (unsigned int i=0; i < vec.size(); i++) {
		float t = (1.0/vec.size()) * i;
		cout << i << "\t" << vec[i] << "\t" << vec2[i] << endl;
	}
}

int main(int argc, char **argv) {
	using namespace std;
	
	vector<float> x, y, res;

	unsigned int size = 32;
	x = testfunc(size, 0);
	y = testfunc(size, 1 * M_PI);

	FourierCCR c(new OMPFFT);
	BruteforceCCR b;

	cout << "# INPUT t X(t) Y(t)" << endl;
	printarray(x,y);
	
	res = c.ccr(new vector<float>(x), new vector<float>(y));
	cout << endl << endl << "# CCR" << endl;
	printarray(res);

	res = b.ccr(&x,&y);
	cout <<  endl << endl << "# BRUTEFORCE" << endl;
	printarray(res);
}
