#pragma once

class Atom {
public:
	float x;
	float y;
	float z;

	Atom();
	Atom(const float xNew, const float yNew, const float zNew);
	Atom(const Atom &atom);
	Atom operator+=(const Atom &atom);
	Atom operator-=(const Atom &atom);	
	void absolute();
	float norm();
	void printCoords();
};
