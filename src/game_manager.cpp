#include "game_manager.hpp"

#include <chrono>
#include <stdexcept>

static uint64_t get_time()
{
	return std::chrono::system_clock::now().time_since_epoch()/
		std::chrono::milliseconds(1);
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

game_list_t game_manager_t::list() const
{
	return games_m;
}

std::string std::to_string(const game_info_t& info)
{
	std::string json;
	json+="\"status\":\""+std::to_string(info.status)+"\",";
	json+="\"moves\":"+std::to_string(info.moves)+",";
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