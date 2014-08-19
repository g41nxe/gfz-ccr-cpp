/*
 * DataLoader.h
 *
 *  Created on: 23.09.2013
 *      Author: Dan
 */

#ifndef DATALOADER_H_
#define DATALOADER_H_

class DataLoader {
public:
	DataLoader(std::string);
	virtual ~DataLoader();
	
	void load();
	std::vector<float> getRow(unsigned int);
	std::vector<int> getYears();
	unsigned int size();
	std::vector<std::vector<float>>* getData();

private:
	void readLine(std::string);
	std::vector<std::vector<float> > data;
	std::string filename;
	std::vector<int> years;
};

#endif /* DATALOADER_H_ */
