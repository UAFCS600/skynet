#ifndef CHECKERS_CLIENT_HPP
#define CHECKERS_CLIENT_HPP

#include "checkers.hpp"
#include <cstdint>
#include <map>
#include <string>

namespace skynet
{
	namespace checkers
	{
		struct game_info_t
		{
			skynet::checkers::status_t status;
			skynet::checkers::board_list_t boards;
			uint64_t create_time;
			uint64_t modify_time;
		};

		typedef std::map<std::string,game_info_t> game_list_t;

		game_list_t list_games(const std::string& server);
		game_info_t info_game(const std::string& server,
			const std::string& game_name);
		void play_game(const std::string& server,
			const std::string& game_name,const skynet::checkers::board_t& board);
	}
}

namespace std
{
	std::string to_string(const skynet::checkers::game_list_t& list);
	std::string to_string(const skynet::checkers::game_info_t& info);
}

#endif
