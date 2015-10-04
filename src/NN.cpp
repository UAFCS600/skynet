#include "NN.h"
#include <random>
using namespace std;
double sigmoid(double x)
{
    return 2./(1+exp(x/(-2.)))-1.;
}

//
//Default constructor
//
//
Network::Network()
{
	addLayer(32);
  topology_.push_back(32);
  topology_.push_back(40);
  topology_.push_back(10);
  topology_.push_back(1);
	addLayer(40);
	addLayer(10);
	addLayer(1);
}

Network::Network(const vector<int> & top)
{
  for(int i=0; i<top.size(); ++i)
  {
    topology_.push_back(top[i]);
    addLayer(top[i]);
  }
}

//
//Copy constructor
//
//
//
Network::Network(const Network & orig)
{
  for(int i=0; i<orig.topology_.size(); ++i)
  {
    topology_.push_back(orig.topology_[i]);
    addLayer(orig.topology_[i]);
  }


  for (unsigned int i=1; i<layer_.size(); ++i)
  {
    for (unsigned int j=0; j<layer_[i].size(); ++j)
    {
      for (unsigned int k=0; k<layer_[i-1].size();++k)
      {

        
        layer_[i][j].weights[k]=orig.layer_[i][j].weights[k];

      }
    }
  } 

}


//
//  Two parametr constructor which takes a topology and a value
//
//
Network::Network(const vector<int> & top, int val)
{
  for(int i=0; i<top.size();++i)
  {
    topology_.push_back(top[i]);
    addLayer(top[i]);
  }

  for (unsigned int i=1; i<layer_.size(); ++i)
  {
    for (unsigned int j=0; j<layer_[i].size(); ++j)
    {
      for (unsigned int k=0; k<layer_[i-1].size();++k)
      {

        
        layer_[i][j].weights[k]=val;

      }
    }
  } 
}


void Network::seed()
{
   // Seed with a real random value, if available
  random_device rd;
    
  mt19937 gen(rd());
  uniform_real_distribution<> dis(-1,1);

  for (unsigned int i=1; i<layer_.size(); ++i)
  {
    for (unsigned int j=0; j<layer_[i].size(); ++j)
    {
      for (unsigned int k=0; k<layer_[i-1].size();++k)
      {

        
        layer_[i][j].weights[k]=dis(gen);

      }
    }
  } 
}




void Network::print()
{
	  
  for (unsigned int i=0; i<layer_.size(); ++i)
  {
    cout  <<  i << ": ";
    for (unsigned int j=0; j<layer_[i].size(); ++j)
    {
      cout << "N";
    }
    cout <<  " ("  << layer_[i][0].weights.size() << ")" << endl;
  }

}



double Network::evaluator(vector<double> input)
{
 for (unsigned int i=0; i<input.size(); ++i)
  {
    layer_[0][i].value=input[i];
  }
 
  for (unsigned int i=1; i<layer_.size(); ++i)
  {
    for (unsigned int j=0; j<layer_[i].size(); ++j)
    {
      double temp=0.0;
      for (unsigned int k=0; k<layer_[i-1].size();++k)
      {

        temp+=layer_[i-1][k].value *( layer_[i][j].weights[k]);

      }
    layer_[i][j].value=sigmoid(temp);
    }
  }
  return layer_[layer_.size()-1][0].value;
}

/**
*addLayer function
*takes an integer for teh size and returns nothing. 
*adds a vector of nodes to the layer_ data member.
*
*/
void Network::addLayer(int size)
{
  vector<Node> nodes;
  int numweights=0;
  if (layer_.size()>0)
  {
    numweights=layer_[layer_.size()-1].size();
  }
  for (unsigned int i=0; i<size; ++i)
  {
    Node thenode(numweights, 0.1);
    nodes.push_back(thenode);
  }

  layer_.push_back(nodes);
}


