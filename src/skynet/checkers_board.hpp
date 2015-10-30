#ifndef CHECKERS_BOARD_HPP
#define CHECKERS_BOARD_HPP

#include <string>
#include <vector>

namespace skynet
{
	typedef std::string checkers_player_t;
	typedef std::string checkers_board_t;
	typedef std::vector<checkers_board_t> checkers_board_list_t;

	bool is_valid(const checkers_board_t& board);

	checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);
}

#endif
