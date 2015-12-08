#include "neuralnet.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

/**
*	The default sigmoid function
*	Returns a double
*	
*	Computes a sigmoid in the form 
*	y=a/(1+exp(-x/b))+c
* 	or 
*	y=x
*
*	Arguments: 
*		double x  - x in the sigmoid. No range restrictions. 
*		size_t t  - which sigmoid to use 
*					0 is linear
*					1 is the standard sigmoid 
*		double a  - default value of 1
* 		double b  - default value of 1
*		double c  - default value of 0
*	
*
*/

double sigmoid(double x,size_t index,const double a=1,const double b=1,const double c=0)
{
	switch(index)
	{
		case 0:
			return x;
		case 1:
			return a/(1.0+std::exp(-x/b))+c;
		default:
			throw std::runtime_error("Invalid sigmoid index(expected value 0<=index<=2).");
	};
}



/**
*
*	Two parameter constructor for a neural network type
*	Takes a vector of size_t which tell the size of each layer
*	the number of layers is deduced from the length of that vector
*
*	Another vector of doubles for the weights. 
*
*/
skynet::neuralnet_t::neuralnet_t(const std::vector<size_t>& layers,const std::vector<double>& weights):
	layers_m(layers)
{
	if(layers_m.size()<=0)
		throw std::runtime_error("Invalid number of layers(expected value >= 0).");

	if(layers_m.back()!=1)
		throw std::invalid_argument("Expected last layer value of 1(got "+
			std::to_string(layers_m.back())+").");

	create_node_layers_m();
	set_node_weights_m(weights);
}


/**
*
*	The evaluate member function
*	returns a double which is the evaluated value for the neural network
*	it also takes a size_t for which type of sigmoid to use (detailed in the sigmoid function)
*	then doubles a,b, and c for the sigmoid (parameters again discussed in the sigmoid function)
*
*	evaluates the neural network
*
*/
double skynet::neuralnet_t::evaluate(const std::vector<double>& inputs,
	const size_t sigmoid_index,const double a,const double b,const double c)
{
	if(b==0)
		throw std::runtime_error("Division by 0.");

	for(size_t ii=0;ii<node_layers_m[0].size();++ii)
		node_layers_m[0][ii].value=inputs[ii];

	for(size_t ii=1;ii<node_layers_m.size();++ii)
	{
		for(size_t jj=0;jj<node_layers_m[ii].size();++jj)
		{
			node_layers_m[ii][jj].value=0;

			for(size_t kk=0;kk<node_layers_m[ii-1].size();++kk)
				node_layers_m[ii][jj].value+=node_layers_m[ii-1][kk].value*node_layers_m[ii-1][kk].weights[jj];

			node_layers_m[ii][jj].value=sigmoid(node_layers_m[ii][jj].value,sigmoid_index,a,b,c);
		}
	}

	return node_layers_m[node_layers_m.size()-1][0].value;
}


/**
*	Create Layer Member Function
*
*	Returns nothing. 
*	Takes no parameters
*
*	Adds a layer of nodes onto the neural network
*
*/
void skynet::neuralnet_t::create_node_layers_m()
{
	node_layers_m.clear();

	for(size_t ii=0;ii<layers_m.size();++ii)
	{
		if(ii+1<layers_m.size())
		{
			std::vector<node_t> node_layer;

			for(size_t jj=0;jj<layers_m[ii];++jj)
			{
				node_t node;
				node.value=0;
				node.weights.resize(layers_m[ii+1],1);
				node_layer.push_back(node);
			}

			node_layers_m.push_back(node_layer);
		}
		else
		{
			node_t node;
			node.value=5;
			node.weights.clear();
			node_layers_m.push_back({node});
		}
	}
}


/**
*
*	Set node weights function
*	Sets the weights for a given node
*	
*	returns nothing and takes a vector of doubles for the weights
*	
*
*/
void skynet::neuralnet_t::set_node_weights_m(const std::vector<double>& weights)
{
	std::vector<double*> real_weights;

	for(auto& layer:node_layers_m)
		for(auto& node:layer)
			for(auto& weight:node.weights)
				real_weights.push_back(&weight);

	if(weights.size()!=real_weights.size())
		throw std::runtime_error("Invalid number of weights(expected "+
			std::to_string(real_weights.size())+" got "+std::to_string(weights.size())+").");

	for(size_t ii=0;ii<real_weights.size();++ii)
		*(real_weights[ii])=weights[ii];
}
