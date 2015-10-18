#include <algorithm>
#include <iostream>
#include "api_outline.hpp"
#include "catch.hpp"
#include "neuralnet.hpp"
#include "board.hpp"


void check_boards(ai::checkers_board_list_t& expected, const ai::checkers_board_list_t& actual)
{
	for (auto it = expected.begin(); it != expected.end(); ++it) {
		auto location = std::find(actual.begin(), actual.end(), *it);
		REQUIRE( location != actual.end() );
	}
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

	SECTION ("Single Red King Left Edge") {
		ai::checkers_board_t red_king =
			"____________R__________________b"; //Position 12: 8, 16
		ai::checkers_board_list_t expected = {
			"________R______________________b", //Result 8
			"________________R______________b"}; //Result 16

		std::cout << getBoard(red_king) << std::endl;

		ai::checkers_board_list_t actual = ai::move_generator(red_king, "red");

		REQUIRE( 2 == actual.size() );
	
		check_boards(expected, actual);
	}

	SECTION ("Single Red King Right Edge") {
		ai::checkers_board_t red_king =
			"_______________R_______________b"; //Position 15: 11, 19
		ai::checkers_board_list_t expected = {
			"___________R___________________b", //Result 11
			"___________________R___________b"}; //Result 19

		std::cout << getBoard(red_king) << std::endl;

		ai::checkers_board_list_t actual = ai::move_generator(red_king, "red");

		REQUIRE( 2 == actual.size() );

		check_boards(expected, actual);
	}

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

	SECTION ("Red Initial Moves") {
		ai::checkers_board_list_t expected = {
			"rrrrrrrr_rrrr_______bbbbbbbbbbbb",
			"rrrrrrrr_rrr_r______bbbbbbbbbbbb",
			"rrrrrrrrr_rr_r______bbbbbbbbbbbb",
			"rrrrrrrrr_rr__r_____bbbbbbbbbbbb",
			"rrrrrrrrrr_r__r_____bbbbbbbbbbbb",
			"rrrrrrrrrr_r___r____bbbbbbbbbbbb",
			"rrrrrrrrrrr____r____bbbbbbbbbbbb"};

		ai::checkers_board_list_t actual = ai::move_generator(initial_board, "red");

		REQUIRE( expected.size() == actual.size() ); 

		check_boards(expected, actual);
	}
}
