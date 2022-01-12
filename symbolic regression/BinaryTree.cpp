#include "BinaryTree.h"
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <random>
#include <thread>


using namespace std;

/* ****************** tree class functions *******************/
//constructor:
Tree::Tree() {
	int maxLevel = 8;
	nodeList.resize(1000);
	//fillTree();
}

//constructor:
Tree::Tree(int maxDepth) {
	int maxLevel = maxDepth;
	nodeList.resize(1500);
	fillTree();
}

Tree::~Tree(){}

//called by constructor to fill tree.
void Tree::fillTree(int index) { //varible argument
	if (nodeList.capacity() < (index * 2 + 2)) { // plus 2 becasue index starts at 1
		nodeList.resize(index * 2 + 2); 
	}

	PicNodeEntry(index); // add the new node, the node type depends on level
	if (!nodeList[index].isOp)
		return;
	//fill the children of the current node, if not unitary fill with 2 kids.
	fillTree((index * 2) + 1); // put child of unitary on the right sin(x) not xsin
	if (!(nodeList[index].isUnitary)) { //if not unitary add the other child too
		fillTree(index * 2);
	}
	return;
}

//pics the node probabalistically based on tree depth 
void Tree::PicNodeEntry(int index) {
	
	int pick = round(RangedRand(0, pow(maxLevel,2))); //(0-mxlev^2)

	if (pick > pow(getLevel(index),2)) { //pick  op
		int which_op = round(RangedRand(2, 7)); //returns 2-7
		setNode(index, which_op); // code 1 is for constant
	} else {
		int x_or_const = round(RangedRand(0, 1));
		setNode(index, x_or_const); // code 1 is for constant
	}
}

//sets the node given an index and code
void Tree::setNode(int index, int code) { //var arg because a float will not be sent when not a constant
	if (nodeList.capacity() < (index * 2 + 2)) { // plus 2 becasue index starts at 1
		nodeList.resize(index * 2 + 2);
	}
	nodeList[index].code = code; // set the code property in the struct
	nodeList[index].isFilled = true;
	nodeList[index].constant = 0;
	switch (code) {
	case 0: // 0 code == x variable
		nodeList[index].isOp = false;
		/*nodeList[index * 2].isFilled = false;
		nodeList[index * 2+1].isFilled = false;*/
		break;
	case 1: // constant
		nodeList[index * 2].isFilled = false;
		//nodeList[index * 2+1].isFilled = false;
		nodeList[index].isOp = false;
		nodeList[index].constant = (RangedRand(-10, 10)); // this needs fixing.
		break;
	case 2: // +
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = false;
		break;
	case 3: // - 
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = false;
		break;
	case 4: // /
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = false;
		break;
	case 5: // *
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = false;
		break;
	case 6: // sin
		//nodeList[index * 2].isFilled = false; //setting left child false if not
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = true;
		break;
	case 7: // cos
		//nodeList[index * 2].isFilled = false; //setting left child false if not
		nodeList[index].isOp = true;
		nodeList[index].isUnitary = true;
		break;
	}
	 
}

//retunrs the level given an index
int Tree::getLevel(int index) {
	int count = 1;
	while (!( 1 == index)){
		count++;
		index = floor(index / 2);
	}
	return count; 
}

//prints the formula from the tree as a string
void Tree::print(int index) {
	if (!nodeList[index].isFilled) {
	return;
	}

	/* first recur on left child */
	cout << "(";
	print(index*2);

	/* then print the data of node */
	std::cout << returnSymbolicOP(nodeList[index]) << " ";

	/* now recur on right child */
	print(index*2 + 1);
	cout << ")";
}

//mutates the tree, stregth is from 0 to 100.
//stregth is the prob of each node being mutated
void Tree::mutateTree(int mutationStrength, int index) {
	if (false == nodeList[index].isFilled) {
		return;
	}
	/* first recur on left child */
	mutateTree(mutationStrength, index * 2);

	//point mutations:
	if (mutationStrength > round(RangedRand(0, 50))) { // pobabalistic mutation
		nodeList[index];
		if (nodeList[index].code < 2) { // if x or constant
			setNode(index, round(RangedRand(0, 1)));
		}
		else if (nodeList[index].code < 6) {
			setNode(index, round(RangedRand(2, 5))); // change binary op
		}
		else {
			setNode(index, round(RangedRand(6, 7))); // change unary
		}
	}
	//heavier mutation:
	if (mutationStrength > RangedRand(0, 300)) {
		if (nodeList.capacity() < (index * 2 + 2)) { // plus 2 becasue index starts at 1
			nodeList.resize(index * 2 + 2);
		}
		fillTree(index); // rewrite a branch
	}

	/* now recur on right child */
	mutateTree(mutationStrength, index * 2 + 1);
}
int Tree::getdepth(int index) { //return the depth of the longest branch
	if (!nodeList[index].isFilled)
		return 0;
	else {
		int left = getdepth(index * 2);
		int right = getdepth(index * 2 + 1);
		if (left > right)
			return left +1 ;
		else return right +1;
	}
}

