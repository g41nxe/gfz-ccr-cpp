/*
 * fft_it.cpp
 *
 *  Created on: 27.08.2013
 *      Author: Dan
 */

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

#include "fft.h"

#define _USE_MATH_DEFINES

unsigned int BIT_REVERSAL(unsigned int num, int count) {
	unsigned int reverse_num = 0;
	int i;
	for (i = 0; i < count; i++) {
		if ((num & (1 << i)))
			reverse_num |= 1 << ((count - 1) - i);
	}
	return reverse_num;
}

std::vector<std::complex<float> > FFT_ITE(std::vector<std::complex<float> > a,
		int isign) {

	using namespace std;

	unsigned int n = a.size(); // n must be power of 2
	vector<complex<float> > A(n, 0.0f);
	complex<float> omega, omega_m;
	double m;

	// permutate data by bit-reversing the log2(n)+1 bits of the indices
	for (unsigned int i = 0; i < n; i++) {
		A[BIT_REVERSAL(i, ceil(log2(n)))] = a[i];
	}

	for (unsigned int s = 1; s <= ceil(log2(n)); s++) {
		m = pow(2, s);
		double theta = 2 * M_PI / m * isign;
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

std::vector<std::complex<float> > FFT_REC(std::vector<std::complex<float> > a,
		int isign) {

	using namespace std;

	unsigned int n = a.size(); // n must be power of 2

	if (n == 1)
		return a;

	vector<complex<float> > a_odd;
	vector<complex<float> > a_even;

	for (unsigned int i = 0; i < a.size(); i++)
		if (i % 2 == 0)
			a_even.push_back(a[i]);
		else
			a_odd.push_back(a[i]);

	vector<complex<float> > y_odd = FFT_REC(a_odd, isign);
	vector<complex<float> > y_even = FFT_REC(a_even, isign);
	vector<complex<float> > y(n, 0.0f);
	complex<float> omega, omega_n;

	double theta = 2 * M_PI / n * isign;
	omega_n = complex<double>(cos(theta), sin(theta));
	omega = complex<double>(1, 0);

	for (unsigned int k = 0; k <= n / 2 - 1; k++) {
		y[k] = y_even[k] + omega * y_odd[k];
		y[k + (n / 2)] = y_even[k] - omega * y_odd[k];
		omega *= omega_n;
	}
	return y;
}

std::vector<std::complex<float> > IFFT_REC(
		std::vector<std::complex<float> > a) {
	using namespace std;
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] = complex<float>(a[i].imag(), a[i].real());
	}

	std::vector<std::complex<float> > y = FFT_REC(a);

	for (unsigned int i = 0; i < y.size(); i++) {
		y[i] = complex<float>(y[i].imag() / y.size(), y[i].real() / y.size());
	}

	return y;

}

std::vector<std::complex<float> > IFFT_ITE(
		std::vector<std::complex<float> > a) {
	using namespace std;
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] = complex<float>(a[i].imag(), a[i].real());
	}

	std::vector<std::complex<float> > y = FFT_ITE(a);

	for (unsigned int i = 0; i < y.size(); i++) {
		y[i] = complex<float>(y[i].imag() / y.size(), y[i].real() / y.size());
	}

	return y;

}

std::vector<std::complex<float> > FFT_REAL(std::vector<float> f) {

	using namespace std;

	unsigned int n = f.size();
	// length must be a power of 2

	// repack real data a[] into complex array A[] of half size
	// real(A[i]) = a[i] with even i
	// imag(A[i]) = a[i] with odd i
	vector<complex<float> > A;
	for (unsigned int i = 0; i < n; i += 2) {
		A.push_back(complex<float>(f[i], f[i + 1]));
	}

	vector<complex<float> > X(n, 0.0f), Y;
	complex<float> F, G, omega, omega_n;
	complex<float> i = complex<float>(0, 1);

	float theta = 2 * M_PI / n;
	omega_n = complex<float>(cos(theta), sin(theta));
	omega = complex<float>(1, 0);

	Y = FFT_ITE(A);
	for (unsigned int r = 1; r < n / 2; r++) {
		F = complex<float>(0.5, 0) * (Y[r] + conj(Y[(n / 2) - r]));
		G = complex<float>(-0.5, 0) * i * (Y[r] - conj(Y[(n / 2) - r]));
		X[r] = F + omega * G;

		if (r == 0) {
			X[n / 2] = F - G; // special case
		} else {
			X[n - r] = conj(X[r]); // symmetry property of fft
		}
		omega *= omega_n;
	}

	return X;
}

