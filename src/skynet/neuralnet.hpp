/*! \file */

#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <cstdlib>
#include <vector>

namespace skynet
{
	/**
	\class neuralnet_t
	*Stores and performs evaluations for a standard feed forward neural network in the form of weights.
	*/
	class neuralnet_t
	{
		public:
			/**
			*Will throw std::runtime_error on bad input.
			\param layers				A vector of size_t stating the layer layout (blondie24 ex: {32,24,16,1}).
			\param weights				A vector of double that stores the weights for the neural network.
			*/
			neuralnet_t(const std::vector<size_t>& layers,const std::vector<double>& weights);


			/**
			*Will throw std::runtime_error on sigmoid index or invalid constants.
			\param inputs 				Layer 0 input values to perform evalation on.
			\param sigmoid_index 		A value of 0 indicates use y=x and 1 indicates use y=a/(1+e^(-x/b))+c;
			\param a					Index 1 constant.
			\param b					Index 1 constant (cannot be 0).
			\param c					Index 1 constant.
			*/
			double evaluate(const std::vector<double>& inputs,const size_t sigmoid_index,
				const double a=1,const double b=1,const double c=0);

		private:
			struct node_t
			{
				std::vector<double> weights;
				double value;
			};

			void create_node_layers_m();
			void set_node_weights_m(const std::vector<double>& weights);

			std::vector<size_t> layers_m;
			std::vector<std::vector<node_t>> node_layers_m;
	};
}

#endif
