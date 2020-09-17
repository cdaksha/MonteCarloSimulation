# Monte Carlo Simulation Makefile

# *****************************************************
# Variables to control Makefile operation

CXX = clang++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o Coordinates.o Point3D.o
	$(CXX) $(CXXFLAGS) -o main main.o Coordinates.o Point3D.o

main.o: Coordinates.hpp Point3D.hpp 

Coordinates.o: Point3D.hpp

Point3D.o: Point3D.hpp
