#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <locale>
#include <limits>
#include <random>
#include <time.h>

#include "Point3D.hpp"
#include "Coordinates.hpp"
using namespace std;

// Struct to parse commas
struct csv_reader: std::ctype<char> {
    csv_reader(): std::ctype<char>(get_table()) {}
    static std::ctype_base::mask const* get_table() {
        static std::vector<std::ctype_base::mask> rc(table_size, std::ctype_base::mask());

        rc[','] = std::ctype_base::space;
        rc['\n'] = std::ctype_base::space;
        rc[' '] = std::ctype_base::space;
        return &rc[0];
    }
}; 

// FUNCTION DECLARATIONS
inline double min_image_dist(const Point3D &x, const Point3D &y);
inline void wrap_coords(Point3D &point);
inline double RandomDouble(const double &min, const double &max);
inline Point3D perturb(const Point3D &point, const double &cutoff);
inline double hard_sphere(const double &distance);
double energy_pairwise(const Coordinates &c, const int &i);
bool metropolis(Coordinates &c, const double &cutoff);
void simulate(Coordinates &c, const double &cutoff, const long &iters, 
	const int &dump_freq);
void tests();

// CONSTANTS 
const double k_B = 1.3807e-23; // Boltzmann constant [Joules/Kelvin]
const double T = 300.0; // temperature
const double D = 1.0; // particle diameter
const double L = 10.939*D; // simulation box length	
const int N = 1000; // number of particles

int main() {
	// SETTING SEED
	srand(0);
	// TIMER
	time_t start = time(0); 
	// OUTPUT SPECIFICATION
	cout.precision(3);

	// PARAMETERS
	const double cutoff = 0.1; // random perturbation cutoff
	const long iters = 1000000; // number of MC steps
	const int dump_freq = 50000; // "dump.xyz" output frequency

	// RUNNING TESTS
	// tests();


	// ----- READING INPUT DATA FILE -----
	string infile;
	cout << "Enter the input topology file name: ";
//	cin >> infile;
	infile = "initial_lattice.xyz";
	cout << "Reading " << infile << "..." << endl;
	ifstream in(infile);	
	// Check for error in file name
	if (in.fail()) {
		cerr << "ERROR: FILE NOT FOUND" << endl;
		exit(1);
	}
	// The call to 'imbue' uses a locale that treats commas as whitespace. 
	in.imbue(locale(locale(), new csv_reader()));	
	// Skipping lines
	for (int i = 0; i < 2; i++) {
		in.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	Coordinates c = Coordinates();
	in >> c;	
	// ----- READING INPUT DATA FILE -----


	// ----- OUTPUT DUMP TEST -----
	// string outfile;
	// cout << "Enter the output dump name: ";
	// cin >> outfile;
	// cout << "Outputting initial coords to " << outfile << "..." << endl; 
	// // "ios_base::app" appends to file
	// ofstream out(outfile, ios_base::app);
	// out << c;	
	// ----- OUTPUT DUMP TEST -----


	// ----- RUNNING MONTE CARLO SIMULATION -----
	simulate(c, cutoff, iters, dump_freq);
	// ----- RUNNING MONTE CARLO SIMULATION -----


	double time_elapsed = difftime(time(0), start); 
	cout << "Time elapsed: " << time_elapsed << " seconds." << endl;
	return 0;	
}



// Minimum image correction for distance between two points
inline double min_image_dist(const Point3D &x, const Point3D &y) {
	Point3D tmp = Point3D(x);
	tmp -= y;
	tmp.absolute();
	if (tmp.x > L/2) {
		tmp.x = L - tmp.x;
	}
	if (tmp.y > L/2) {
		tmp.y = L - tmp.y;
	}
	if (tmp.z > L/2) {
		tmp.z = L - tmp.z;
	}
	return tmp.norm();
}

// Wrap coordinates to simulate periodic boundary conditions
inline void wrap_coords(Point3D &point) {
	if (point.x > L) {
		point.x -= L;
	} else if (point.x < 0.0) {
		point.x += L;
	}
	if (point.y > L) {
		point.y -= L;
	} else if (point.y < 0.0) {
		point.y += L;
	}
	if (point.z > L) {
		point.z -= L;
	} else if (point.z < 0.0) {
		point.z += L;
	}
}

// Generate random double in range [min, max]
inline double RandomDouble(const double &min, const double &max) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = max - min;
    double r = random * diff;
    return min + r;
}

