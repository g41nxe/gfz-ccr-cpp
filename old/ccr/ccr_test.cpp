#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "ccr.h"

float getRand(float hi, float lo) {
	return hi + (float) rand() / ((float) RAND_MAX / (hi - lo));
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

	//cout << R << " cross-correlations (bruteforce) of 2 functions with length " << N << endl;

	vector<float> x(N), y(N), z;

	srand((unsigned) time(0));

	for (unsigned int i = 0; i < N; i++) {
		x[i] = getRand(-5, 5);
		y[i] = getRand(-5, 5);
	}

	for (unsigned int rounds = 0; rounds < R; rounds++) {
		z = ccr(x,y);
	}
}
