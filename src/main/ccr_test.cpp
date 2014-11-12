#include <complex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "../math/fft/IterativeFFT.h"
#include "../math/fft/RecursiveFFT.h"
#include "../math/fft/OMPFFT.h"

#include "../math/ccr/FourierCCR.h"
#include "../math/ccr/BruteforceCCR.h"

#include "../io/helpers.h"

int main(int argc, char **argv) {
	using namespace std;

	float vx[4] = {1.0, 2.0, 3.0, 4.0};
	float vy[4] = {3.0, 2.0, 0.0, 2.0};
	float vr[7] = {12.0, 17.0, 12.0, 15.0, 8.0, 4.0, 2.0};

	vector<float> x(&vx[0], &vx[0]+4);
	vector<float> y(&vy[0], &vy[0]+4);
	vector<float> res(&vr[0], &vr[0]+7);

	FourierCCR iccr(new IterativeFFT);
	FourierCCR rccr(new RecursiveFFT);
	FourierCCR occr(new OMPFFT);
	BruteforceCCR bccr;
	
	cout << "Input x: "; print(x); 
	cout << "Input y: "; print(y);
	cout << "Expected result: "; print(res);
	cout << "BruteforceCCR result: "; print(bccr.ccr(&x,&y));
	cout << "IterativeCCR result: "; print(iccr.ccr(&x,&y));
	cout << "RecursiveCCR result: "; print(rccr.ccr(&x,&y));
	cout << "OMPCCR result:"; print(occr.ccr(&x, &y));

}
