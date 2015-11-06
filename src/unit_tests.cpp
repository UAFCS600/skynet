#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "skynet/checkers_board.hpp"
#include "skynet/neuralnet.hpp"

//Uncomment line below to see passing tests.
//#define SHOW_PASSES

bool failed=false;

void test_neural_network_construction()
{
	std::cout<<"Neural Network Construction"<<std::endl;
	std::vector<size_t> topography{32,40,10,1};
	std::vector<double> weights(1690,1);
	std::vector<size_t> invalid_topography{};
	std::vector<double> invalid_weights(0.0,0);

	{
		std::cout<<"  Construction Exceptions"<<std::endl;

		try
		{
			skynet::neuralnet_t neuralnet(topography,weights);
		}
		catch(std::exception& error)
		{
			std::cout<<"    FAIL NN with Blondie24 topography and valid weights ("<<error.what()<<")."<<std::endl;
			failed=true;
		}
		catch(...)
		{
			std::cout<<"    FAIL NN with Blondie24 topography and valid weights (unknown error)."<<std::endl;
			failed=true;
		}

		#ifdef SHOW_PASSES
			std::cout<<"    PASS NN with Blondie24 topography and valid weights."<<std::endl;
		#endif

		try
		{
			skynet::neuralnet_t neuralnet(topography,invalid_weights);
			std::cout<<"    FAIL NN with Blondie24 topography and invalid weights."<<std::endl;
			failed=true;
		}
		catch(...)
		{
			#ifdef SHOW_PASSES
				std::cout<<"    PASS NN with Blondie24 topography and invalid weights."<<std::endl;
			#endif
		}

		try
		{
			skynet::neuralnet_t neuralnet(invalid_topography,weights);
			std::cout<<"    FAIL NN with invalid topography and valid weights."<<std::endl;
			failed=true;
		}
		catch(...)
		{
			#ifdef SHOW_PASSES
				std::cout<<"    PASS NN with invalid topography and valid weights."<<std::endl;
			#endif
		}

		try
		{
			skynet::neuralnet_t neuralnet(invalid_topography,invalid_weights);
			std::cout<<"    FAIL NN with invalid topography and invalid weights."<<std::endl;
			failed=true;
		}
		catch(...)
		{
			#ifdef SHOW_PASSES
				std::cout<<"    PASS NN with invalid topography and invalid weights."<<std::endl;
			#endif
		}
	}

	{
		std::cout<<"  Evaluation"<<std::endl;
		skynet::neuralnet_t neuralnet(topography,weights);
		const std::vector<double> zeros(32,0);
		const std::vector<double> ones(32,1);

		{
			try
			{
				double expected=0;
				double output=neuralnet.evaluate(zeros,0);

				if(output!=expected)
					throw std::runtime_error("Expected output "+std::to_string(expected)+" got "+std::to_string(output)+".");

				#ifdef SHOW_PASSES
					std::cout<<"    PASS NN with Blondie24 topography and weights of zeros."<<std::endl;
				#endif
			}
			catch(std::exception& error)
			{
				std::cout<<"    FAIL NN with Blondie24 topography and weights of zeros ("<<error.what()<<")."<<std::endl;
				failed=true;
			}
			catch(...)
			{
				std::cout<<"    FAIL NN with Blondie24 topography and weights of zeros (unknown error)."<<std::endl;
				failed=true;
			}
		}

		{
			try
			{
				double expected=12800;
				double output=neuralnet.evaluate(ones,0);

				if(output!=expected)
					throw std::runtime_error("Expected output "+std::to_string(expected)+" got "+std::to_string(output)+".");

				#ifdef SHOW_PASSES
					std::cout<<"    PASS NN with Blondie24 topography and weights of ones."<<std::endl;
				#endif
			}
			catch(std::exception& error)
			{
				std::cout<<"    FAIL NN with Blondie24 topography and weights of ones ("<<error.what()<<")."<<std::endl;
				failed=true;
			}
			catch(...)
			{
				std::cout<<"    FAIL NN with Blondie24 topography and weights of ones (unknown error)."<<std::endl;
				failed=true;
			}
		}
	}
}

