/*
 * IterativeFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>

#include "FFTStrategy.h"
#include "IterativeFFT.h"

std::vector<std::complex<float> > IterativeFFT::fft(
		std::vector<std::complex<float> > a) {

	using namespace std;

	unsigned int n = a.size(); // n must be power of 2
	vector < complex<float> > A(n, 0.0f);
	complex<float> omega, omega_m;
	double m;

	// permutate data by bit-reversing the log2(n)+1 bits of the indices
	for (unsigned int i = 0; i < n; i++) {
		A[bit_rev(i, ceil(log2(n)))] = a[i];
	}

	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		m = pow(2, s);
		double theta = 2 * M_PI / m;
		omega_m = complex<double>(cos(theta), sin(theta));
		omega = complex<double>(1, 0);

		for (unsigned int j = 0; j <= (m / 2) - 1; j++) {
			for (unsigned int k = j; k <= n - 1; k += m) {
				complex<double> t = omega * A[k + (m / 2)];
				complex<double> u = A[k];
				A[k] = u + t;
				A[k + (m / 2)] = u - t;
			}
			omega *= omega_m;
		}
	}

	return A;
}

std::vector<std::complex<float> > IterativeFFT::ifft(std::vector<std::complex<float> > a) {
	using namespace std;
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] = complex<float>(a[i].imag(), a[i].real());
	}

	std::vector<std::complex<float> > y = fft(a);

	for (unsigned int i = 0; i < y.size(); i++) {
		y[i] = complex<float>(y[i].imag() / y.size(), y[i].real() / y.size());
	}

	return y;

}

unsigned int IterativeFFT::bit_rev(unsigned int num, int count) {
	unsigned int reverse_num = 0;
	int i;
	for (i = 0; i < count; i++) {
		if ((num & (1 << i)))
		reverse_num |= 1 << ((count - 1) - i);
	}
	return reverse_num;
}


