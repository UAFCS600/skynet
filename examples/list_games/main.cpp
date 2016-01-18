#include <iostream>
#include <skynet/checkers.hpp>
#include <skynet/wget.hpp>
#include <stdexcept>
#include <string>

void print_game(const std::string& game_name,const skynet::game_info_t& game_info)
{
	std::cout<<"  Name     "<<game_name<<std::endl;
	std::cout<<"  Status   "<<std::to_string(game_info.status)<<std::endl;
	std::cout<<"  Boards   "<<game_info.boards.size()<<std::endl;

	for(auto board:game_info.boards)
		std::cout<<"    "<<board<<std::endl;

	std::cout<<"  Created  "<<game_info.create_time<<std::endl;
	std::cout<<"  Modified "<<game_info.modify_time<<std::endl;
	std::cout<<std::endl;
}

int main()
{
	try
	{
		std::string server="137.229.25.219";
		std::cout<<"Listing games on server "<<server<<":"<<std::endl;

		for(auto game:skynet::list_games(server))
			print_game(game.first,game.second);
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