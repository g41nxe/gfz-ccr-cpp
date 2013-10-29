#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <limits>

#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"

void equals(std::vector<std::complex<float> > a, std::vector<std::complex<float> > b, bool onlyReal=false) {
	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		assert(a[i] == b[i]);
	}
}

int main(int argc, char **argv) {
	using namespace std;
	
	vector<complex<float> > x,y, res;
	srand((unsigned) time(0));

	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);

	y.push_back(10);
	y.push_back(complex<float>(-2.0, -2.0));
	y.push_back(complex<float>(-2.0));
	y.push_back(complex<float>(-2.0, 2.0));

	IterativeFFT i; 
	res = i.fft(x);
	equals(res,y);
	res.clear();
	res = i.ifft(i.fft(x));
	equals(res, x);
	cout << "Iterative FFT Algorithm: ok" << endl;

	RecursiveFFT r;
	res.clear();
	res = r.fft(x);
	equals(res,y);
	res.clear();
	res = r.ifft(r.fft(x));
	equals(res, x);
	cout << "Recursive FFT Algorithm: ok" << endl;
}