int Tree::getCompexity(int index) {
	if (!nodeList[index].isFilled)
		return 0;
	else {
		int left = getCompexity(index * 2);
		int right = getCompexity(index * 2 + 1);

		return left + 1 + right + 1;
	}
}

/* ****************** population class functions *******************/

//constructor
Population::Population(int popSize, std::vector<point> points) {
	populationSize = popSize;
	trees.resize(populationSize);
	data = points; // setting the property data equal to the data we want to match
	cout << populationSize;
	for (int i = 0; i < populationSize; i++) {
		trees[i] = Tree(8);
		cout << " \n";
		trees[i].error = evaluation(trees[i], smalldata);
		cout << "here is the init line up: \n";
		trees[i].print();
	}
	smallestError = trees[0].error;
	cout << "smallest error to start:  " << smallestError;
	errorList.push_back(smallestError);
}

Population::Population(int popSize, std::vector<point> points, std::vector<point> smallpoint, std::vector<point> validation) {
	populationSize = popSize;
	trees.resize(populationSize);
	data = points; // setting the property data equal to the data we want to match
	smalldata = smallpoint;
	validationSet = validation;
	cout << populationSize;
	for (int i = 0; i < populationSize; i++) {
		trees[i] = Tree(8);
		trees[i].error = evaluation(trees[i], smalldata);
	}
	smallestError = trees[0].error;
	cout << "smallest error to start:  " << smallestError;
	errorList.push_back(smallestError);
	cout << "finishd building the pop\n";
}
//this is the random search alg:
Tree Population::randomSearch(int evaluations) {
	cout << "random time";
	for (int i = 0; i < evaluations; i++) {
		float newError = evaluation2(trees[0], smalldata);
		cout << newError << "	" << trees[0].getCompexity(1) << "\n";
		if (smallestError > newError) {
			errorList.push_back(newError); // adding the error to the list of best errors
			smallestError = newError;
			bestSolution = trees[0];
			//cout << newError << "," << i << "\n";
			/*cout << newError << "\n";*/
		}
		//clear and refill the tree with a new random equation
		trees[0].nodeList.clear();
		trees[0].nodeList.resize(511);
		trees[0].fillTree();
	}
	cout << "\n done, final random output error is: \n " << smallestError << " and the best solution is: \n";
	cout << "\n" << "here is the size: " << size(errorList) << "\n";
	return bestSolution;
}

Tree Population::evoSearchSexual(int generations, int numSaved) {
	cout << "called";
	if (numSaved > populationSize) {
		cout << "<ERROR> you cant save more trees than are in the forest";
		return trees[0];
	}
	for (int i = 0; i < generations; i++) { //this is the loop that runs each interation
		//if (i % 5 == 0)
		//cout << i << ",	"<< diversityCalc() << "\n";
		std::sort(trees.begin(), trees.end(), compareTrees); // does reordering, best first
		if (trees[0].error < smallestError) {
			errorList.push_back(trees[0].error);
			smallestError = trees[0].error; // reset the bestDist to the new best value
			bestSolution = trees[0];//saving the member if it is an all time best.
		}
		//replace the falures with mutated parents.
		selectForBreed(numSaved);
	}
	return bestSolution;
}

void Population::selectForBreed(int numSaved) {
	for (int i = (numSaved); i < (populationSize); i++) { // run through the non saved part of the population:
		int parent1 = round(RangedRand(0, numSaved));
		int parent2 = round(RangedRand(0, numSaved));

		breed(parent1, 1, parent2, 1, i, 1);
		trees[parent1].mutateTree(3, i); //very mild mutation
		trees[parent2].mutateTree(6, i); //very mild mutation
		trees[i].error = evaluation2(trees[i], smalldata); //add distance property
		trees[i].mutateTree(10, i); //very mild mutation
		//cout << trees[i].error << " is the child error\n";
	}
}

