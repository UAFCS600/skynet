#include "wget.hpp"

#include "../json.hpp"
#include <mongoose/mongoose.h>
#include <stdexcept>

struct wget_t
{
	std::string data;
	std::string error;
	bool done;
};

static inline void wget_ev_handler(mg_connection* connection,int ev,void* ev_data)
{
	wget_t& responder=*(wget_t*)(connection->mgr->user_data);

	if(ev==MG_EV_CONNECT)
	{
		int status=*(int*)ev_data;

		if(status!=0)
		{
			responder.error=strerror(status);
			responder.done=true;
			return;
		}
	}
	else if(ev==MG_EV_HTTP_REPLY)
	{
		connection->flags|=MG_F_CLOSE_IMMEDIATELY;
		http_message* hm=(http_message*)ev_data;

		if(hm->resp_code!=200)
		{
			responder.error="Connection error: "+std::to_string(hm->resp_code)+".";
			responder.done=true;
			return;
		}

		responder.data=std::string(hm->body.p,hm->body.len);
		responder.done=true;
	}
}

static inline std::string wget(const std::string& address,const std::string& post_data)
{
	wget_t responder;
	responder.data="";
	responder.error="";
	responder.done=false;

	mg_mgr mgr;
	mg_mgr_init(&mgr,&responder);
	mg_connect_http(&mgr,wget_ev_handler,address.c_str(),nullptr,post_data.c_str());

	while(mgr.active_connections!=nullptr)
		mg_mgr_poll(&mgr,1000);

	mg_mgr_free(&mgr);

	if(responder.error.size()>0)
		throw std::runtime_error(responder.error);

	return responder.data;
}
#include <iostream>
static inline skynet::game_info_t json_to_game_info(const json_t& json)
{
	if(!json.isObject())
		throw std::runtime_error("Game info is not a JSON object.");

	skynet::game_info_t game;
	game.status=skynet::checkers_status_from_string(json["status"].asString());

	for(auto board:json["boards"])
	{
		skynet::validate(board.asString());
		game.boards.push_back(board.asString());
	}

	game.create_time=json["create_time"].asUInt64();
	game.modify_time=json["modify_time"].asUInt64();

	return game;
}

skynet::game_list_t skynet::list_games(const std::string& server)
{
	std::string data=wget(server+"/?list_games=true","");

	json_t json=JSON_parse(data);

	if(!json.isObject())
		throw std::runtime_error("Game list is not a JSON object.");

	if(json.isMember("error"))
		throw std::runtime_error(json["error"].asString());

	skynet::game_list_t games;

	for(Json::ValueIterator key=json.begin();key!=json.end();++key)
		games[key.key().asString()]=json_to_game_info(*key);

	return games;
}

skynet::game_info_t skynet::info_game(const std::string& server,
	const std::string& game_name)
{
	std::string data=wget(server+"/?info_game=true",
		"{\"name\":\""+game_name+"\"}");

	json_t json=JSON_parse(data);

	if(!json.isObject())
		throw std::runtime_error("Game info is not a JSON object.");

	if(json.isMember("error"))
		throw std::runtime_error(json["error"].asString());

	return json_to_game_info(json);
}

void skynet::play_game(const std::string& server,
		const std::string& game_name,const skynet::checkers_board_t& board)
{
	std::string data=wget(server+"/?play_game=true",
		"{\"name\":\""+game_name+"\",\"board\":\""+board+"\"}");

	json_t json=JSON_parse(data);

	if(!json.isObject())
		throw std::runtime_error("Game play is not a JSON object.");

	if(json.isMember("error"))
		throw std::runtime_error(json["error"].asString());
}

std::string std::to_string(const skynet::game_info_t& info)
{
	std::string json;
	json+="\"status\":\""+std::to_string(info.status)+"\",";
	json+="\"boards\":"+std::to_string(info.boards)+",";
	json+="\"create_time\":"+std::to_string(info.create_time)+",";
	json+="\"modify_time\":"+std::to_string(info.modify_time);
	return "{"+json+"}";
}

std::string std::to_string(const skynet::game_list_t& list)
{
	std::string json;

	for(auto ii:list)
		json+="\""+ii.first+"\":"+std::to_string(ii.second)+",";

	if(json.size()>0&&json[json.size()-1]==',')
		json.erase(json.size()-1,1);

	return "{"+json+"}";
}