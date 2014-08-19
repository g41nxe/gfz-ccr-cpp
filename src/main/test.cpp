#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <omp.h>
#include <chrono>

#include "../math/ccr/CCRStrategy.h"
#include "../math/ccr/BruteforceCCR.h"
#include "../math/ccr/FourierCCR.h"
#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"
#include "../math/fft/ThreadFFT.h"
#include "../math/fft/OMPFFT.h"

#include "../io/helpers.h"
#include "../io/DataLoader.h"

#define THREADCOUNT 4

std::chrono::duration<double> run_fft (FourierCCR ccr, 
	std::vector<std::vector<float>>*, unsigned int, bool);

int main(int argc, char **argv) {
	using namespace std;

	if (argc < 2)
		exit(-1);

	string filename("data/dongge_realisations.txt");
	FourierCCR ccr;
	string type;
	bool printData = false;

	switch(atoi(argv[1])) {
		case 0: // iterative
		ccr = FourierCCR(new IterativeFFT);
		type = "Iterative";
		break;
		
		case 1: // recursive
		ccr = FourierCCR(new RecursiveFFT);
		type = "Recursive";
		break;
		
		case 2: // openmp (omp)
		ccr = FourierCCR(new OMPFFT);
		type = "openMP (openMP)";
		break;
		
		case 3: // openmp (iterative)
		ccr = FourierCCR(new IterativeFFT);
		type = "openMP (iterative)";
		break;

		default:
		printData = true;
		break;
	}

	// init threadcount
	int thr_cnt;
	
	if (getenv("THREADCOUNT") != NULL) {
		thr_cnt = atoi(getenv("THREADCOUNT"));
	} else { 
		thr_cnt = THREADCOUNT;
	}
	omp_set_num_threads(thr_cnt);

	// load data
	DataLoader dl(filename);
	dl.load();
	vector<vector<float>>* data = dl.getData();

	// print runs
	if (printData) {
		cout << "runs" << "\t";
		for (unsigned int r : {1, 5, 10}) {
			cout << r * 0.1 * data->size() << "\t";
		}
		cout << endl;

		exit(0);
	}

	// resize
	int pow2 = nextPowerOf2(data->at(0).size());
	for (unsigned int i = 0; i < data->size(); i++) {
		data->at(i).resize(pow2, 0);
	}

	// do work
	vector<chrono::duration<double>> times;

	for (unsigned int r : {1, 5, 10}) {
		unsigned int runs = r * 0.1 * data->size();

		chrono::duration<double> t = run_fft(ccr, data, runs, 
			atoi(argv[1]) == 3 || atoi(argv[1]) == 2);
		
		times.push_back(t);
	}

	// print results
	cout << type << "\t";

	for (auto it : times) {
		cout << it.count() << "\t";
	}

	cout << endl;

	exit(0);
}

std::chrono::duration<double> run_fft (FourierCCR ccr, 
	std::vector<std::vector<float>>* data, unsigned int runs,
	bool parallel) {
	using namespace std;

	chrono::time_point<std::chrono::high_resolution_clock> start, end;
	// start timer
	start = chrono::system_clock::now();
	vector<vector<float>> z(runs, vector<float>());

	if (parallel) {
			#pragma omp parallel
		{
				#pragma omp for
			for (unsigned int i = 0; i < runs; i++) {
				vector<float> x(data->at(0)), y(data->at(i));
				z[i] = ccr.ccr(&x, &y);
			}
		}
	} else {

		for (unsigned int i = 0; i < runs; i++) {
			vector<float> x(data->at(0)), y(data->at(i));
			z[i] = ccr.ccr(&x, &y);
		}
	}

	// stop timer
	end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = 
		std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	return elapsed_seconds;
}