void Population::breed(int pGiving, int pGIndex, int pBackUp, int pBIndex, int child, int childIndex) {
	if (!trees[pGiving].nodeList[pGIndex].isFilled) {
			return;
	}
	//give to the child from parent index
	trees[child].setNode(childIndex, trees[pGiving].nodeList[pGIndex].code); // clean up flags
	trees[child].nodeList[childIndex] = trees[pGiving].nodeList[pGIndex];

	if (2 > RangedRand(0, 10)) {//2- are constants, unitary is fine of right
		pBIndex = chooseNewBranch(pGiving, pGIndex, pBackUp, pBIndex, child, childIndex);
		if (!pBIndex == 0) {
			breed(pBackUp, pBIndex, pGiving, pGIndex, child, childIndex); // swap parents rewrite the current node.
		}
	}
	else {

		//left side of tree: 
		breed(pGiving, pGIndex * 2, pBackUp, pBIndex, child, childIndex * 2);

		//right side of tree: 
		breed(pGiving, pGIndex * 2 + 1, pBackUp, pBIndex, child, childIndex * 2 + 1);
	}
}
// finds a good branch to give from the BackupParent, they will become the main parent, return index
int Population::chooseNewBranch(int pGiving, int pGIndex, int pBackUp, int pBIndex, int child, int childIndex) {
	int childLevel = Tree::getLevel(childIndex); // using the static method from Tree class
	
	int NewParentDepth = trees[pBackUp].getdepth(1);
	// make a list of possible nodes:
	std::vector<int> listOfNodes;
	int range = (pow(2, 8) - 1) - (pow(2, childLevel));
	for (int i = 0; i < range; i++) {
		if ((trees[pBackUp].nodeList[i].isFilled) && ((childLevel + trees[pBackUp].getdepth(i)) < 9)) {
			listOfNodes.push_back(i);
		}
	}
	if (listOfNodes.size() == 0) {
		//cout << "ERROR in the chooseNewBranch";
		return 0;
	}
	else if (1 > RangedRand(0, 2)) { // 50/50 shot it takes the longest one.
		return listOfNodes[listOfNodes.size() - 1];
	} 
	else
		return listOfNodes[round(RangedRand(0, listOfNodes.size() - 1))];
}

//This is a thredded version that I never got working IDK why
//void Population::evoSearch2(int generations, int numSaved) {
//	if (numSaved > populationSize) {
//		cout << "<ERROR> you cant save more trees than are in the forest";
//		return;
//	}
//
//	for (int i = 0; i < generations; i++) { //this is the loop that runs each interation
//		std::sort(trees.begin(), trees.end(), compareTrees); // does reordering, best first
//		if (trees[0].error < smallestError) {
//			smallestError = trees[0].error; // reset the bestDist to the new best value
//			errorList.push_back(trees[0].error);
//			cout <<"new best of: "<< trees[i].error << "on generation: "<< i;
//			cout << "\n";
//			bestSolution = trees[0];//saving the member if it is an all time best.
//		}
//		mutatePop(numSaved);
//		attachErrors(populationSize, trees, data);
//	}
//}
////this is a evolutionary beam search
//void attachErrors(int populationSize, std::vector<Tree>& trees, std::vector<point> data) {
//	
//	std::vector <std::thread> threads;
//	std::vector<float> errors(populationSize);
//	for (int i = 0; i < populationSize; i++) {
//		std::thread t(evalPop, std::ref(trees[i]), std::ref(errors), i, data);
//		threads.push_back(std::move(t)); // Instead of copying, move t into the vector (Less expensive)
//	}
//	// Now wait for the threads to finish,
//	for (auto& t : threads) {
//		t.join();
//	}
//}
Tree Population::evoSearch(int generations, int numSaved) {
	if (numSaved > populationSize) {
		cout << "<ERROR> you cant save more trees than are in the forest";
		return trees[0];
	}
	for (int i = 0; i < generations; i++) { //this is the loop that runs each interation
		std::sort(trees.begin(), trees.end(), compareTrees); // does reordering, best first
		if (trees[0].error < smallestError) {
			smallestError = trees[0].error; // reset the bestDist to the new best value
			errorList.push_back(trees[0].error);
			bestSolution = trees[0];//saving the member if it is an all time best.
			trees[0].print();
		}
		cout << smallestError;
		cout << "\n";
		//replace the falures with mutated parents.
		mutatePop(numSaved);

	}
	return bestSolution;
}

