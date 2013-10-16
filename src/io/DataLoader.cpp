/*
 * DataLoader.cpp
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <stdlib.h>

#include "DataLoader.h"

DataLoader::DataLoader(std::string filename) {
	this->filename = filename;
}

DataLoader::~DataLoader() {
}

std::vector<float> DataLoader::getYears(){
	return this->years;
}

std::vector<float> DataLoader::getRow(unsigned int i){
	return this->data.at(i);
}


void DataLoader::load() {
	using namespace std;

	ifstream f(this->filename.c_str());

	if (!f.is_open())
		throw "Error opening file!";

	string line;
	vector<vector<double> > data;

	while (getline(f, line)) {
		this->readLine(line);
	}
}

void DataLoader::readLine(std::string line) {
	using namespace std;

	string element;
	stringstream ss(line);
	int i = 0;

	while (ss >> element) {
		if (element.size() > 0) {
			try {
				if (element.compare("NaN") != 0)
					this->data.at(i++).push_back(atof(element.c_str()));
				else
					this->data.at(i++).push_back(NAN);

			} catch (const out_of_range& e) {
				vector<float> v;

				if (element.compare("NaN") != 0)
					v.push_back(atof(element.c_str()));
				else
					v.push_back(NAN);

				this->data.push_back(v);
			}
		}
	}
}

