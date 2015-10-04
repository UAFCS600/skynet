#include "NN.h"
#include "UnitTester.h"
#include <chrono>
#include <iostream>
#include <random>				//for C++11 random
using namespace std;



int main()
{

	//make a vector with some topology
	//fill it with random values
	vector<int> topo={10,40,32,1};
	Network NN(topo);
	NN.seed();
	NN.print();

	random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(-1,1);
	vector<double> input;
	for(int i=0; i<10; ++i)
	{
		input.push_back(dis(gen));
	}

	//The actual timing portion of the code
	chrono::time_point<std::chrono::system_clock> start, end;	
  start = chrono::system_clock::now();

  for(int i=0; i<100000; ++i)
  {	
    NN.evaluator(input);	
  }

  end = chrono::system_clock::now();	
  chrono::duration<double> elapsed_seconds = end-start;
  cout << "Total time = " << elapsed_seconds.count() << endl;
  cout << "Time Per Board = " << elapsed_seconds.count()/100000.0 << endl;
	return 0;
}