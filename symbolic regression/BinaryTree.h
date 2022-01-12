#ifndef BinaryTree_H
#define BinaryTree_H

#include <string>
#include <vector>
#include <math.h> 

extern int N; // extern int the number of points

/* ****************** structures *******************/

struct node { // struct for the node
    float constant;
    int code; // this defines the type node
    bool isOp = false; // if an oporator true
    bool isUnitary = false; // if unitary then true
    bool isFilled = false;
};

struct point {//struct to hold 2d points
    float x;
    float y;
};

// function to return the symbolic representation of the node.
std::string returnSymbolicOP(node curNode); 

/* ****************** tree class functions *******************/


class Tree // class to hold a single memeber of a population
{
private:

    void PicNodeEntry(int index); //returns the node that will get set by the fill tree func 

public:
    void setNode(int index, int code);
    float error = 0; // this is the error when compared with the data
    int maxLevel = 8; // by default it will equal 8
    std::vector<node> nodeList; // this is the array that will hold the tree.
    Tree(int maxDepth);
    Tree();
    ~Tree();
    void fillTree(int index = 1); // used to init the tree
    static int getLevel(int index);
    int getdepth(int index);
    int getCompexity(int index);

    void print(int index = 1);
    void mutateTree(int mutationStrength, int index = 1);

};


/* ****************** population class functions *******************/
class Population {

public:
    Population(int popSize, std::vector<point> points); // constructor
    Population(int popSize, std::vector<point> points, std::vector<point> smallpoint, std::vector<point> validation);

    //variables
    float smallestError;
    int populationSize;
    std::vector<float> errorList;


    std::vector<Tree> trees; //holds the forest.

    Tree bestSolution; // holds the tree of the best solution.

    //algorithums
    Tree randomSearch(int evaluations);

    //evolutionary search() non sexual.
    Tree evoSearch(int generations, int numSaved);

    //void evoSearch2(int generations, int numSaved);

    //evolutionary search() sexual with cross over
    Tree evoSearchSexual(int generations, int numSaved);

    Tree hillclimber(int evaluations);

    void climbHill(Tree& tree, int index);

    float diversityCalc();


    void Climbconstant(Tree& tree, int index);


private:

    //used by evoSearchSexual to beed parents into children
    void breed(int pGiving, int pGIndex, int pBackUp, int pBIndex, int child, int childIndex =1); ////// needs to be written

    int chooseNewBranch(int pGiving, int pGIndex, int pBackUp, int pBIndex, int child, int childIndex);
    
    void selectForBreed(int numSaved);
    //hold the points we are trying to match
    std::vector<point> data;
    std::vector<point> smalldata;
    std::vector<point> validationSet;

    //takes the number of trees in the population to save.
    //mutates them and replaces the children with them.
    //should be basically dane, just needs the mutatetree() function 
    void mutatePop(int numSaved);
};
 
/* ****************** non class functions *******************/

void evalPop(std::vector<float>& errors, std::vector<Tree>& trees, int index, std::vector<point> data);

float evaluation(Tree &equation, std::vector<point> &points); //parallel 

float evaluation2(Tree equation, std::vector<point> points); // non-parallel 

void evaluatePoint1(float x, Tree equation, std::vector<point> points, int index, std::vector <float>& distances);

float evaluatePoint(float x, Tree equation, int index = 1);

float doOperation(int code, float left, float right);

float RangedRand(int range_min, int range_max);

bool compareTrees(Tree t1, Tree t2);



#endif