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
#include "../io/helpers.h"

int main(int argc, char **argv) {
	using namespace std;
	
	vector<complex<float> > x, y, res;

	srand((unsigned) time(0));

	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);

	y = x;

	res.push_back(10);
	res.push_back(complex<float>(-2.0, -2.0));
	res.push_back(complex<float>(-2.0));
	res.push_back(complex<float>(-2.0, 2.0));

	cout << "Input: ";	print(x);
	cout << "Expected fft result: "; print(res);

	IterativeFFT i; 
	i.fft(&x);
	cout << "Iterative fft: "; print(x);
	//equals(res,y);
	i.ifft(&x);
	cout << "Iterative ifft: "; print(x);
	//equals(res, x);
	//cout << "Iterative FFT Algorithm: ok" << endl;

	RecursiveFFT r;
	r.fft(&y);
	cout << "Recursive fft: "; print(y);
	//equals(res,y);
	r.ifft(&y);
	cout << "Recursive ifft: "; print(y);
	//equals(res, x);
	//cout << "Recursive FFT Algorithm: ok" << endl;
}
