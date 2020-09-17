#include <iostream>
#include <cmath>
#include "Atom.hpp"
using namespace std;

Atom::Atom() : x(0), y(0), z(0) {}

Atom::Atom(const float xNew, const float yNew, const float zNew) {
	x = xNew;
	y = yNew;
	z = zNew;
}

Atom::Atom(const Atom &atom) : Atom(atom.x, atom.y, atom.z) {}

Atom Atom::operator+=(const Atom &atom) {
	x += atom.x;
	y += atom.y;
	z += atom.z;
	return *this;	
}

Atom Atom::operator-=(const Atom &atom) {
	x -= atom.x;
	y -= atom.y;
	z -= atom.z;
	return *this;
}

void Atom::absolute() {
	x = abs(x);
	y = abs(y);
	z = abs(z);
}

float Atom::norm() {
	return pow(pow(x,2)+pow(y,2)+pow(z,2), 0.5);
}

void Atom::printCoords() {
	cout << "[" << x << ", " << y << ", " << z << "]" << endl;
}