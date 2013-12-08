#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "../math/ccr/CCRStrategy.h"
#include "../math/ccr/BruteforceCCR.h"
#include "../math/ccr/FourierCCR.h"
#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"
#include "../math/fft/ThreadFFT.h"

#include "../io/helpers.h"

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

	assert(argc == 4);

	FourierCCR ccr;
	switch(atoi(argv[3])) {
		case 0: // iterative
			ccr = FourierCCR(new IterativeFFT);
			break;
		case 1: // threads
			ccr = FourierCCR(new ThreadFFT);
			break;
		case 2: // recursive
			ccr = FourierCCR(new RecursiveFFT);
			break;
		default:
			break;
	}


	//cout << R << " cross-correlations (iterative) of 2 functions with length " << N << endl;
	vector<float> x(2 * N, 0.f), y(2 * N, 0.f), z;

	srand((unsigned) time(0));

	for (unsigned int i = 0; i < N; i++) {
		x[i] = getRand(-5, 5);
		y[i] = getRand(-5, 5);
	}

	for (unsigned int rounds = 0; rounds < R; rounds++) {
		vector<float> current_x(x);
		vector<float> current_y(y);
		z = ccr.ccr(&current_x, &current_y);
	}
}
