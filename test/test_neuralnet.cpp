#include "catch.hpp"
#include "neuralnet.hpp"
#include "api_outline.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

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
	"r_r_r_r_" //7  layer 0
	"_r_r_r_r" //15 layer 1
	"r_r_r_r_" //23   .
	"________" //31   .
	"________" //39   .
	"_b_b_b_b" //47
	"b_b_b_b_" //55
	"_b_b_b_b";//63 layer 7

	// Would need to track a case where pieces are added invalidly
	SECTION ("Board Validation") {
		REQUIRE( true == ai::valid_board(initial_board) );
	}
}

TEST_CASE ("Initial Moves") {
	ai::checkers_board_t initial_board =
	"r_r_r_r_" //7
	"_r_r_r_r" //15
	"r_r_r_r_" //23
	"________" //31
	"________" //39
	"_b_b_b_b" //47
	"b_b_b_b_" //55
	"_b_b_b_b";//63

	std::string red_states_layers01="r_r_r_r_"
					"_r_r_r_r";

	std::vector<std::string> red_states_layer2;
	std::vector<std::string> red_states_layer3;

	red_states_layer2.emplace_back("__r_r_r_");
	red_states_layer3.emplace_back("_r______");

	red_states_layer2.emplace_back("r___r_r_");
	red_states_layer3.emplace_back("_r______");

	red_states_layer2.emplace_back("r___r_r_");
	red_states_layer3.emplace_back("___r____");

	red_states_layer2.emplace_back("r_r___r_");
	red_states_layer3.emplace_back("___r____");

	red_states_layer2.emplace_back("r_r___r_");
	red_states_layer3.emplace_back("_____r__");

	red_states_layer2.emplace_back("r_r_r___");
	red_states_layer3.emplace_back("_____r__");

	red_states_layer2.emplace_back("r_r_r___");
	red_states_layer3.emplace_back("_____r__");

	std::string blk_state = "________"
				"_b_b_b_b"
				"b_b_b_b_"
				"_b_b_b_b";

	ai::checkers_player_t red = "r";
	ai::checkers_board_list_t red_result = ai::move_generator(initial_board, red);

	REQUIRE( 7 == red_result.size() );
	//ai::checkers_board_list_t red_result;

	for (size_t ii = 0; ii <= red_states_layer2.size(); ++ii) {
		std::string expected = red_states_layers01 +
					red_states_layer2[ii] +
					red_states_layer3[ii] +
					blk_state;
		REQUIRE( std::find(red_result.begin(), red_result.end(), expected) != red_result.end() );
	}
}
