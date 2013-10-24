#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>

#include "fft.h"

std::vector<std::complex<float> > mul(std::vector<std::complex<float> > a,
		std::vector<std::complex<float> > b) {
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] *= b[i];
	}
	return a;
}

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

	//cout << R << " cross-correlations (recursive) of 2 functions with length " << N << endl;

	vector<complex<float> > x(2 * N, 0.f), x2(2 * N, 0.f), y(2 * N, 0.f), z;

	srand((unsigned) time(0));

	for (unsigned int i = 0; i < N; i++) {
		x[i] = getRand(-5, 5);
		y[i] = getRand(-5, 5);
	}

	for (unsigned int rounds = 0; rounds < R; rounds++) {

		x2 = FFT_REC(x);
		for (unsigned int i; i < x2.size(); i++) {
			x2[i] = conj(x2[i]);
		}

		z = IFFT_REC(mul(x2, FFT_REC(y)));
	}
}
