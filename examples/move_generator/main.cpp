#include <iostream>
#include <skynet/checkers.hpp>
#include <stdexcept>

int main()
{
	try
	{
		skynet::checkers::board_t start_board("rrrrrrrrrrrr________bbbbbbbbbbbb");
		std::cout<<"Generating red moves for board "<<start_board<<":"<<std::endl;

		for(auto board:skynet::checkers::move_generator(start_board,skynet::checkers::RED))
			std::cout<<"  "<<board<<std::endl;
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