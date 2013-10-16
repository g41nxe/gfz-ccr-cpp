#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cmath>
#include <stdexcept>
#include <sys/time.h>

static std::vector<std::vector<double> > result;

/*
 * check if all values in data are in range between -1 and 1
 */
bool check(std::vector<double> data) {
	for (unsigned i = 0; i < data.size(); i++) {
		if (!std::isnan((data.at(i))))
			if (!(data.at(i) >= -1 && data.at(i) <= 1))
				return false;;
	}
	return true;
}

/*
 * calculate count of values which are not number
 */
int nan_count(std::vector<double> data) {
	int count = 0;
	for (unsigned i = 0; i < data.size(); i++) {
		if (std::isnan((data.at(i))))
			count++;
	}
	return count;
}

/**
 * calculate percentage of values wich are not number
 */
double nan_percentage(std::vector<double> data) {

	return 100.0 * nan_count(data) / data.size();
}

/**
 * calculate percentage of values wich are not number
 */
double nan_percentage(std::vector<std::vector<double> > data) {
	int count = 0, size = 0;
	for (unsigned i = 0; i < data.size(); i++) {
		count += nan_count(data.at(i));
		size += data.at(i).size();
	}

	return 100.0 * count / size;
}

/**
 * Berechnung des Mittelwertes
 *
 * mean = sum[i=0..count(data)](data(i)) / count(data)
 *
 */
double mean(std::vector<double> data) {
	double sum = 0;
	for (unsigned i = 0; i < data.size(); i++) {
		if (!std::isnan((data.at(i))))
			sum += data.at(i);
	}
	return sum / data.size();
}

/**
 * Berechnung der Varianz
 *
 * variance = sum[i=0..count(data)]( (data(i) - mean(data))^2 ) / (count(data)-1)
 *
 */
double variance(std::vector<double> data) {
	double m = mean(data), sum = 0;

	for (unsigned i = 0; i < data.size(); i++) {
		if (!std::isnan((data.at(i))))
			sum += pow(data.at(i) - m, 2);
	}

	return sum / (data.size() - 1);
}

/**
 * data vom Mittelwert befreihen
 */
void zero_mean(std::vector<double> &data) {
	double m = mean(data);

	for (unsigned i = 0; i < data.size(); i++) {
		data[i] = data[i] - m;
	}
}

/**
 * Pearson Correlation
 *
 * signals need to have same length and zero-mean
 * returns the person correlation for one specific lag
 *
 * p(lag) = sum[i=0..windowsize]( f(i) * h(i+lag) ) / ( sqrt(var(f)) * sqrt(var(h)) )
 *
 */
std::vector<double> correlation(std::vector<double> f, std::vector<double> h,
		int maxlag) {
	using namespace std;

	if (f.size() != h.size())
		throw invalid_argument("Time series must have same length!");

	if (maxlag < 0)
		throw invalid_argument("Maxlag must be positive!");

	std::vector<double> r(2 * maxlag + 1);
	for (int delay = -maxlag; delay < maxlag; delay++) {
		double sum = 0;
		for (unsigned i = 0; i < f.size(); i++) {

			// delay + i => negative
			if (delay + i < 0)
				continue;

			// delay + i > f.size
			if ((delay + i) >= 0 && (unsigned) (delay + i) >= f.size())
				continue;

			// NaN values
			if (isnan(f.at(i)) || isnan(h.at(i + delay))) {
				continue;

				sum += f.at(i) * h.at(i + delay);
			}
		}
		r.push_back(sum / (sqrt(variance(f)) * sqrt(variance(h))));
	}

	result.push_back(r);
	return r;
}

/**
 * maxlag: +/- maximale verschiebung der Zeitreihen zueinander
 * windowsize: größe des betrachteten Ausschnitts der Zeitreihe
 * overlap: der Abstand in dem sich die verschobenen Zeitreihen überlagern
 *
 * TODO: zero pad signals with different length
 */
std::vector<std::vector<double> > windowed_correlation(std::vector<double> f,
		std::vector<double> h, int maxlag, int window_size, int overlap) {
	using namespace std;

	if (f.size() != h.size())
		throw invalid_argument("Time series must have same length!");

	if (window_size < 0)
		throw invalid_argument("Window size must positive!");

	if (overlap > window_size)
		throw invalid_argument(
				"Overlap must be strictly less than window size!");

	if (overlap < 0)
		throw invalid_argument("Overlap must be positive!");

	if (maxlag < 0)
		throw invalid_argument("Maxlag must be positive!");

	result.clear();

	// substract mean from the time series so we can use pearson correlation
	zero_mean(f);
	zero_mean(h);

	// skip first row (x values stored in there)
	for (vector<double>::iterator start_f = f.begin() + 1, start_h = h.begin()
			+ 1; start_f + window_size < f.end();
			start_f += window_size - overlap, start_h += window_size - overlap) {
		vector<double> f_sub(start_f, start_f + window_size);
		vector<double> h_sub(start_h, start_h + window_size);

		result.push_back(correlation(f_sub, h_sub, maxlag));
	}

	return result;
}

void load_double(std::string line, std::vector<std::vector<double> > &data) {
	using namespace std;

	string element;
	stringstream ss(line);
	int i = 0;

	while (ss >> element) {
		if (element.size() > 0) {
			try {
				if (element.compare("NaN") != 0)
					data.at(i++).push_back(atof(element.c_str()));
				else
					data.at(i++).push_back(NAN);
			} catch (const out_of_range& e) {
				vector<double> v;
				if (element.compare("NaN") != 0)
					v.push_back(atof(element.c_str()));
				else
					v.push_back(NAN);

				data.push_back(v);
			}
		}
	}
}

int main() {
	using namespace std;

	ifstream f("../lianhua_realisations.txt");
	if (!f.is_open()) {
		cout << "Error while reading file!" << endl;
		return 1;
	}

	string line, element;
	vector<vector<double> > data;
	while (getline(f, line)) {
		load_double(line, data);
	}

	int count = 0, nans = 0;
	for (unsigned i = 0; i < data.size(); i++) {
		for (unsigned j = 0; j < data.at(i).size(); j++) {
			count++;
			if (isnan(data.at(i).at(j))) {
				nans++;
			}
		}
	}

	cout << "data info: " << data.size() << " time series" << " with "
			<< data[0].size() << " values and " << nan_percentage(data)
			<< "% NaNs" << endl;

	int maxlag = 100;
	int overlap = 130;
	int windowsize = 200;

	vector<vector<double> > r = windowed_correlation(data.at(1), data.at(1),
			maxlag, windowsize, overlap);

	if (!r.empty()) {
		cout << "result info: " << r.size() << " time series with "
				<< r.at(1).size() << " values and " << nan_percentage(r)
				<< "% NaNs" << endl;
	}

	for (unsigned i = 0; i < r.size(); i++) {
		cout << i << ": " << check(r.at(i)) << " ";
	}
}

