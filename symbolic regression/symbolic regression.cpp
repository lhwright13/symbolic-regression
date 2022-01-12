#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "BinaryTree.h"
#include <ctime>
#include<thread>
#include <cstdlib>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

//prototypes:
std::vector<point> read(); // read in text document
void print(std::vector<point> points); //fun to print a list of points
//The following are used for parallelization
void randSearch(Population &pop, std::vector<point> &points);
void evoBeam(Population& pop, std::vector<point>& points);
void sexualEvoSearch(Population& pop, std::vector<point>& points);
void hillTime(Population& pop, std::vector<point>& points);

//for randomizing stuff
int N = 1000; // number of points in the given set

//used for the random number generator
random_device rd;
mt19937 gen(rd());

// function to read in a file and save it as an array of struct "point"
std::vector<point> read() {
	const char* file =("C:\\Users\\lhwri\\OneDrive\\Desktop\\Experemental Data2\\data.txt");
	std::vector<point> points(1000); // creating an array of N size of the point type

int count = 0;
	ifstream fin;
	fin.open(file);
	if (fin.is_open()) {
		for (int i = 0; i < N; ++i) {
			char delim = ',';
			char delim2 = '/n';
			count++;
			fin >> points[i].x >> delim >> points[i].y >> delim2;
		}
		fin.close();
	}
	return points;
}

//print the list of points
void print(vector<point> points) {
	for (int i = 0; i < size(points)-1; i++) {
		cout << points[i].x << ",\t" << points[i].y << '\n';
	}
}

//prints the contents of vector
void print(std::vector <float> const& a) {
	cout << "The vector elements are :";

	for (int i = 0; i < a.size(); i++)
		std::cout << a.at(i) << ' ';
}

