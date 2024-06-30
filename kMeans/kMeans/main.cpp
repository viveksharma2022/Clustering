#include <iostream>
#include <exception>
#include "KMeans.h"
#include "utility.h"

int main() {
	srand((unsigned)time(NULL));
	std::string fileName = "./data/Mall_Customers.csv";
	std::vector<Utility::Point2D<float>> geodeticCoordinates;
	Utility::ReadFromFile(fileName, geodeticCoordinates);
	KMeansClutering<float> kMeanObj(5, geodeticCoordinates);
	kMeanObj.Run();
	Utility::WriteToFile(
		"./data/Mall_Customers_classified.csv",
		geodeticCoordinates);
	return 0;
}