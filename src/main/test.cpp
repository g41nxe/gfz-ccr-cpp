#include <ctime>
#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <omp.h>

#include "../math/ccr/CCRStrategy.h"
#include "../math/ccr/BruteforceCCR.h"
#include "../math/ccr/FourierCCR.h"
#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"
#include "../math/fft/ThreadFFT.h"
#include "../math/fft/OMPFFT.h"

#include "../io/helpers.h"
#include "../io/DataLoader.h"

int main(int argc, char **argv) {
	using namespace std;

	if (argc < 2)
		exit(-1);

	string filename("data/dongge_realisations.txt");
	FourierCCR ccr;

	switch(atoi(argv[1])) {
		case 0: // iterative
			ccr = FourierCCR(new IterativeFFT);
			break;
		case 1: // recursive
			ccr = FourierCCR(new RecursiveFFT);
			break;
		case 2: // openmp (iterative fft)
			ccr = FourierCCR(new OMPFFT);
			break;
		default:
			break;
	}

	DataLoader dl(filename);
	dl.load();
	
	vector<float> x(dl.getRow(0)), z;
	int pow2 = nextPowerOf2(dl.getRow(0).size()); 
	x.resize(pow2);

	for (unsigned int i = 0; i < dl.size(); i++) {
		vector<float> y(dl.getRow(i));	
		y.resize(pow2);
		z = ccr.ccr(&x, &y);
	}
}
