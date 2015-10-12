#include "catch.hpp"
#include "neuralnet.hpp"
#include <vector>

const std::vector<double> eval_data_null(32, 0);
const std::vector<double> eval_data_full(32, 1);
TEST_CASE ("Neural Network Construction") {
	std::vector<size_t> standard_network_topography = {32, 40, 10, 1};
	std::vector<size_t> topography = {};
	std::vector<double> invalid_weights(0.0, 0);
	std::vector<double> weights(1690, 1.0); //TODO: Show how many weights are required?

	SECTION ("Construction Exceptions") {
		REQUIRE_THROWS( neuralnet_t base_network(standard_network_topography, invalid_weights) );
		REQUIRE_THROWS( neuralnet_t base_network(topography, invalid_weights) );
		
		neuralnet_t base_network(standard_network_topography, weights);
	}

	SECTION ("Evaluation") {
		int result = 0;

		neuralnet_t base_network(standard_network_topography, weights);
		result = base_network.evaluate(eval_data_null, 0);
		REQUIRE( 0 == result );
	}
}