void test_checkers_is_valid(const std::string& message,const skynet::checkers_board_t& board,const bool should_fail)
{
	try
	{
		if(skynet::is_valid(board))
		{
			if(!should_fail)
			{
				#ifdef SHOW_PASSES
					std::cout<<"    PASS "<<message<<" (\""<<board<<"\")."<<std::endl;
				#endif
			}
			else
			{
				throw std::runtime_error("expected true got false");
			}
		}
		else
		{
			if(should_fail)
			{
				#ifdef SHOW_PASSES
					std::cout<<"    PASS "<<message<<" (\""<<board<<"\")."<<std::endl;
				#endif
			}
			else
			{
				throw std::runtime_error("expected false got true");
			}
		}
	}
	catch(std::exception& error)
	{
		std::cout<<"    FAIL "<<message<<" (\""<<board<<"\") error ("<<error.what()<<")."<<std::endl;
		failed=true;
	}
	catch(...)
	{
		std::cout<<"    FAIL "<<message<<" (\""<<board<<"\") error (unknown error)."<<std::endl;
		failed=true;
	}
}

void test_checkers_board_validation()
{
	std::cout<<"  Board Validation"<<std::endl;
	test_checkers_is_valid("Empty string","",true);

	test_checkers_is_valid("Long board","bbbbbbbbbbbb________rrrrrrrrrrrr_",true);
	test_checkers_is_valid("Invalid characters","bbbbbbbbbbbb________rrrrrrarrrrr",true);

	test_checkers_is_valid("Initial board","rrrrrrrrrrrr________bbbbbbbbbbbb",false);
	test_checkers_is_valid("Initial red move","rrrrrrrr_rrrr_______bbbbbbbbbbbb",false);
}

void test_checkers_move(const std::string& message,const skynet::checkers_board_t& board,const skynet::checkers_player_t& player,
	const bool absolute,skynet::checkers_board_list_t expected_boards,bool should_fail)
{
	bool passed=true;
	std::string error_message="";

	try
	{
		if(!skynet::is_valid(board))
			throw std::runtime_error("Input board is invalid");

		if(player!="red"&&player!="black")
			throw std::runtime_error("Input player is invalid");

		auto out_boards=skynet::move_generator(board,player);
		std::sort(out_boards.begin(),out_boards.end());

		std::sort(expected_boards.begin(),expected_boards.end());

		size_t found_count=0;

		for(size_t ii=0;ii<expected_boards.size();++ii)
		{
			bool found=false;

			if(!skynet::is_valid(expected_boards[ii]))
				throw std::runtime_error("Expected board ["+std::to_string(ii)+"](\""+expected_boards[ii]+"\") is invalid");

			for(size_t jj=0;jj<out_boards.size();++jj)
			{
				if(!skynet::is_valid(out_boards[jj]))
					throw std::runtime_error("Out board ["+std::to_string(jj)+"](\""+out_boards[jj]+"\") is invalid");

				if(out_boards[jj]==expected_boards[ii])
				{
					found=true;
					++found_count;
					break;
				}
			}

			if(!found)
				throw std::runtime_error("Expected board ["+std::to_string(ii)+"](\""+expected_boards[ii]+"\") not found");
		}

		if(out_boards.size()!=expected_boards.size()&&absolute)
			throw std::runtime_error("Expected "+std::to_string(expected_boards.size())+" out board(s) got "+std::to_string(out_boards.size()));

		if(found_count<expected_boards.size())
			throw std::runtime_error("Expected at least "+std::to_string(expected_boards.size())+" out board(s) got "+std::to_string(found_count));
	}
	catch(std::exception& error)
	{
		passed=should_fail;

		if(!passed)
			error_message=error.what();
	}
	catch(...)
	{
		passed=should_fail;

		if(!passed)
			error_message="unknown";
	}

	if(passed)
	{
		#ifdef SHOW_PASSES
			std::cout<<"    PASS "<<std::flush;
		#endif
	}
	else
	{
		std::cout<<"    FAIL "<<std::flush;
		failed=true;
	}

	#ifndef SHOW_PASSES
		if(!passed)
	#endif
	{
		std::cout<<message<<" (\""<<board<<"\")."<<std::endl;
	}

	if(!passed)
		std::cout<<"         "<<error_message<<"."<<std::endl;
}

void test_checkers_move_singles()
{
	std::cout<<"  Single Piece Moves"<<std::endl;

	{
		skynet::checkers_board_t board("____________R__________________b");

		skynet::checkers_board_list_t expected
		{
			"________R______________________b",
			"________________R______________b"
		};

		test_checkers_move("Single red king left edge",board,"red",true,expected,false);
	}

	{
		skynet::checkers_board_t board("___________R___________________b");

		skynet::checkers_board_list_t expected
		{
			"_______R_______________________b",
			"_______________R_______________b"
		};

		test_checkers_move("Single red king right edge",board,"red",true,expected,false);
	}
}

