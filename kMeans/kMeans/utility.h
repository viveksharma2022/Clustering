#pragma once
#include <fstream>
#include <sstream>
#include<boost/tokenizer.hpp>

typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;

namespace Utility {
	
	template<typename T>
	struct Point2D {
	public:
		T x, y;
		uint16_t kClass;
		Point2D() = default;
		Point2D(T x, T y) :
			x(x),
			y(y),
			kClass(0)
		{}
		Point2D& operator=(Point2D& point) {
			this->x = point.x;
			this->y = point.y;
			this->kClass = point.kClass;
			return *this;
		}

		Point2D& operator +=(Point2D& point) {
			this->x += point.x;
			this->y += point.y;
			return *this;
		}
	};

	template<typename T>
	void ReadFromFile(const std::string& fileName, std::vector<Point2D<T>>&geodeticCoordinates);

	template<typename T>
	void WriteToFile(const std::string& fileName, std::vector<Point2D<T>>& geodeticCoordinates);
}

template<typename T>
void Utility::ReadFromFile(const std::string& fileName, std::vector<Point2D<T>>& geodeticCoordinates) {

	// Check if file exists
	std::ifstream in(fileName, std::ifstream::in);
	if (!in.is_open()) return;

	std::string line;
	std::vector< std::string > vec;
	while (getline(in, line)) {
		Tokenizer tok(line);
		try
		{
			if (std::distance(tok.begin(),tok.end()) < 5)
				continue;
			geodeticCoordinates.emplace_back(std::stof(*std::next(tok.begin(), 2)), std::stof(*std::next(tok.begin(), 4)));
		}
		catch (const std::exception& exc)
		{
			// catch anything thrown within try block that derives from std::exception
			std::cerr << exc.what();
		}
	}
}

template<typename T>
void Utility::WriteToFile(const std::string& fileName, std::vector<Point2D<T>>& geodeticCoordinates) {
	// Check if file exists
	std::ofstream outFile;
	outFile.open(fileName);
	outFile << "Age, Spending score(0-100), kClass\n";
	for (const auto& gc : geodeticCoordinates) {
		outFile << gc.x << "," << gc.y << "," << gc.kClass << "\n";
	}
	outFile.close();
}