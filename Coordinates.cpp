#include <iostream>
#include <vector>
#include "Coordinates.hpp"

Coordinates::Coordinates() : xmin(0), xmax(0), ymin(0), ymax(0), zmin(0), zmax(0) {}

// Print dimensions
void Coordinates::printDimensions() {
	std::cout << "[" << xmin << ", " << xmax << "], [" 
		<< ymin << ", " << ymax << "], [" 
		<< zmin << ", " << zmax << "], with " 
		<< positions.size() << " points\n";	
}

// Writing to output
std::ostream& operator<<(std::ostream &out, const Coordinates &c) {
	out << c.positions.size() << "\n\n";
	for (const Point3D &xyz : c.positions) {
		out << xyz << "\n";
	}
	return out; 
}

// Reading input
std::istream& operator>>(std::istream &in, Coordinates &c) {
	Point3D xyz;
	int temp;
	std::cout << "WARNING: Ignoring first column of input file.\n";
	while (in >> temp >> xyz) {
		c.positions.push_back(xyz);
		// std::cout << xyz << std::endl;
        c.xmin = std::min<double>(c.xmin, xyz.x);
        c.xmax = std::max<double>(c.xmax, xyz.x);
        c.ymin = std::min<double>(c.xmin, xyz.y);
        c.ymax = std::max<double>(c.xmax, xyz.y);	
        c.zmin = std::min<double>(c.zmin, xyz.z);
        c.zmax = std::max<double>(c.zmax, xyz.z);
	}
	return in;
}