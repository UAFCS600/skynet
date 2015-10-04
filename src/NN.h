/************************************
*
* The header for my neural network class
* Derived from my old one from 2008
*
*
* Brandon Marken (c) 2013
* bamarken@alaska.edu
*
************************************/

#ifndef NN_H_INCLUDED
#define NN_H_INCLUDED
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <random>
using namespace std;

/**sigmoid function*/
double sigmoid(double x);


/***************
*
* The Node Struct
* provides the nodes for the neural network class
*
*
*
*
******************/
struct  Node{
  //data memberes
  vector<double> weights;
  double value;
  //constructors
  Node(const vector<double> w) : weights(w)
  {}
  Node(int size, double value)
  {
    weights.assign(size,value);
  }


};


class  Network
{
public:
	Network();
  Network(const Network & orig);
  Network(const vector<int> & top, int val);      //tests for constant valued
  Network(const vector<int> & top); 
  
	void seed();
	void print();
	double evaluator(vector<double> input);

private:
	 void addLayer(int size);
private:	
	vector< vector<Node> > layer_;
  vector<int> topology_;        //a vector of ints for the network topology
};

#endif
