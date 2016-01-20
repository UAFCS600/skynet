#include <ctime>
#include <iostream>
#include <skynet/checkers.hpp>
#include <skynet/checkers_client.hpp>
#include <stdexcept>
#include <string>

void print_time(const uint64_t milliseconds)
{
	time_t time=milliseconds/1000;
	std::cout<<std::asctime(std::localtime(&time))<<std::flush;
}

void print_game(const std::string& game_name,const skynet::checkers::game_info_t& game_info)
{
	std::cout<<"  Name     "<<game_name<<std::endl;
	std::cout<<"  Status   "<<std::to_string(game_info.status)<<std::endl;
	std::cout<<"  Boards   "<<game_info.boards.size()<<std::endl;

	for(auto board:game_info.boards)
		std::cout<<"    "<<board<<std::endl;

	std::cout<<"  Created  "<<std::flush;
	print_time(game_info.create_time);
	std::cout<<"  Modified "<<std::flush;
	print_time(game_info.modify_time);
}

int main()
{
	try
	{
		std::string server="skynet.cs.uaf.edu";
		std::string game_name="test";
		std::cout<<"Listing game info on server "<<server<<" for game "<<game_name<<":"<<std::endl;
		print_game(game_name,skynet::checkers::info_game(server,game_name));
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