// Move atom by random perturbation
inline Point3D perturb(const Point3D &point, const double &cutoff) {
	double moves[3];
	for (int i = 0; i < 3; i++) {
		moves[i] = RandomDouble(-cutoff, cutoff);
	}	
	Point3D xyz_new = Point3D(point.x + moves[0], point.y + moves[1],
		point.z + moves[2]);
	return xyz_new;
} 

// Hard sphere potential
inline double hard_sphere(const double &distance) {
	double out = distance >= D ? 0 : INFINITY;
	return out;
}

// Calculate pairwise energy for each pair ij, where atom i is provided
double energy_pairwise(const Coordinates &c, const int &i) {
	double U(0.0);
	for (int j = 0; j < N; j++) {
		if (j == i) { // skip atom i
			continue;
		} else if (U == INFINITY) { 
			break;
		} 
		U += hard_sphere(min_image_dist(c.positions[i], c.positions[j]));
	}	
	return U;
}

// Run metropolis acceptance criteria algorithm
// Returns bool indicating whether new config was accepted or not
bool metropolis(Coordinates &c, const double &cutoff) {
	bool accepted = true;
	int selected = rand() % N;
	// Preserving old point
	Point3D xyz_old(c.positions[selected]); 
	double U_old = energy_pairwise(c, selected);
	c.positions[selected] = perturb(c.positions[selected], cutoff);
	wrap_coords(c.positions[selected]);
	double deltaU = energy_pairwise(c, selected) - U_old;
	if (deltaU <= 0) {
		// do nothing
	} else {
		double R(RandomDouble(0, 1.0));
		if (exp(-1/(k_B*T)*deltaU) > R) {
			// do nothing
		} else {
			// REJECT - revert to coordinate before perturbation
			accepted = false;
			c.positions[selected] = xyz_old;
		}
	}
	return accepted;
}

// Run MC simulation with specified number of iterations 
// and dump frequency. Dump frames to 'dump.xyz'.
void simulate(Coordinates &c, const double &cutoff, const long &iters, 
	const int &dump_freq) {
	bool accepted;
	int accepts(0); int total(0);
	ofstream out("dump.xyz", ios_base::app);
	for (int i = 1; i <= iters; i++) {
		accepted = metropolis(c, cutoff);
		if (accepted) {
			accepts++;
		}
		total++;

		if (i % dump_freq == 0) {
			cout << "Iteration Number: " << i << "..." << endl;
			cout << "Acceptance Ratio: " << (double) accepts / total << endl;
			out << c;	
		}
	}
}

void tests() {
	cout << "---RUNNING TESTS---" << endl;
	cout << "Creating a point..." << endl;
	Point3D point = Point3D();
	cout << "Point: " << point << endl;

	point = Point3D(1.0, 2.0, -5.0);
	cout << "Point: " << point << endl;

	Point3D point2 = Point3D(point);
	cout << "Point2: " << point2 << endl;

	cout << "TEST: ADDING POINT TO POINT2 ..." << endl;
	point2 += point;
	cout << "Point2: " << point2 << endl;	

	cout << "TEST: POINT2 NORM ..." << endl;
	cout << point2.norm() << endl;	
	cout << "TEST: MINIMUM IMAGE CORRECTION FOR POINT2 & POINT ..." << endl;
	cout << min_image_dist(point2, point) << endl;

	cout << "TEST: PERTURBATION OF POINT2 WITH CUTOFF = 1.0 ..." << endl;
	point = perturb(point2, -1.0);
	cout << point << endl;

	cout << "TEST: HARD SPHERE POTENTIAL ..." << endl;
	cout << "D = 0.5, hard_sphere = " << hard_sphere(0.5) << endl;
	cout << "D = 12, hard_sphere = " << hard_sphere(12) << endl;

	cout << "---END OF TESTS---" << endl;
	exit(1);
}
