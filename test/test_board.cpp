#include <algorithm>
#include <iostream>
#include "api_outline.hpp"
#include "catch.hpp"
#include "neuralnet.hpp"
#include "board.hpp"

//TODO: move helper functions into own file.
void print_board(ai::checkers_board_t& board)
{
	
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
	invalBoard = 
	"________________________________";
	REQUIRE( false == is_valid_checkers_board(invalBoard) );
}

TEST_CASE ("Single Piece Moves") {

	ai::checkers_board_t red_king =
	"____________R___________________";

	std::cout << getBoard(red_king) << std::endl;
	std::string blank = "____";
	std::string lhs = "";
	std::string rhs = "";

	ai::checkers_board_list_t red_states = ai::move_generator(red_king, "r");
	REQUIRE( 4 == red_states.size() );

	ai::checkers_board_t expected = "";
	REQUIRE( true == check_boards(expected, red_states) );
}

TEST_CASE ("Board Validation") {

	ai::checkers_board_t initial_board =
	"rrrrrrrrrrrr________bbbbbbbbbbbb";

	// Would need to track a case where pieces are added invalidly
	SECTION ("Board Validation") {
		REQUIRE( true == ai::valid_board(initial_board) );
	}
}

TEST_CASE ("Initial Moves") {
	ai::checkers_board_t initial_board =
	"rrrrrrrrrrrr________bbbbbbbbbbbb";
	
	std::string static_red = "rrrrrrrr";
	std::string static_blk = "____bbbbbbbbbbbb";

	ai::checkers_board_list_t red_states;

	std::string piece8 = "_rrr";
	std::string new_location12 = "r___";
	std::string new_location13 = "_r__";

	red_states.emplace_back(static_red + piece8 + new_location12 + static_blk);
	red_states.emplace_back(static_red + piece8 + new_location13 + static_blk);

	std::string piece9 = "r_rr";
	std::string new_location14 = "__r_";
	red_states.emplace_back(static_red + piece9 + new_location13 + static_blk);
	red_states.emplace_back(static_red + piece9 + new_location14 + static_blk);

	std::string piece10 = "rr_r";
	std::string new_location15 = "___r";
	red_states.emplace_back(static_red + piece10 + new_location14 + static_blk);
	red_states.emplace_back(static_red + piece10 + new_location15 + static_blk);

	std::string piece11 = "rrr_";
	red_states.emplace_back(static_red + piece11 + new_location15 + static_blk);


	ai::checkers_player_t red = "red";
	ai::checkers_board_list_t red_result = ai::move_generator(initial_board, red);

	for (auto it = red_states.begin(); it != red_states.end(); ++it) {
		std::cout << *it << std::endl;
	}
	REQUIRE( 7 == red_states.size() ); //Adding in self validation.
	REQUIRE( 7 == red_result.size() );

	bool correct_generation = true;
	for (auto it = red_states.begin(); it != red_states.end(); ++it) {
		auto location = std::find(red_result.begin(), red_result.end(), *it);
		REQUIRE( location != red_result.end() );
	}
}
