#include <iostream>
#include <cmath>
#include "Point3D.hpp"

Point3D::Point3D() : x(0), y(0), z(0) {}

Point3D::Point3D(const double xNew, const double yNew, const double zNew) {
	x = xNew;
	y = yNew;
	z = zNew;
}

Point3D::Point3D(const Point3D &point) : Point3D(point.x, point.y, point.z) {}

Point3D Point3D::operator+=(const Point3D &point) {
	x += point.x;
	y += point.y;
	z += point.z;
	return *this;	
}

Point3D Point3D::operator-=(const Point3D &point) {
	x -= point.x;
	y -= point.y;
	z -= point.z;
	return *this;
}

void Point3D::absolute() {
	x = std::abs(x);
	y = std::abs(y);
	z = std::abs(z);
}

double Point3D::norm() {
	return std::pow(std::pow(x,2)+std::pow(y,2)+std::pow(z,2), 0.5);
}

std::ostream& operator<<(std::ostream &out, const Point3D &point) {
	out << point.x << ", " << point.y << ", " << point.z; 
	return out;
}

std::istream& operator>>(std::istream &in, Point3D &point) {
	in >> point.x >> point.y >> point.z;
	return in;
}
