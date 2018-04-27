#define NS_ENABLE_SSL
#include <iostream>
#include <memory>
#include <mongoose/mongoose.h>
#include <stdexcept>
#include <string>

#include "game_manager.hpp"
#include "handler.hpp"

skynet::checkers::board_list_t opening_moves{};
size_t max_game_moves=200;
size_t game_ttl_mins=10;
size_t game_timeout_secs=60;

game_manager_t global_game_manager(opening_moves,max_game_moves,
	game_ttl_mins,game_timeout_secs);

int main(int argc,char* argv[])
{
	std::unique_ptr<mg_serve_http_opts> server_options(new mg_serve_http_opts);
	server_options->document_root="web";
	server_options->enable_directory_listing="no";
	server_options->ssi_pattern="**.html$";
	std::string port="8080";

	try
	{
		mg_mgr manager;
		mg_mgr_init(&manager,server_options.get());

		mg_connection* server=mg_bind(&manager,port.c_str(),client_handler);

		if(!server)
			throw std::runtime_error("Failed ot open port "+port+
				" (Is something running on this port? Do you have permissions?).");

		mg_set_protocol_http_websocket(server);

		std::cout<<"Starting web server on port "<<port<<std::endl;

		while(true)
		{
			mg_mgr_poll(&manager,1000);
			global_game_manager.timeout_games();
			global_game_manager.cleanup_old_games();
		}

		mg_mgr_free(&manager);
	}
	catch(std::exception& error)
	{
		std::cout<<error.what()<<std::endl;
		return 1;
	}
	catch(...)
	{
		std::cout<<"Unknown error occurred."<<std::endl;
		return 1;
	}

	return 0;
}
