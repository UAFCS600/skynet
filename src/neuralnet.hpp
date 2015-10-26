#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include "expr_parser.hpp"

#include <cstdlib>
#include <vector>

class neuralnet_t
{
	struct node_t
	{
		std::vector<double> weights;
		double value;
	};

	public:
		neuralnet_t(const std::vector<size_t>& layers,const std::vector<double>& weights);
		double evaluate(const std::vector<double>& inputs,expr_parser_t& sigmoid);

	private:
		void create_node_layers_m();
		void set_node_weights_m(const std::vector<double>& weights);

		std::vector<size_t> layers_m;
		std::vector<std::vector<node_t>> node_layers_m;
};

#endif
