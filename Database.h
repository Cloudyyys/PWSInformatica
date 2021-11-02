#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class dataCont {
public:
	std::string productCode;
	std::string name;
	std::string category;
	float price;
	int inStorage;
	int sales;
	std::string remarks;

	dataCont(std::string inputProductCode) {
		productCode = inputProductCode;
	}
};

class catTemplate {
public:
	std::vector <dataCont> category;
	std::string nameCategory;
	std::string nameDataFileCat;

	catTemplate(std::string inputCategoryName) {
		nameCategory = inputCategoryName;
		nameDataFileCat = (inputCategoryName + "DataFile.txt");
	}
};

