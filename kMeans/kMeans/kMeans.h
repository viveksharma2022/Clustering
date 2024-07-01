#pragma once
#include <vector>
#include "utility.h"
#include<cstdlib>
#include <cmath>
#include <chrono>
#include <execution>

struct Options {
	uint64_t iterations;
	float centroidTolerance;
};

template<typename T>
class KMeansClutering {
private:
	std::vector<Utility::Point2D<T>>& points;
	uint8_t numberOfClusters;
	Options options;
public:
	std::vector<uint64_t> kClassCount;
	std::vector<Utility::Point2D<T>> centroids;
	std::vector<T> distance;
	KMeansClutering(uint8_t numberOfClusters, std::vector<Utility::Point2D<T>>& points) :
		numberOfClusters(numberOfClusters),
		points(points),
		options({ static_cast<uint64_t>(1e3), 1e-3 })
	{
		KMeansInitialize();
	}
	void ComputeDistanceFromCentroids(Utility::Point2D<T>& point);
	void KMeansInitialize();
	void SetOptions(uint64_t iterations, float centroidTolerance);
	void UpdateCentroids();
	void Run();
};

template<typename T>
void KMeansClutering<T>::KMeansInitialize() {

	for (auto& p : points) {
		p.kClass = static_cast<char>((rand() % numberOfClusters) + 1);
	}
	kClassCount.resize(numberOfClusters);
	centroids.resize(numberOfClusters);
	distance.resize(numberOfClusters);
	// Initialize centroids
	for_each(centroids.begin(), centroids.end(), [&](auto& c) {
		std::cout << rand() % points.size() << "\n";
		c = points[rand() % points.size()];
		});
}

template<typename T>
void KMeansClutering<T>::SetOptions(const uint64_t iterations, const float centroidTolerance) {
	options.iterations = iterations;
	options.centroidTolerance = centroidTolerance;
}

template<typename T>
void KMeansClutering<T>::Run() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (size_t iter = 0; iter < options.iterations; iter++) {
		std::fill(kClassCount.begin(), kClassCount.end(), 0); // clear class count
		std::for_each(points.begin(), points.end(), [&](auto& p) {
			ComputeDistanceFromCentroids(p);
			auto it = std::min_element(distance.begin(), distance.end());
			uint16_t minIndex = std::distance(distance.begin(), it);
			p.kClass = minIndex + 1;
			kClassCount[p.kClass - 1]++;
			});
		UpdateCentroids();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " [µs]" << std::endl;
}

template<typename T>
void KMeansClutering<T>::ComputeDistanceFromCentroids(Utility::Point2D<T>& point) {
	std::transform(centroids.begin(), centroids.end(), distance.begin(),
		[&](const auto& c) {
			return sqrt(pow(c.x - point.x, 2) + pow(c.y - point.y, 2));
		});
}

template<typename T>
void KMeansClutering<T>::UpdateCentroids() {
	// clear the centroids first
	std::for_each(centroids.begin(), centroids.end(), [](auto& c) {
		c.x = 0;
		c.y = 0;
		});
	
	// update new centroids as average of all points in the class
	std::for_each(points.begin(), points.end(), [&](auto& p) {
		centroids[p.kClass - 1] += p;
		});

	for (int i = 0; i < centroids.size(); i++) {
		centroids[i].x /= kClassCount[i];
		centroids[i].y /= kClassCount[i];
	}
}