void Population::mutatePop(int numSaved) {
	int whichParent = 0;
	for (int i = (numSaved); i < (populationSize); i++) { // run through the non saved part of the population:
		trees[i] = trees[whichParent];//copying the parent into the falure's spot
		trees[i].mutateTree(30);
		trees[i].error = evaluation2(trees[i], smalldata); //add distance property
		whichParent++;
		if (whichParent == numSaved)
			whichParent = 0;
	}

	for (int i = 0; i < numSaved; i++) { //lastly shuffle the parents
		trees[i].mutateTree(5);
		trees[i].error = evaluation2(trees[i], smalldata); //add distance property
	}
}

Tree Population::hillclimber(int evaluations) {
	cout << "called hill climber \n";
	
	for (int i = 0; i < evaluations; i++) { //this is the loop that runs each interation
		cout << trees[0].error << "\n";
		errorList.push_back(trees[0].error);
		if (trees[0].error < smallestError) {
			smallestError = trees[0].error; // reset the bestDist to the new best value
			bestSolution = trees[0];//saving the member if it is an all time best.
		}
		//replace the falures with mutated parents.
		climbHill(trees[0], 1);
	}
	return bestSolution;
}

void Population::climbHill(Tree &tree, int index){
	if (!tree.nodeList[index].isFilled) {
		return;
	}
	/* first recur on left child */

	climbHill(tree, index * 2);

	int nodeCode = tree.nodeList[index].code;
	if (nodeCode > 1)
		nodeCode = 2;
	else if (nodeCode > 5)
		nodeCode = 3;
	float newError;
	float save;

	switch (nodeCode) {
	case 0: // 0 code == x variable
		tree.nodeList[index].constant = RangedRand(-10, 10);
		tree.nodeList[index].code = 1;
		newError = evaluation(tree, smalldata);
		if (newError < tree.error) {
			tree.error = newError;
		}
		else {
			tree.nodeList[index].code = 0; // wasn't better
		}
		break;
	case 1: // constant
		save = tree.nodeList[index].constant;
		tree.nodeList[index].constant = RangedRand(-10, 10);
		newError = evaluation(tree, smalldata);
		if (newError < tree.error) {
			tree.error = newError;
		}
		else {
			tree.nodeList[index].constant = save; // wasn't better
		}
		break;
	case 2: // +
		save = tree.nodeList[index].code;
		tree.nodeList[index].code = round(RangedRand(2, 5));
		newError = evaluation(tree, smalldata);
		if (newError < tree.error) {
			tree.error = newError;
		}
		else {
			tree.nodeList[index].code = save; // wasn't better
		}
		break;
	case 3:
		 save = tree.nodeList[index].code;
		tree.nodeList[index].code = round(RangedRand(6,7));
		 newError = evaluation(tree, smalldata);
		if (newError < tree.error) {
			tree.error = newError;
		}
		else {
			tree.nodeList[index].code = save; // wasn't better
		}
		break;
	}
	/* now recur on right child */
	climbHill(tree, index * 2 + 1);
}


void Population::Climbconstant(Tree& tree, int index) {
	if (!tree.nodeList[index].isFilled) {
		return;
	}
	/* first recur on left child */

	Climbconstant(tree, index * 2);

	if (tree.nodeList[index].code == 1) {
		float oldError = 100;
		float save = tree.nodeList[index].constant;

		do {
			save = tree.nodeList[index].constant;
			if (abs(tree.nodeList[index].constant) > 10)
				break;
			tree.nodeList[index].constant += .5;
			oldError = tree.error;
			tree.error = evaluation(tree, smalldata);
			cout << "working + \n";
		} while(oldError > tree.error);
		tree.nodeList[index].constant = save;
		oldError = tree.error;

		do  {
			save = tree.nodeList[index].constant;
			if (abs(tree.nodeList[index].constant) > 10)
				break;
			tree.nodeList[index].constant -= .5;
			oldError = tree.error;
			tree.error = evaluation(tree, smalldata);
			cout << "working - \n";
		} while (oldError > tree.error);
		tree.nodeList[index].constant = save;
		tree.error = oldError;
	}

	Climbconstant(tree, index * 2 + 1);
}

