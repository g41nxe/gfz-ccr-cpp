#ifndef FFT_H_
#define FFT_H_

std::vector<std::complex<float> > FFT_ITE(std::vector<std::complex<float> > a,
		int isign = 1);

std::vector<std::complex<float> > FFT_REC(std::vector<std::complex<float> > a,
		int isign = 1);

std::vector<std::complex<float> > IFFT_REC(
		std::vector<std::complex<float> > a);

std::vector<std::complex<float> > IFFT_ITE(
		std::vector<std::complex<float> > a);

std::vector<std::complex<float> > FFT_REAL(std::vector<float> f);

#endif /* FFT_H_ */
