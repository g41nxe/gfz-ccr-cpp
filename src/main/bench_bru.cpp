#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "../math/ccr/BruteforceCCR.h"

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

	BruteforceCCR ccr;


	//cout << R << " cross-correlations (iterative) of 2 functions with length " << N << endl;
	vector<float> x(2 * N, 0.f), y(2 * N, 0.f), z;

	srand((unsigned) time(0));

	for (unsigned int i = 0; i < N; i++) {
		x[i] = getRand(-5, 5);
		y[i] = getRand(-5, 5);
	}

	for (unsigned int rounds = 0; rounds < R; rounds++) {
		z = ccr.ccr(&x, &y);
	}
}