float Population::diversityCalc() {
	float diversity = 0;
	for (int i = 0; i < populationSize; i++) {
		diversity += evaluation2(trees[i],smalldata);
	}
	return diversity;
}

/* ****************** non class functions *******************/

//returns a string representation of the node
std::string returnSymbolicOP(node curNode) {
	int code = curNode.code;
	switch (code) {
	case 0: // 0 code == x variable
		return "x";
		break;
	case 1: // constant
		return std::to_string(curNode.constant);
		break;
	case 2: // +
		return "+";
		break;
	case 3: // - 
		return "-";
		break;
	case 4: // /
		return "/";
		break;
	case 5: // *
		return "*";
		break;
	case 6: // sin
		return "sin";
		break;
	case 7: // cos
		return"cos";
		break;
	}
	cout << code << "not found \n";
	return "error";
}

void evalPop(std::vector<Tree>& trees, std::vector<float>& errors, int index, std::vector<point>data) {
	errors[index] = evaluation2(trees[index], data);
	cout << "eval error in thred" << index;
}

// given an x, and tree or "equation" will return a y
//this is by far the slowest function!!!
float evaluatePoint(float x, Tree equation, int index) {
	if (false == equation.nodeList[index].isFilled) { // if it is an empty node
		//this is really only for the uniary oporator and doesn't do anything
		return 0;
	}
	if (!equation.nodeList[index].isOp) { // if it is a constant then return it.
		if (equation.nodeList[index].code == 0) { // is the var x
			return x;
		}
		else {
			return equation.nodeList[index].constant; // is a constant
		}
	}

	float leftval = evaluatePoint(x, equation, index*2);
	float rightval = evaluatePoint(x, equation, index * 2 +1);
	/*cout << " here is the op\n" << leftval << returnSymbolicOP(equation.nodeList[index]) << rightval << '\n';*/

	return doOperation(equation.nodeList[index].code, leftval, rightval);
}

//will return a float given a node code and two constants.
float doOperation(int code, float left, float right) {
	switch (code) {
	case 0: // 0 code == x variable
		cout << "error send a variable instead of a op";
		return 0;
	case 1: // constant
		cout << "error send a constant instead of a op";
		return 0;
	case 2: // +
		return left + right;
	case 3: // - 
		return left - right;
	case 4: // /
		return left / right;
	case 5: // *
		return left * right;
	case 6: // sin
		return sin(right);
	case 7: // cos
		return cos(right);
	}
	cout << code << "error in the evaluation, code not found \n";
	return 0;
}

//returns a Root Mean Squared value given a tree and data set. (threded 
float evaluation(Tree& equation, std::vector<point>& points) {
	double sumationCount = 0;
	int numOfPoints = size(points);
	std::vector <std::thread> threads;
	std::vector <float> distances(1000);
	for (int i = 0; i < numOfPoints; i++) {
		std::thread t(evaluatePoint1, points[i].x, equation, points,i, std::ref(distances));
		threads.push_back(std::move(t)); // Instead of copying, move t into the vector (Less expensive)
	}
	// Now wait for the threads to finish,
	for (auto& t : threads) {
		t.join();
	}
	for (int i = 0; i < numOfPoints; i++) {
		sumationCount += distances[i];
		}
	return sqrt(sumationCount / N);
}

void evaluatePoint1(float x, Tree equation, std::vector<point> points, int index, std::vector <float> &distances) {
	float yActual = points[index].y;
	float yEstimate = evaluatePoint(points[index].x, equation, 1); // i + 1 b/c tree starts at 1.
	//cout << xEstimate << '\n';
	distances[index] = pow(yActual - yEstimate, 2);
}

//this is squential
float evaluation2(Tree equation, std::vector<point> points) {
	double sumationCount = 0;
	int numOfPoints = size(points);
	for (int i = 0; i < numOfPoints; i++) {
		float yActual = points[i].y;
		float yEstimate = evaluatePoint(points[i].x, equation, 1);
		//cout << xEstimate << '\n';
		sumationCount += pow(yActual - yEstimate, 2);
	}
	return sqrt(sumationCount / numOfPoints);
}

float RangedRand(int min, int max)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(min* 100000, max* 100000);
	float randVal = static_cast<float>(dist(gen)) / 100000;
	return randVal;
}

bool compareTrees(Tree t1, Tree t2)
{
	return (t1.error < t2.error);
}


