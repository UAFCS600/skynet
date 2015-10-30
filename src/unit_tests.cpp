#include <algorithm>
#include <fstream>
#include <vector>

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include "skynet/checkers_board.hpp"
#include "skynet/neuralnet.hpp"

//Testing Utility Function
//    Absolute checks if the expected boards are the exact set of all boards expected...
//    not just a subset of the expected boards...
void test_checkers_board(const skynet::checkers_board_t& board,const skynet::checkers_player_t& player,
	const bool absolute,skynet::checkers_board_list_t expected_boards)
{
	REQUIRE(skynet::is_valid(board));

	REQUIRE((player=="red"||player=="black"));

	auto out_boards=skynet::move_generator(board,player);
	std::sort(out_boards.begin(),out_boards.end());

	std::sort(expected_boards.begin(),expected_boards.end());

	size_t found=0;

	for(auto expected_board:expected_boards)
	{
		REQUIRE(skynet::is_valid(expected_board));

		for(auto out_board:out_boards)
		{
			if(out_board==expected_board)
			{
				++found;
				break;
			}
		}
	}

	REQUIRE(found==expected_boards.size());

	if(absolute)
		REQUIRE(out_boards.size()==expected_boards.size());
}

//Testing Utility Function
//    Reads file for tests, format is:
//    test_board_0
//    side_to_test_0
//    contains|absolute
//    expected_board_0
//    expected_board_1
//    expected_board_2
//    ...
//    \n
//    test_board_1
//    side_to_test_1
//    contains|absolute
//    expected_board_0
//    expected_board_1
//    expected_board_2
//    ...
//    \n
void test_checkers_board_from_file(const std::string& filename)
{
	std::ifstream istr(filename);
	bool done=false;
	std::string temp;

	while(!done)
	{
		temp="";

		while(true)
		{
			if(!std::getline(istr,temp)||temp.size()!=0)
				break;
		}

		skynet::checkers_board_t board(temp);

		if(!skynet::is_valid(temp))
			throw std::runtime_error("Invalid test board \""+temp+"\".");

		if(!std::getline(istr,temp))
			break;

		skynet::checkers_player_t player(temp);

		if(player!="red"&&player!="black")
			throw std::runtime_error("Invalid player value \""+temp+"\" (expected \"red\" or \"black\").");

		if(!std::getline(istr,temp))
			break;

		if(temp!="absolute"&&temp!="contains")
			throw std::runtime_error("Invalid absolute value \""+temp+"\" (expected \"absolute\" or \"contains\").");

		bool absolute(temp=="absolute");

		skynet::checkers_board_list_t expected;

		while(true)
		{
			if(!std::getline(istr,temp))
			{
				temp="";
				done=true;
			}


			if(temp.size()==0)
			{
				test_checkers_board(board,player,absolute,expected);
				break;
			}

			expected.push_back(temp);
		}
	}
}

TEST_CASE("Neural Network Construction")
{
	std::vector<size_t> topography{32,40,10,1};
	std::vector<double> weights(1690,1);

	std::vector<size_t> invalid_topography{};
	std::vector<double> invalid_weights(0.0,0);

	SECTION("Construction Exceptions")
	{
		REQUIRE_NOTHROW(skynet::neuralnet_t neuralnet(topography,weights));
		REQUIRE_THROWS(skynet::neuralnet_t neuralnet(topography,invalid_weights));
		REQUIRE_THROWS(skynet::neuralnet_t neuralnet(invalid_topography,weights));
		REQUIRE_THROWS(skynet::neuralnet_t neuralnet(invalid_topography,invalid_weights));
	}

	SECTION("Evaluation")
	{
		skynet::neuralnet_t neuralnet(topography,weights);
		const std::vector<double> zeros(32,0);
		const std::vector<double> ones(32,1);

		REQUIRE(neuralnet.evaluate(zeros,0)==0);
		REQUIRE(neuralnet.evaluate(ones,0)==12800);
	}
}

TEST_CASE("Board Validation")
{

	SECTION("Empty Board")
	{
		REQUIRE(!skynet::is_valid(""));
	}

	SECTION("Oversized Board")
	{
		REQUIRE(!skynet::is_valid("bbbbbbbbbbbb________rrrrrrarrrrr_"));
	}

	SECTION("Invalid Characters")
	{
		REQUIRE(!skynet::is_valid("bbbbbbbbbbbb________rrrrrrarrrrr"));
	}


	SECTION("Initital Board Validation")
	{
		REQUIRE(skynet::is_valid("rrrrrrrrrrrr________bbbbbbbbbbbb"));
		REQUIRE(skynet::is_valid("rrrrrrrr_rrrr_______bbbbbbbbbbbb"));
	}
}

