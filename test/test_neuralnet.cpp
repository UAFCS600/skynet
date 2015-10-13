#include "catch.hpp"
#include "neuralnet.hpp"
#include "api_outline.hpp"
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

TEST_CASE ("Neural Network API ") {

	ai::checkers_board_t initial_board =
	"r_r_r_r_" //7
	"_r_r_r_r" //15
	"r_r_r_r_" //23
	"________" //31
	"________" //39
	"_b_b_b_b" //47
	"b_b_b_b_" //55
	"_b_b_b_b";//63

	// Would need to track a case where pieces are added invalidly
	SECTION ("Board Validation") {
		REQUIRE( true == ai::valid_board(initial_board) );
	}
}
