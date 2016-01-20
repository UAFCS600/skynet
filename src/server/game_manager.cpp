#include "game_manager.hpp"

#include <chrono>
#include <stdexcept>
#include <random>

static inline uint64_t get_time()
{
	return std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
}

static inline size_t random_number(const size_t start,const size_t range)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_real_distribution<double> dist(start,range);
	return dist(mt);
}

game_manager_t::game_manager_t(const skynet::checkers::board_list_t& opening_moves,
	const size_t max_game_moves,const size_t game_ttl_mins,const size_t game_timeout_secs,
	const size_t max_name_size):
	opening_moves_m(opening_moves),
	max_game_moves_m(max_game_moves),game_ttl_mins_m(game_ttl_mins),
	game_timeout_secs_m(game_timeout_secs),max_name_size_m(max_name_size)
{}

skynet::checkers::game_list_t game_manager_t::list() const
{
	return games_m;
}

void game_manager_t::timeout_games()
{
	if(game_timeout_secs_m>0)
	{
		std::vector<std::string> games_to_timeout;

		uint64_t time=get_time();
		uint64_t one_sec=1000;

		for(auto ii:games_m)
			if(ii.second.status!=skynet::checkers::RED_WON&&ii.second.status!=skynet::checkers::BLACK_WON&&
				ii.second.create_time!=ii.second.modify_time&&time-ii.second.modify_time>=one_sec*game_timeout_secs_m)
					games_to_timeout.push_back(ii.first);

		for(auto name:games_to_timeout)
			timeout_game(name);
	}
}

void game_manager_t::cleanup_old_games()
{
	if(game_ttl_mins_m>0)
	{
		std::vector<std::string> games_to_delete;

		uint64_t time=get_time();
		uint64_t one_min=60000;

		for(auto ii:games_m)
			if(time-ii.second.modify_time>=one_min*game_ttl_mins_m)
				games_to_delete.push_back(ii.first);

		for(auto name:games_to_delete)
			delete_game(name);
	}
}

void game_manager_t::create_game(const std::string& name)
{
	if(name.size()>max_name_size_m)
		throw std::runtime_error("Invalid game name \""+name+"\" (name cannot be greater than "+std::to_string(max_name_size_m)+").");
	if(name.size()==0||isspace(name[0])!=0||isspace(name[name.size()-1])!=0)
		throw std::runtime_error("Invalid game name \""+name+"\" (cannot start or end with whitespace).");
	if(games_m.count(name)>0)
		throw std::runtime_error("Game \""+name+"\" already exists.");

	uint64_t time=get_time();

	skynet::checkers::board_t starting_board("rrrrrrrrrrrr________bbbbbbbbbbbb");

	if(opening_moves_m.size()>0)
		starting_board=opening_moves_m[random_number(0,opening_moves_m.size())];

	skynet::checkers::game_info_t game{skynet::checkers::RED_TURN,{starting_board},time,time};
	games_m[name]=game;
}

void game_manager_t::timeout_game(const std::string& name)
{
	if(games_m.count(name)>0)
	{
		if(games_m[name].status==skynet::checkers::RED_TURN)
			games_m[name].status=skynet::checkers::BLACK_WON;
		else
			games_m[name].status=skynet::checkers::RED_WON;
	}
}


void game_manager_t::delete_game(const std::string& name)
{
	if(games_m.count(name)>0)
		games_m.erase(name);
}

skynet::checkers::game_info_t game_manager_t::info_game(const std::string& name)
{
	if(games_m.count(name)==0)
		throw std::runtime_error("Game \""+name+"\" does not exist.");

	return games_m[name];
}

void game_manager_t::play_game(const std::string& name,const skynet::checkers::board_t& board)
{
	if(games_m.count(name)==0)
		throw std::runtime_error("Game \""+name+"\" does not exist.");

	if(games_m[name].status==skynet::checkers::RED_WON||games_m[name].status==skynet::checkers::BLACK_WON)
		throw std::runtime_error("Game \""+name+"\" is over.");

	if(games_m[name].boards.size()>max_game_moves_m)
		throw std::runtime_error("Game \""+name+"\" has reached the max number moves (max is "+std::to_string(max_game_moves_m)+").");

	games_m[name].modify_time=get_time();

	skynet::checkers::player_t player=skynet::checkers::RED;

	if(games_m[name].status==skynet::checkers::BLACK_TURN)
		player=skynet::checkers::BLACK;

	skynet::checkers::board_list_t moves=skynet::checkers::move_generator(games_m[name].boards.back(),player);

	bool found=false;

	for(auto move:moves)
	{
		if(board==move)
		{
			found=true;
			break;
		}
	}

	if(!found)
	{
		std::string winner="red";
		games_m[name].status=skynet::checkers::RED_WON;

		if(player==skynet::checkers::RED)
		{
			winner="black";
			games_m[name].status=skynet::checkers::BLACK_WON;
		}

		throw std::runtime_error("Invalid move \""+board+"\", "+winner+" wins.");
	}

	games_m[name].boards.push_back(board);

	if(games_m[name].status==skynet::checkers::RED_TURN)
		games_m[name].status=skynet::checkers::BLACK_TURN;
	else
		games_m[name].status=skynet::checkers::RED_TURN;
}
