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
#include "../math/fft/OMPFFT.h"
#include "../io/helpers.h"

std::vector<std::complex<float> > mul( std::vector<std::complex<float> > a,
		std::vector<std::complex<float> > b) {

	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		a[i] *= b[i];
	}

	return a;
}

int main(int argc, char **argv) {
	using namespace std;
	
	vector<complex<float> > x, y, resx, resy, ccr, tmp;

	srand((unsigned) time(0));

	x.push_back(1.0);
	x.push_back(2.0);
	x.push_back(3.0);
	x.push_back(4.0);
	x.resize(8, 0);

	y.push_back(3.0);
	y.push_back(2.0);
	y.push_back(0.0);
	y.push_back(2.0);
	y.resize(8, 0);

	resx.push_back(complex<float>(10, 0));
	resx.push_back(complex<float>(-0.4142, -7.246));
	resx.push_back(complex<float>(-2, 2));
	resx.push_back(complex<float>(2.4142, -1.2426));
	resx.push_back(complex<float>(-2, 0));
	resx.push_back(complex<float>(2.4142, 1.2426));
	resx.push_back(complex<float>(-2, -2));
	resx.push_back(complex<float>(-0.4142, 7.2462));


	resy.push_back(7);
	resy.push_back(3);
	resy.push_back(-1);
	resy.push_back(3);

	ccr.push_back(12);
	ccr.push_back(17);
	ccr.push_back(12);
	ccr.push_back(15);
	ccr.push_back(8);
	ccr.push_back(4);
	ccr.push_back(2);

	OMPFFT o;
	IterativeFFT i;

	cout << "X: ";	print(x);
	cout << "Expected fft result:\t"; print(resx);

	o.fft(&x);
	cout << "calculated fft:\t\t"; print(x);
	o.ifft(&x);
	cout << "OMP ifft: "; print(x);
	
	

	
}
