#ifndef CHECKERS_BOARD_HPP
#define CHECKERS_BOARD_HPP

#include <string>
#include <vector>

namespace skynet
{
	enum checkers_player_t
	{
		RED,
		BLACK
	};

	checkers_player_t checkers_player_from_string(const std::string& str);

	typedef std::string checkers_board_t;
	typedef std::vector<checkers_board_t> checkers_board_list_t;

	void validate(const checkers_board_t& board);

	checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);
}

namespace std
{
	std::string to_string(const skynet::checkers_player_t& player);
	std::string to_string(const skynet::checkers_board_list_t& boards);
}

#endif
