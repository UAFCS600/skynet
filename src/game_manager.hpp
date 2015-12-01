#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <cstdint>
#include <map>

#include "skynet/checkers.hpp"

struct game_info_t
{
	skynet::checkers_status_t status;
	skynet::checkers_board_list_t boards;
	uint64_t create_time;
	uint64_t modify_time;
};

typedef std::map<std::string,game_info_t> game_list_t;

class game_manager_t
{
	public:
		game_manager_t(const size_t max_game_moves=100,const size_t game_ttl_mins=10,
			const size_t game_timeout_secs=30,const size_t max_name_size=10);
		game_list_t list() const;
		void timeout_games();
		void cleanup_old_games();
		void create_game(const std::string& name);
		void timeout_game(const std::string& name);
		void delete_game(const std::string& name);
		game_info_t info_game(const std::string& name);
		void play_game(const std::string& name,const skynet::checkers_board_t& board);

	private:
		size_t max_game_moves_m;
		size_t game_ttl_mins_m;
		size_t game_timeout_secs_m;
		size_t max_name_size_m;
		game_list_t games_m;
};

namespace std
{
	std::string to_string(const game_info_t& info);
	std::string to_string(const game_list_t& list);
}

#endif