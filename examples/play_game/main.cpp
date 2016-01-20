#include <iostream>
#include <skynet/checkers.hpp>
#include <skynet/checkers_client.hpp>
#include <stdexcept>
#include <string>

int main()
{
	try
	{
		std::string server="skynet.cs.uaf.edu";
		std::string game_name="test";
		skynet::checkers::board_t board("rrrrrrrrr_rr_r______bbbbbbbbbbbb");

		std::cout<<"Playing "<<board<<" on server "<<server<<" for game "<<game_name<<":"<<std::endl;
		skynet::checkers::play_game(server,game_name,board);
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