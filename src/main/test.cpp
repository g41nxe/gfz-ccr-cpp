#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <omp.h>
#include <chrono>
#include <map>

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

	map<int, vector<chrono::duration<double>>> all_times;

	// do work
	int pow2 = nextPowerOf2(data->at(0).size());

	cout << "#\t100\t200\t400" << endl;

	// iterate over different array sizes
	for (int i = 3; i > 0; i--) {
		vector<chrono::duration<double>> times;
		// resize
		for (unsigned int i = 0; i < data->size(); i++) {
			data->at(i).resize(pow2, 0);
		}
		for (unsigned int runs : {100, 200, 400}) {
			chrono::duration<double> t = run_fft(ccr, data, runs, 
				atoi(argv[1]) == 3 || atoi(argv[1]) == 2);

			times.push_back(t);
		}

		all_times[pow2] = times;
		pow2/=2;
	}

	for (const auto& tlist : all_times) {
		cout << tlist.first << "\t";
		for (const auto& t : tlist.second)
			cout << t.count() << "\t";
		cout << endl;
	}
	cout << endl;
	exit(0);
}

std::chrono::duration<double> run_fft (FourierCCR ccr, 
	std::vector<std::vector<float>>* data, unsigned int runs,
	bool parallel) {
	using namespace std;

	vector<vector<float>> z(runs, vector<float>());

	// start timer
	auto start = chrono::system_clock::now();
	//std::chrono::milliseconds timespan(1000);
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
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = 
	std::chrono::duration_cast<chrono::milliseconds>(end - start);

	return elapsed_seconds;
}