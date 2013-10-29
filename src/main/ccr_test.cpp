#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"

#include "../math/ccr/FourierCCR.h"
#include "../math/ccr/BruteforceCCR.h"

void equals(std::vector<float> a, std::vector<float> b) {
	assert(a.size() == b.size());
	for (unsigned int i = 0; i < a.size(); i++) {
		assert(a[i] == b[i]);
	}
}

int main(int argc, char **argv) {
	using namespace std;

	float vx[4] = {1.0, 2.0, 3.0, 4.0};
	float vy[4] = {3.0, 2.0, 0.0, 2.0};
	float vr[6] = {4.0, 8.0, 15.0, 12.0, 17.0, 12.0};

	vector<float> x(&vx[0], &vx[0]+4);
	vector<float> y(&vy[0], &vy[0]+4);
	vector<float> res(&vr[0], &vr[0]+4);

	FourierCCR iccr(new IterativeFFT);
	FourierCCR rccr(new RecursiveFFT);
	BruteforceCCR bccr;

	cout << "CCR with Bruteforce: ok" << endl;
	equals(bccr.ccr(x,y), res);
	cout << "CCR with iterative FFT: ok" << endl;
	equals(iccr.ccr(x,y), res);
	cout << "CCR with recursive FFT: ok" << endl;
	equals(rccr.ccr(x,y), res);	
}
