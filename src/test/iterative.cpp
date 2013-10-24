#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../math/fft/IterativeFFT.h"
#include "../math/ccr/FourierCCR.h"

float getRand(float hi, float lo) {
	return hi + (float) rand() / ((float) RAND_MAX / (hi - lo));
}

template<class T> void printVector(std::vector<T> a){
	std::cout << "[ " ;
	for (unsigned int i = 0; i < a.size(); i++) {
		std::cout << a[i] << " " ;
	}
	std::cout << " ]" << std::endl;;
}

int main(int argc, char **argv) {
	using namespace std;

	unsigned int N, R;

	if (argc < 2)
		N = 256;
	else
		N = pow(2, atoi(argv[1]));

	if (argc < 3)
		R = 100;
	else
		R = atoi(argv[2]);

	cout << R << " runs: cross correlation of two vectors[" << N << "] " << endl;

	vector<float> x(N, 0.f), y(N, 0.f), z;

	srand((unsigned) time(0));

	for (unsigned int i = 0; i < N; i++) {
		x[i] = getRand(-5, 5);
		y[i] = getRand(-5, 5);
	}

	printVector(x);
	printVector(y);

	FourierCCR fccr(new IterativeFFT);
	for (unsigned int rounds = 0; rounds < R; rounds++) {
		z = fccr.ccr(x,y);
		printVector(z);
	}
}