TEST_CASE("Single Piece Moves")
{
	SECTION("Single Red King Left Edge")
	{
		skynet::checkers_board_t board("____________R__________________b");

		skynet::checkers_board_list_t expected
		{
			"________R______________________b",
			"________________R______________b"
		};

		test_checkers_board(board,"red",true,expected);
	}

	SECTION("Single Red King Right Edge")
	{
		skynet::checkers_board_t board("___________R___________________b");

		skynet::checkers_board_list_t expected
		{
			"_______R_______________________b",
			"_______________R_______________b"
		};

		test_checkers_board(board,"red",true,expected);
	}
}


SCENARIO("Crowning Red King")
{
	skynet::checkers_board_t board=("__________________________r____b");

	skynet::checkers_board_list_t expected
	{
		"______________________________Rb"
	};

	GIVEN("A pre-crown red pawn state.")
	{
		WHEN("Generate possible red moves.")
		{
			THEN("The red pawn is crowned.")
			{
				test_checkers_board(board,"red",true,expected);
			}
		}
	}
}

SCENARIO("Crowning Black King")
{
	skynet::checkers_board_t board("____b_________________________R_");

	skynet::checkers_board_list_t expected
	{
		"B_____________________________R_"
	};

	GIVEN("A pre-crown black pawn state.")
	{
		WHEN("Generate possible black moves.")
		{
			THEN("The black pawn is crowned.")
			{
				test_checkers_board(board,"black",true,expected);
			}
		}
	}
}


SCENARIO("Crowning Jumps")
{
	skynet::checkers_board_t board("_____r__b__________r______b_____");

	GIVEN("A there are two moves for a pawn to take,then the only aviable move is to jump.")
	{
		WHEN("black evaluates the board for possible moves")
		{
			THEN("There should only be the jump which results in a crowned piece.")
			{
				skynet::checkers_board_list_t expected
				{
					"_B_________________r______b_____"
				};

				test_checkers_board(board,"black",true,expected);
			}
		}
		AND_WHEN("Red evaluates the board for possible moves")
		{
			THEN("There there should only be one possible move reported with a crowning jump")
			{
				skynet::checkers_board_list_t expected
				{
					"____________r______r______b_____"
				};

				test_checkers_board(board,"red",true,expected);
			}
		}
	}
}

SCENARIO("Initial Board")
{
	skynet::checkers_board_list_t actual;

	GIVEN("Given an initial board state")
	{
		skynet::checkers_board_t board("rrrrrrrrrrrr________bbbbbbbbbbbb");

		WHEN("The red player evalues for possible moves")
		{
			THEN("Each possible red move should be represented.")
			{
				skynet::checkers_board_list_t expected
				{
					"rrrrrrrr_rrrr_______bbbbbbbbbbbb",
					"rrrrrrrr_rrr_r______bbbbbbbbbbbb",
					"rrrrrrrrr_rr_r______bbbbbbbbbbbb",
					"rrrrrrrrr_rr__r_____bbbbbbbbbbbb",
					"rrrrrrrrrr_r__r_____bbbbbbbbbbbb",
					"rrrrrrrrrr_r___r____bbbbbbbbbbbb",
					"rrrrrrrrrrr____r____bbbbbbbbbbbb"

				};

				test_checkers_board(board,"red",true,expected);
			}
		}
		AND_WHEN("The black player generates moves for an initial board")
		{
			THEN("Each possible black move should be represented.")
			{
				skynet::checkers_board_list_t expected
				{
					"rrrrrrrrrrrr____b____bbbbbbbbbbb",
					"rrrrrrrrrrrr____b___b_bbbbbbbbbb",
					"rrrrrrrrrrrr_____b__b_bbbbbbbbbb",
					"rrrrrrrrrrrr_____b__bb_bbbbbbbbb",
					"rrrrrrrrrrrr______b_bb_bbbbbbbbb",
					"rrrrrrrrrrrr______b_bbb_bbbbbbbb",
					"rrrrrrrrrrrr_______bbbb_bbbbbbbb"
				};

				test_checkers_board(board,"black",true,expected);
			}
		}
	}
}

SCENARIO("Blondie24 Boards")
{
	test_checkers_board_from_file("tests/blondie24.txt");
}