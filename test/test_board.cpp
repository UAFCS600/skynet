#include <algorithm>
#include <iostream>
#include "api_outline.hpp"
#include "catch.hpp"
#include "neuralnet.hpp"
#include "board.hpp"

//TODO: move helper functions into own file.
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
	ai::checkers_board_list_t boardList(3, "2");
	REQUIRE( false == check_boards(null, boardList) );
}

TEST_CASE ("Invalid Boards")
{
	//test for invalid characters
	ai::checkers_board_t invalBoard = 
	"bbbbbbbbbbbb________rrrrrrarrrrr";
	REQUIRE( false == is_valid_checkers_board(invalBoard) );
}

TEST_CASE ("Single Piece Moves") {

	ai::checkers_board_t red_king =
	"________" //7  layer 0
	"________" //15 layer 1
	"________" //23   .
	"____R___" //31   .
	"________" //39   .
	"________" //47
	"________" //55
	"________";//63 layer 7

	std::cout << getBoard(red_king) << std::endl;
	std::string blank = "________";
	std::string lhs = "___R____";
	std::string rhs = "_____R__";

	ai::checkers_board_list_t red_states = ai::move_generator(red_king, "r");
	REQUIRE( 4 == red_states.size() );

	ai::checkers_board_t expected = blank +
					blank +
					lhs +
					blank +
					blank +
					blank +
					blank +
					blank;
	REQUIRE( true == check_boards(expected, red_states) );
}

TEST_CASE ("Board Validation") {

	ai::checkers_board_t initial_board =
	"_r_r_r_r" //7  layer 0
	"r_r_r_r_" //15 layer 1
	"_r_r_r_r" //23   .
	"________" //31   .
	"________" //39   .
	"b_b_b_b_" //47
	"_b_b_b_b" //55
	"b_b_b_b_";//63 layer 7

	// Would need to track a case where pieces are added invalidly
	SECTION ("Board Validation") {
		REQUIRE( true == ai::valid_board(initial_board) );
	}
}

TEST_CASE ("Initial Moves") {
	ai::checkers_board_t initial_board =
	"_r_r_r_r" //7
	"r_r_r_r_" //15
	"_r_r_r_r" //23
	"________" //31
	"________" //39
	"b_b_b_b_" //47
	"_b_b_b_b" //55
	"b_b_b_b_";//63

	std::string red_states_layers01="_r_r_r_r"
					"r_r_r_r_";

	std::vector<std::string> red_states_layer2;
	std::vector<std::string> red_states_layer3;

	red_states_layer2.emplace_back("___r_r_r");
	red_states_layer3.emplace_back("r_______");

	red_states_layer2.emplace_back("___r_r_r");
	red_states_layer3.emplace_back("__r_____");

	red_states_layer2.emplace_back("_r___r_r");
	red_states_layer3.emplace_back("__r_____");

	red_states_layer2.emplace_back("_r___r_r");
	red_states_layer3.emplace_back("____r___");

	red_states_layer2.emplace_back("_r_r___r");
	red_states_layer3.emplace_back("____r___");

	red_states_layer2.emplace_back("_r_r___r");
	red_states_layer3.emplace_back("______r_");

	red_states_layer2.emplace_back("_r_r_r__");
	red_states_layer3.emplace_back("______r_");

	std::string blk_state = "________"
				"b_b_b_b_"
				"_b_b_b_b"
				"b_b_b_b_";

	ai::checkers_player_t red = "red";
	ai::checkers_board_list_t red_result = ai::move_generator(initial_board, red);

	REQUIRE( 7 == red_result.size() );

	for (size_t ii = 0; ii <= red_states_layer2.size(); ++ii) {
		std::string expected = red_states_layers01 +
					red_states_layer2[ii] +
					red_states_layer3[ii] +
					blk_state;
		REQUIRE( true == check_boards(expected, red_result) );
	}
}
