#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <cstdlib>
#include <vector>

namespace skynet
{
	class neuralnet_t
	{
		struct node_t
		{
			std::vector<double> weights;
			double value;
		};

		public:
			neuralnet_t(const std::vector<size_t>& layers,const std::vector<double>& weights);
			double evaluate(const std::vector<double>& inputs,const size_t sigmoid_index,
				const double a=1,const double b=1,const double c=0);

		private:
			void create_node_layers_m();
			void set_node_weights_m(const std::vector<double>& weights);

			std::vector<size_t> layers_m;
			std::vector<std::vector<node_t>> node_layers_m;
	};
}

#endif