void test_checkers_crowns()
{
	std::cout<<"  Crowns"<<std::endl;
	{
		skynet::checkers_board_t board=("__________________________r____b");

		skynet::checkers_board_list_t expected
		{
			"______________________________Rb"
		};

		test_checkers_move("Given pre-crown red pawn state, then the red pawn is crowned",board,"red",true,expected,false);
	}

	{
		skynet::checkers_board_t board("____b_________________________R_");

		skynet::checkers_board_list_t expected
		{
			"B_____________________________R_"
		};

		test_checkers_move("Given pre-crown black pawn state, then the black pawn is crowned",board,"black",true,expected,false);
	}
}

void test_checkers_forced_jumps()
{
	std::cout<<"  Forced Jumps"<<std::endl;
	{
		skynet::checkers_board_t board("_____r__b__________r______b_____");

		{
			skynet::checkers_board_list_t expected
			{
				"_B_________________r______b_____"
			};

			test_checkers_move("Jump must be taken by black pawn and black pawn is crowned",board,"black",true,expected,false);
		}

		{
			skynet::checkers_board_list_t expected
			{
				"____________r______r______b_____"
			};

			test_checkers_move("Jump must be taken by red pawn and red pawn is crowned",board,"red",true,expected,false);
		}
	}
}

void test_checkers_opening_moves()
{
	std::cout<<"  Opening Moves"<<std::endl;
	skynet::checkers_board_list_t actual;

	{
		skynet::checkers_board_t board("rrrrrrrrrrrr________bbbbbbbbbbbb");
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

			test_checkers_move("Red opening moves.",board,"red",true,expected,false);
		}

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

			test_checkers_move("Black opening moves.",board,"black",true,expected,false);
		}
	}
}

//Testing Utility Function
//    Reads file for tests, format is:
//    test_board_0
//    should_pass|should_fail
//    side_to_test_0
//    contains|absolute
//    expected_board_0
//    expected_board_1
//    expected_board_2
//    ...
//    \n
//    test_board_1
//    should_pass|should_fail
//    side_to_test_1
//    contains|absolute
//    expected_board_0
//    expected_board_1
//    expected_board_2
//    ...
//    \n
void test_checkers_move_file(const std::string& title,const std::string& filename)
{
	std::cout<<"  "<<title<<std::endl;

	try
	{
		std::ifstream istr(filename);
		bool done=false;
		std::string temp;

		while(!done)
		{
			temp="";

			while(true)
			{
				if(!std::getline(istr,temp))
				{
					if(istr.eof())
						done=true;

					break;
				}

				if(temp.size()!=0)
					break;
			}

			if(done)
				break;

			skynet::checkers_board_t board(temp);

			if(!skynet::is_valid(temp))
				throw std::runtime_error("invalid test board \""+temp+"\".");

			bool should_fail=false;

			if(!std::getline(istr,temp))
				break;

			if(temp=="should_pass")
				should_fail=false;
			else if(temp=="should_fail")
				should_fail=true;
			else
				throw std::runtime_error("Invalid pass/fail value \""+temp+"\" (expected \"should_pass\" or \"should_fail\").");

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
					test_checkers_move("",board,player,absolute,expected,should_fail);
					break;
				}

				expected.push_back(temp);
			}
		}
	}
	catch(std::exception& error)
	{
	}
	catch(...)
	{
	}
}

void test_checkers_file_moves()
{
	test_checkers_move_file("Blondie24 Boards","tests/blondie24.txt");
	test_checkers_move_file("Blondie24 vs Chinook","tests/blondie24_vs_chinook.txt");
	test_checkers_move_file("Blondie24 vs Human 1926","tests/blondie24_vs_human1926.txt");
	test_checkers_move_file("Blondie24 vs Human 2134","tests/blondie24_vs_human2134.txt");
	test_checkers_move_file("Blondie24 vs Human 1771","tests/blondie24_vs_human1771.txt");
	test_checkers_move_file("Sam Boards","tests/sam_program.txt");
}

void test_checkers_move_generator()
{
	std::cout<<"Checkers Move Generator"<<std::endl;
	test_checkers_move_singles();
	test_checkers_crowns();
	test_checkers_forced_jumps();
	test_checkers_opening_moves();
	test_checkers_file_moves();
}

int main()
{
	test_neural_network_construction();
	test_checkers_board_validation();
	test_checkers_move_generator();

	if(failed)
		std::cout<<"THERE ARE FAILING TESTS"<<std::endl;
	else
		std::cout<<"ALL TESTS PASSED"<<std::endl;

	return 0;
}