#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <limits>

#include "../math/fft/ThreadFFT.h"
#include "../math/fft/IterativeFFT.h"
#include "../io/helpers.h"

int main(int argc, char **argv) {
	using namespace std;
	
	vector<complex<float> > x, y, res;

	srand((unsigned) time(0));

	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);
	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);

	y = x;

	vector<complex<float> > *ptr_x = &x;

	res.push_back(10);
	res.push_back(complex<float>(-2.0, -2.0));
	res.push_back(complex<float>(-2.0));
	res.push_back(complex<float>(-2.0, 2.0));

	cout << "Input: ";	print(x);
	cout << "Expected fft result: "; print(res);

	ThreadFFT i; 
	i.fft(ptr_x);
	cout << "Thread fft: "; print(x);
	//equals(res,y);
	i.ifft(ptr_x);
	cout << "Thread ifft: "; print(x);
	//equals(res, x);
	//cout << "Iterative FFT Algorithm: ok" << endl;

	IterativeFFT r;
	r.fft(&y);
	cout << "Iterative fft: "; print(y);
	//equals(res,y);
	r.ifft(&y);
	cout << "Iterative ifft: "; print(y);
	//equals(res, x);
	//cout << "Recursive FFT Algorithm: ok" << endl;
}
