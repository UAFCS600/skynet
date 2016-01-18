#include <iostream>
#include <skynet/checkers.hpp>
#include <skynet/wget.hpp>
#include <stdexcept>
#include <string>

int main()
{
	try
	{
		std::string server="137.229.25.219";
		std::string game_name="test";
		skynet::checkers_board_t board("rrrrrrrrr_rr_r______bbbbbbbbbbbb");

		std::cout<<"Playing "<<board<<" on server "<<server<<
			" for game "<<game_name<<":"<<std::endl;

		skynet::play_game(server,game_name,board);
		std::cout<<"Done"<<std::endl;
	}
	catch(std::exception& error)
	{
		std::cout<<"Error - "<<error.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Error - Unknown."<<std::endl;
	}

	return 0;
}