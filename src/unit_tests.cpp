#include <algorithm>
#include <vector>

#define CATCH_CONFIG_MAIN
#include <Catch/catch.hpp>

#include "api_outline.hpp"
#include "board.hpp"
#include "neuralnet.hpp"

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

void check_boards(ai::checkers_board_list_t& expected, const ai::checkers_board_list_t& actual)
{
	for (auto it = expected.begin(); it != expected.end(); ++it) {
		auto location = std::find(actual.begin(), actual.end(), *it);
		REQUIRE( location != actual.end() );
	}
}

TEST_CASE ("Single Piece Moves") {

	SECTION ("Single Red King Left Edge") {
		ai::checkers_board_t red_king =
			"____________R__________________b"; //Position 12: 8, 16
		ai::checkers_board_list_t expected = {
			"________R______________________b", //Result 8
			"________________R______________b"}; //Result 16

		ai::checkers_board_list_t actual = ai::move_generator(red_king, "red");

		REQUIRE( 2 == actual.size() );
		std::sort(expected.begin(), expected.end());
		std::sort(actual.begin(), actual.end());
		for(size_t i=0; i<actual.size(); ++i)
		{
			REQUIRE( getBoard(actual[i]) == getBoard(expected[i]) );
		}
		check_boards(expected, actual);
	}

	SECTION ("Single Red King Right Edge") {
		ai::checkers_board_t red_king =
			"___________R___________________b"; //Position 15: 11, 19
		ai::checkers_board_list_t expected = {
			"_______R_______________________b", //Result 11
			"_______________R_______________b"}; //Result 19

		ai::checkers_board_list_t actual = ai::move_generator(red_king, "red");

		REQUIRE( 2 == actual.size() );
		std::sort(expected.begin(), expected.end());
		std::sort(actual.begin(), actual.end());
		for(size_t i=0; i<actual.size(); ++i)
		{
			std::cout << actual[i] << std::endl;
			REQUIRE( getBoard(actual[i]) == getBoard(expected[i]) );
		}
		check_boards(expected, actual);
	}
}

SCENARIO ("Crowning Red King") {
	ai::checkers_board_t board_state;
	ai::checkers_board_t expected =
		"______________________________Rb";
	ai::checkers_board_list_t actual;

	GIVEN("A pre-crown red pawn state.") {
		board_state = "__________________________r____b";
		WHEN ("Generate possible red moves.") {
			actual = ai::move_generator(board_state, "red");
			THEN ("The red pawn is crowned.") {
				REQUIRE( 1 == actual.size() );
				REQUIRE( expected == actual[0] );
				
			}
		}
	}
}

SCENARIO ("Crowning black King") {
	ai::checkers_board_t board_state;
	ai::checkers_board_t expected =
		"B_____________________________R_";
	ai::checkers_board_list_t actual;

	GIVEN("A pre-crown black pawn state.") {
		board_state = "____b_________________________R_";
		WHEN ("Generate possible black moves.") {
			actual = ai::move_generator(board_state, "black");
			THEN ("The red pawn is crowned.") {
				REQUIRE( 1 == actual.size() );
				REQUIRE( expected == actual.back() );
			}
		}
	}
}

SCENARIO ("Crowning Jumps") {
	ai::checkers_board_t board_state;
	ai::checkers_board_t expected;
	ai::checkers_board_list_t actual;

	GIVEN ("A there are two moves for a pawn to take, then the only aviable move is to jump.") {
		board_state = "_____r__b__________r______b_____";
		WHEN ("black evaluates the board for possible moves") {
			actual = ai::move_generator(board_state, "black");
			std::cout << actual[0] << std::endl;
			THEN ("There should only be the jump which results in a crowned piece.") {
				expected = "_B_________________r_____b_____";
				REQUIRE( 1 == actual.size() );
				REQUIRE( expected == actual.back() );
			}
		}
		AND_WHEN("Red evaluates the board for possible moves") {
			actual = ai::move_generator(board_state, "red");
			THEN ("There there should only be one possible move reported with a crowning jump") {
				expected = "_____r__b______________________R";
				REQUIRE( 1 == actual.size() );
				REQUIRE( expected == actual.back() );
			}
		}
	}
}

