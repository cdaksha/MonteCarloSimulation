#pragma once

class Point3D {
public:
	double x;
	double y;
	double z;

	Point3D();
	Point3D(const double xNew, const double yNew, const double zNew);
	Point3D(const Point3D &point);
	Point3D operator+=(const Point3D &point);
	Point3D operator-=(const Point3D &point);	
	void absolute();
	double norm();
	friend std::ostream& operator<<(std::ostream &out, const Point3D &point);	
	friend std::istream& operator>>(std::istream &in, Point3D &point);
};
