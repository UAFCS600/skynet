/*! @file */

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
		/**
		\struct game_info_t
		*Stores game information.
		*/
		struct game_info_t
		{
			//!Status of the game.
			skynet::checkers::status_t status;
			//!All boards played during the game (beginning with the initial board).
			skynet::checkers::board_list_t boards;
			//!Time of game creation (in seconds since the Unix epoch).
			uint64_t create_time;
			//!Last time game was modified (such as a board was played, in seconds since the Unix epoch).
			uint64_t modify_time;
		};

		//!Game list map type (key being the game name and value being the info for that game).
		typedef std::map<std::string,game_info_t> game_list_t;

		/**Downloads information for all checkers games on a given server (throws on error).
		\param server		DNS name or IP address of the hosting server (ex: "skynet.cs.uaf.edu" or "137.229.25.135").
		\return				Map of games.
		*/
		game_list_t list_games(const std::string& server);

		/**Downloads information for a specific checkers game on a given server (throws on error).
		\param server		DNS name or IP address of the hosting server (ex: "skynet.cs.uaf.edu" or "137.229.25.135").
		\param game_name	Name of the game to get info for.
		\return				Map of games.
		*/
		game_info_t info_game(const std::string& server,const std::string& game_name);

		/**Places a board (makes a move) for a given server and game name. (throws on error).
		\param server		DNS name or IP address of the hosting server (ex: "skynet.cs.uaf.edu" or "137.229.25.135").
		\param game_name	Name of the game to make a move on.
		\param board		Board to play.
		*/
		void play_game(const std::string& server,const std::string& game_name,const skynet::checkers::board_t& board);
	}
}

namespace std
{
	/**Converts a map of games to a JSON string.
	\param list				Map of game names to game info structs.
	\return					Stringified JSON object.
	*/
	std::string to_string(const skynet::checkers::game_list_t& list);

	/**Converts a game info object to a JSON string.
	\param info				Game info struct.
	\return					Stringified JSON object.
	*/
	std::string to_string(const skynet::checkers::game_info_t& info);
}

#endif