int main() {

	std::vector<point> points = read();
	std::vector<point> smallpoints;
	std::vector<point> validation;

	//dividing points between smallpoints and validation set
	for (int i = 0; i < size(points); i++) {
		if (i % 10 == 0){
			smallpoints.push_back(points[i]);
		}
		else
			validation.push_back(points[i]);
	}
	
	Population pop1(1, points, smallpoints, validation);
	Population pop2(9, points, smallpoints, validation);
	Population pop3(20, points, smallpoints, validation);
	Population pop4(10, points, smallpoints, validation);
	Population pop5(10, points, smallpoints, validation);
	Population pop6(10, points, smallpoints, validation);
	Population pop7(10, points, smallpoints, validation);
	Population pop8(10, points, smallpoints, validation);

	thread th1(hillTime, std::ref(pop1), std::ref(points));
	thread th2(hillTime, std::ref(pop2), std::ref(points));
	thread th3(hillTime, std::ref(pop3), std::ref(points));
	thread th4(hillTime, std::ref(pop4), std::ref(points));
	thread th5(hillTime, std::ref(pop1), std::ref(points));
	thread th6(hillTime, std::ref(pop2), std::ref(points));
	thread th7(hillTime, std::ref(pop3), std::ref(points));
	thread th8(hillTime, std::ref(pop4), std::ref(points));


	thread th1(sexualEvoSearch, std::ref(pop1), std::ref(points));
	thread th2(sexualEvoSearch, std::ref(pop2), std::ref(points));
	thread th3(sexualEvoSearch, std::ref(pop3), std::ref(points));
	thread th4(sexualEvoSearch, std::ref(pop4), std::ref(points));
	thread th5(sexualEvoSearch, std::ref(pop1), std::ref(points));
	thread th6(sexualEvoSearch, std::ref(pop2), std::ref(points));
	thread th7(sexualEvoSearch, std::ref(pop3), std::ref(points));
	thread th8(sexualEvoSearch, std::ref(pop4), std::ref(points));


	 //*********************for evobeam**********************

	thread th1(evoBeam, std::ref(pop1), std::ref(points));
	thread th2(evoBeam, std::ref(pop2), std::ref(points));
	thread th3(evoBeam, std::ref(pop3), std::ref(points));
	thread th4(evoBeam, std::ref(pop4), std::ref(points));
	thread th5(evoBeam, std::ref(pop1), std::ref(points));
	thread th6(evoBeam, std::ref(pop2), std::ref(points));
	thread th7(evoBeam, std::ref(pop3), std::ref(points));
	thread th8(evoBeam, std::ref(pop4), std::ref(points));


	// ******************************** for random search
	

    thread th1(randSearch, std::ref(pop1), std::ref(points));
	thread th2(randSearch, std::ref(pop2), std::ref(points));
	thread th3(randSearch, std::ref(pop3), std::ref(points));
	thread th4(randSearch, std::ref(pop4), std::ref(points));
	thread th5(randSearch, std::ref(pop1), std::ref(points));
	thread th6(randSearch, std::ref(pop2), std::ref(points));
	thread th7(randSearch, std::ref(pop3), std::ref(points));
	thread th8(randSearch, std::ref(pop4), std::ref(points));

	// ******************************** for beam search

	th1.join();
	th2.join();
	th3.join();
	th4.join();


	th5.join();
	th6.join();
	th7.join();
	th8.join();

	cout << "\nfor thred 1\n: ";
	//used to adjust the constants,
	pop1.Climbconstant(pop1.bestSolution, 1);
	pop1.bestSolution.print();
	cout << "\n";
	cout << pop1.smallestError;
	cout << "\n";
	print(pop1.errorList);

	cout << "\nfor thred 2:\n ";
	pop2.Climbconstant(pop2.bestSolution, 1);
	pop2.bestSolution.print();
	cout << "\n";
	cout << pop2.smallestError;
	cout << "\n";
	print(pop2.errorList);

	cout << "\n for thed 3: \n";
	pop3.Climbconstant(pop3.bestSolution, 1);
	pop3.bestSolution.print();
	cout << "\n";
	cout << pop3.smallestError;
	cout << "\n";
	print(pop3.errorList);

	cout << "\n for thred 4:\n ";
	pop3.Climbconstant(pop1.bestSolution, 3);
	pop4.bestSolution.print();
	cout << "\n";
	cout << pop4.smallestError;
	cout << "\n";
	print(pop4.errorList);

	cout << "\n for thred 4:\n ";
	pop5.bestSolution.print();
	cout << "\n";
	cout << pop5.smallestError;
	cout << "\n";
	print(pop5.errorList);

	cout << "\n for thred 4:\n ";
	pop6.bestSolution.print();
	cout << "\n";
	cout << pop6.smallestError;
	cout << "\n";
	print(pop6.errorList);

	cout << "\n for thred 7:\n ";
	pop7.bestSolution.print();
	cout << "\n";
	cout << pop7.smallestError;
	cout << "\n";
	print(pop7.errorList);

	cout << "\n for thred 8:\n ";
	pop8.bestSolution.print();
	cout << "\n";
	cout << pop8.smallestError;
	cout << "\n";
	print(pop8.errorList);

	//_CrtDumpMemoryLeaks(); // for checking if there are memory leaks.
}
//calles the randSearch function for parallelization
void randSearch(Population &pop, std::vector<point> &points) {
	pop.randomSearch(20);
}
//calles the evoSearch function for parallelization
void evoBeam(Population& pop, std::vector<point>& points) {
	pop.evoSearch(3, 3);
}
//calles the sexualEvoSearch function for parallelization
void sexualEvoSearch(Population& pop, std::vector<point>& points) {
	pop.evoSearchSexual(10, 2);
}
//calles the hillclimber function for parallelization
void hillTime(Population& pop, std::vector<point>& points) {
	pop.hillclimber(5);
}

///Other:
//testing eval methods
//Tree test(8); // make test tree
//for (int i = 0; i < 10; i++) {

//	cout << evaluation(test, points);
//	cout << "\n";
//}
//cout << "*********************************\n";
//for (int i = 0; i < 10; i++) {

//	cout << "\n";
//	cout << evaluation2(test, points);
//}
//for (int i = 0; i < 5; i++) {
//	cout << "\n in par: ";
//	cout << evaluation(test, points);
//	cout << "\n in seq: ";
//	cout << evaluation2(test, points);
//}