#pragma once

#include <vector>
#include "Point3D.hpp"

class Coordinates {
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	double zmin;
	double zmax;	
public:
	std::vector<Point3D> positions;

	Coordinates();
	void printDimensions();
	friend std::ostream& operator<<(std::ostream &out, const Coordinates &c);
	friend std::istream& operator>>(std::istream &in, Coordinates &c);	
};