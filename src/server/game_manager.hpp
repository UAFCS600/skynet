#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include <skynet/checkers.hpp>
#include <skynet/checkers_client.hpp>

class game_manager_t
{
	public:
		game_manager_t(const skynet::checkers::board_list_t& opening_moves={},
			const size_t max_game_moves=200,const size_t game_ttl_mins=10,
			const size_t game_timeout_secs=60,const size_t max_name_size=10);
		skynet::checkers::game_list_t list() const;
		void timeout_games();
		void cleanup_old_games();
		void create_game(const std::string& name);
		void timeout_game(const std::string& name);
		void delete_game(const std::string& name);
		skynet::checkers::game_info_t info_game(const std::string& name);
		void play_game(const std::string& name,const skynet::checkers::board_t& board);

	private:
		skynet::checkers::board_list_t opening_moves_m;
		size_t max_game_moves_m;
		size_t game_ttl_mins_m;
		size_t game_timeout_secs_m;
		size_t max_name_size_m;
		skynet::checkers::game_list_t games_m;
};

#endif