TEST_CASE ("Board Validation") {

	ai::checkers_board_t inval_board;
	ai::checkers_board_t valid_board;

	SECTION("Empty Board") {
		inval_board = "";
		REQUIRE( false == ai::is_valid_checkers_board(inval_board) );
	}

	SECTION("Oversized Board") {
		inval_board = "bbbbbbbbbbbb________rrrrrrarrrrr_";
		REQUIRE( false == ai::is_valid_checkers_board(inval_board) );
	}

	SECTION("Invalid Characters") {
		inval_board = "bbbbbbbbbbbb________rrrrrrarrrrr";
		REQUIRE(  false == ai::is_valid_checkers_board(inval_board) );
	}


	SECTION ("Initital Board Validation") {
		valid_board = "rrrrrrrrrrrr________bbbbbbbbbbbb";
		REQUIRE( true == ai::is_valid_checkers_board(valid_board) );

		valid_board = "rrrrrrrr_rrrr_______bbbbbbbbbbbb";
		REQUIRE( true == ai::is_valid_checkers_board(valid_board) );
	}
}

SCENARIO ("Initial Board") {
	ai::checkers_board_list_t actual;

	GIVEN ("Given an initial board state") {
		ai::checkers_board_t initial_board =
			"rrrrrrrrrrrr________bbbbbbbbbbbb";

		WHEN ("The red player evalues for possible moves") {
			actual = ai::move_generator(initial_board, "red");

			THEN ("Each red piece in row 2 should have two moves but the right edge piece") {
				REQUIRE( 7 == actual.size() );

			} AND_THEN ("Each possible red move should be represented.") {
				ai::checkers_board_list_t expected = {
					"rrrrrrrr_rrrr_______bbbbbbbbbbbb",
					"rrrrrrrr_rrr_r______bbbbbbbbbbbb",
					"rrrrrrrrr_rr_r______bbbbbbbbbbbb",
					"rrrrrrrrr_rr__r_____bbbbbbbbbbbb",
					"rrrrrrrrrr_r__r_____bbbbbbbbbbbb",
					"rrrrrrrrrr_r___r____bbbbbbbbbbbb",
					"rrrrrrrrrrr____r____bbbbbbbbbbbb"};
				std::sort(expected.begin(), expected.end());
				std::sort(actual.begin(), actual.end());
				for (size_t ii = 0; ii < actual.size() && ii < expected.size(); ++ii) {
					REQUIRE( actual[ii] == expected[ii] );
				}
			}
		}
		AND_WHEN ("The black player generates moves for an initial board") {
			actual = ai::move_generator(initial_board, "black");

			THEN ("Each black piece in row 5 should have 2 moves but one piece") {
				REQUIRE( 7 == actual.size() );

			} AND_THEN ("Each possible black move should be represented.") {
				ai::checkers_board_list_t expected = {
					"rrrrrrrrrrrr____b____bbbbbbbbbbb",
					"rrrrrrrrrrrr____b___b_bbbbbbbbbb",
					"rrrrrrrrrrrr_____b__b_bbbbbbbbbb",
					"rrrrrrrrrrrr_____b__bb_bbbbbbbbb",
					"rrrrrrrrrrrr______b_bb_bbbbbbbbb",
					"rrrrrrrrrrrr______b_bbb_bbbbbbbb",
					"rrrrrrrrrrrr_______bbbb_bbbbbbbb"};
				std::sort(expected.begin(), expected.end());
				std::sort(actual.begin(), actual.end());
				for (size_t ii = 0; ii < actual.size() && ii < expected.size(); ++ii) {
					REQUIRE( actual[ii] == expected[ii] );
				}
			}
		}
	}
}
