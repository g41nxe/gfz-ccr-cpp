class FFTStrategy {

public:
	virtual std::vector<std::complex<float> > fft(
			std::vector<std::complex<float> >) = 0;

	virtual std::vector<std::complex<float> > ifft(
			std::vector<std::complex<float> >) = 0;
};
