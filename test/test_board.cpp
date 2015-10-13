#include "catch.hpp"
#include "neuralnet.hpp"
#include "api_outline.hpp"
#include <algorithm>
#include <iostream>

void print_board(ai::checkers_board_t& board)
{
	for (size_t ii = 0; ii < board.size(); ii += 8) {
		std::cout << board.substr(ii, 8) << std::endl;
	}
}

bool check_boards(ai::checkers_board_t& expected, const ai::checkers_board_list_t& board_list)
{
	bool board_found = true;
	if (std::find(board_list.begin(), board_list.end(), expected) == board_list.end()) {
		board_found = false;
		std::cout << "CHECKING " << std::endl;
		print_board(expected);
		std::cout << "AGAINST" << std::endl;
		for(auto board : board_list) {
			print_board(board);
		}
	}
	return board_found;
}

TEST_CASE ("Helper Functions") {
	ai::checkers_board_t null = "";
	ai::checkers_board_list_t list(3, "2");
	REQUIRE( false == check_boards(null, list) );
}

TEST_CASE ("Board Validation") {

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
		REQUIRE( true == check_boards(expected, red_result) );
	}
}
