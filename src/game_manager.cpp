#include "game_manager.hpp"

#include <chrono>
#include <stdexcept>

static uint64_t get_time()
{
	return std::chrono::system_clock::now().time_since_epoch()/
		std::chrono::milliseconds(1);
}

game_manager_t::game_manager_t(const size_t max_game_moves,const size_t game_ttl_mins):
	max_game_moves_m(max_game_moves),game_ttl_mins_m(game_ttl_mins)
{}

game_list_t game_manager_t::list() const
{
	return games_m;
}

void game_manager_t::cleanup_old_games()
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

void game_manager_t::create_game(const std::string& name)
{
	if(name.size()==0||isspace(name[0])!=0||isspace(name[name.size()-1])!=0)
		throw std::runtime_error("Invalid game name \""+name+"\" (cannot start or end with whitespace).");
	if(games_m.count(name)>0)
		throw std::runtime_error("Game \""+name+"\" already exists.");

	uint64_t time=get_time();
	game_info_t game{skynet::RED_TURN,{"rrrrrrrrrrrr________bbbbbbbbbbbb"},time,time};
	games_m[name]=game;
}

void game_manager_t::delete_game(const std::string& name)
{
	if(games_m.count(name)>0)
		games_m.erase(name);
}

game_info_t game_manager_t::info_game(const std::string& name)
{
	if(games_m.count(name)==0)
		throw std::runtime_error("Game \""+name+"\" does not exist.");

	return games_m[name];
}

void game_manager_t::play_game(const std::string& name,const skynet::checkers_board_t& board)
{
	if(games_m.count(name)==0)
		throw std::runtime_error("Game \""+name+"\" does not exist.");

	if(games_m[name].status==skynet::RED_WON||games_m[name].status==skynet::BLACK_WON)
		throw std::runtime_error("Game \""+name+"\" is over.");

	if(games_m[name].boards.size()>max_game_moves_m)
		throw std::runtime_error("Game \""+name+"\" has reached the max number moves (max is "+std::to_string(max_game_moves_m)+").");

	games_m[name].modify_time=get_time();

	skynet::checkers_player_t player=skynet::RED;

	if(games_m[name].status==skynet::BLACK_TURN)
		player=skynet::BLACK;

	skynet::checkers_board_list_t moves=
		skynet::move_generator(games_m[name].boards.back(),player);

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
		games_m[name].status=skynet::RED_WON;

		if(player==skynet::RED)
		{
			winner="black";
			games_m[name].status=skynet::BLACK_WON;
		}

		throw std::runtime_error("Invalid move \""+board+"\", "+winner+" wins.");
	}

	games_m[name].boards.push_back(board);

	if(games_m[name].status==skynet::RED_TURN)
		games_m[name].status=skynet::BLACK_TURN;
	else
		games_m[name].status=skynet::RED_TURN;
}

std::string std::to_string(const game_info_t& info)
{
	std::string json;
	json+="\"status\":\""+std::to_string(info.status)+"\",";
	json+="\"boards\":"+std::to_string(info.boards)+",";
	json+="\"create_time\":"+std::to_string(info.create_time)+",";
	json+="\"modify_time\":"+std::to_string(info.modify_time);
	return "{"+json+"}";
}

std::string std::to_string(const game_list_t& list)
{
	std::string json;

	for(auto ii:list)
		json+="\""+ii.first+"\":"+std::to_string(ii.second)+",";

	if(json.size()>0&&json[json.size()-1]==',')
		json.erase(json.size()-1,1);

	return "{"+json+"}";
}