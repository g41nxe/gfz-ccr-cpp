#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <limits>

#include "../math/fft/IterativeFFT.h"
#include "../io/helpers.h"

int main(int argc, char **argv) {
	using namespace std;
	
	vector<float> x;
	vector<complex<float> > y, res;

	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);

	res.push_back(10);
	res.push_back(complex<float>(-2.0, -2.0));
	res.push_back(complex<float>(-2.0));
	res.push_back(complex<float>(-2.0, 2.0));

	cout << "Input: ";	print(x);
	cout << "Expected fft result: "; print(res);

	IterativeFFT i; 
	y = i.fft(&x);
	cout << "Iterative fft: "; print(y);
	//equals(res,y);
	i.ifft(&y);
	cout << "Iterative ifft: "; print(y);
	//equals(res, x);
	//cout << "Iterative FFT Algorithm: ok" << endl;

}
