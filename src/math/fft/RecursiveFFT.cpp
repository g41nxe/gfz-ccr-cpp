/*
 * RekursiveFFT.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <complex>
#include <vector>

#include "FFTStrategy.h"
#include "RecursiveFFT.h"

std::vector<std::complex<float> > RecursiveFFT::fft(std::vector<std::complex<float> > a) {

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

	vector<complex<float> > y_odd = fft(a_odd);
	vector<complex<float> > y_even = fft(a_even);
	vector<complex<float> > y(n, 0.0f);
	complex<float> omega, omega_n;

	double theta = 2 * M_PI / n;
	omega_n = complex<double>(cos(theta), sin(theta));
	omega = complex<double>(1, 0);

	for (unsigned int k = 0; k <= n / 2 - 1; k++) {
		y[k] = y_even[k] + omega * y_odd[k];
		y[k + (n / 2)] = y_even[k] - omega * y_odd[k];
		omega *= omega_n;
	}
	return y;
}

std::vector<std::complex<float> > RecursiveFFT::ifft(std::vector<std::complex<float> > a) {
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
