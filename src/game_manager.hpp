#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "skynet/checkers.hpp"
#include "skynet/wget.hpp"

class game_manager_t
{
	public:
		game_manager_t(const skynet::checkers_board_list_t& opening_moves={},
			const size_t max_game_moves=100,const size_t game_ttl_mins=10,
			const size_t game_timeout_secs=30,const size_t max_name_size=10);
		skynet::game_list_t list() const;
		void timeout_games();
		void cleanup_old_games();
		void create_game(const std::string& name);
		void timeout_game(const std::string& name);
		void delete_game(const std::string& name);
		skynet::game_info_t info_game(const std::string& name);
		void play_game(const std::string& name,const skynet::checkers_board_t& board);

	private:
		skynet::checkers_board_list_t opening_moves_m;
		size_t max_game_moves_m;
		size_t game_ttl_mins_m;
		size_t game_timeout_secs_m;
		size_t max_name_size_m;
		skynet::game_list_t games_m;
};

#endif