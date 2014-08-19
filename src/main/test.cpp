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
		case 2: // openmp 
			ccr = FourierCCR(new OMPFFT);
			break;
		case 3: // openmp (iteratkive)
			ccr = FourierCCR(new IterativeFFT);
		default:
			break;
	}

	DataLoader dl(filename);
	dl.load();

	vector<vector<float>>* data = dl.getData();
	vector<float> z;

	cout << omp_get_num_threads() << endl;

	int pow2 = nextPowerOf2(data->at(0).size());

	for (unsigned int i = 0; i < data->size(); i++) {
		data->at(i).resize(pow2, 0);
	}

	if (atoi(argv[1]) == 3) {
			#pragma omp parallel for 
		for (unsigned int i = 0; i < data->size(); i++) {
			z = ccr.ccr(&data->at(0), &data->at(i));
		}
	}
}
