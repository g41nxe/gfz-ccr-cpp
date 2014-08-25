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

std::chrono::duration<double> run_fft (FourierCCR, 
	std::vector<std::vector<float>>*, unsigned int, bool);

std::chrono::duration<double> run_bruteforce (BruteforceCCR, 
	std::vector<std::vector<float>>*, unsigned int);

int main(int argc, char **argv) {
	using namespace std;

	if (argc < 2)
		exit(-1);

	string filename("data/dongge_realisations.txt");
	FourierCCR ccr;
	BruteforceCCR ccr2;
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

		case 4: //bruteforce
		ccr2 = BruteforceCCR();
		type = "bruteforce";
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

	map<int, map<int, double>> all_times;
	
	// # ccr; max = original dataset size
	int runs[] = {250, 500, 750, 1000};
	
	// array sizes 
	int sizes[3];

	// next power of 2
	int pow2 = nextPowerOf2(data->at(0).size());

	// iterate over different array sizes
	for (unsigned int i = 0; i < 4; i++) {
		// save array size
		sizes[i] = pow2;

		// shrink array to; halve the size every rerun
		for (unsigned int j = 0; j < data->size(); j++) {
			data->at(j).resize(pow2, 0);
		}

		// redo different run counts
		for (unsigned int r : runs) {
			chrono::duration<double> t;

			if (atoi(argv[1]) == 4) {
				t = run_bruteforce(ccr2, data, r);

			} else { 
				t = run_fft(ccr, data, r, 
					atoi(argv[1]) == 3 || atoi(argv[1]) == 2);
			}
			// save timing result	
			all_times[pow2][r] = t.count();
		}	
		pow2 /= 2;	
	}

	// output
	cout << "#" << type << "\t";
	for (int s : sizes)
		cout << s << "\t";
	cout << endl;

	for (int r : runs) {
		cout << r << "\t";
		for (int s : sizes) {
			cout << all_times[s][r] << "\t";
		}
		cout << endl;
	}
	cout << endl;


	exit(0);
}

std::chrono::duration<double> run_bruteforce (BruteforceCCR ccr, 
	std::vector<std::vector<float>>* data, unsigned int runs) {
	using namespace std;

	vector<vector<float>> z(runs, vector<float>());

	// start timer
	auto start = chrono::system_clock::now();

	for (unsigned int i = 0; i < runs; i++) {
		vector<float> x(data->at(0)), y(data->at(i));
		z[i] = ccr.ccr(&x, &y);
	}

	// stop timer
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = 
	std::chrono::duration_cast<chrono::milliseconds>(end - start);

	return elapsed_seconds;
}

std::chrono::duration<double> run_fft (FourierCCR ccr, 
	std::vector<std::vector<float>>* data, unsigned int runs,
	bool parallel) {
	using namespace std;

	vector<vector<float>> z(runs, vector<float>());

	// start timer
	auto start = chrono::system_clock::now();
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