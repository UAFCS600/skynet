#ifndef WGET_HPP
#define WGET_HPP

#include "checkers.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace skynet
{
	struct game_info_t
	{
		skynet::checkers_status_t status;
		skynet::checkers_board_list_t boards;
		uint64_t create_time;
		uint64_t modify_time;
	};

	typedef std::map<std::string,skynet::game_info_t> game_list_t;

	skynet::game_list_t list_games(const std::string& server);
	skynet::game_info_t info_game(const std::string& server,
		const std::string& game_name);
	void play_game(const std::string& server,
		const std::string& game_name,const skynet::checkers_board_t& board);
}

namespace std
{
	std::string to_string(const skynet::game_list_t& list);
	std::string to_string(const skynet::game_info_t& info);
}

#endif