/*! @file */

#ifndef CHECKERS_HPP
#define CHECKERS_HPP

#include <string>
#include <vector>

namespace skynet
{
	//! Enum for determining the checkers player (used primarily for making moves).
	enum checkers_player_t
	{
		RED,			/**<Red player.*/
		BLACK			/**<Black player.*/
	};

	/**String to checkers player conversion function.
	\param str			String representing the checkers player ("red" or "black" - case insensitive).
	\return 			Enum for indicated player.
	*/
	checkers_player_t checkers_player_from_string(const std::string& str);

	//!Enum for determing the status of a checkers game.
	enum checkers_status_t
	{
		RED_TURN,		/**<Red player's turn.*/
		BLACK_TURN,		/**<Black player's turn.*/
		RED_WON,		/**<Red player has won or black player has lost (ex: invalid move).*/
		BLACK_WON		/**<Black player has won or red player has lost (ex: invalid move).*/
	};

	/**String to checkers status conversion function.
	\param str			String representing the checkers status ("red_turn", "black_turn", "red_won", or "black_won" - case insensitive).
	\return 			Enum for indicated status.
	*/
	checkers_status_t checkers_status_from_string(const std::string& str);

	//!Checkers board type containing 'r' for a red pawn, 'R' for a red king, 'b' for a black pawn, 'B' for a black king, and '_' for an empty square.
	typedef std::string checkers_board_t;

	//!Checkers board list type (vector of checkers boards).
	typedef std::vector<checkers_board_t> checkers_board_list_t;

	/**Validation function, throws std::runtime_error if passed board is invalid (in terms of format).
	\param board		String representation of the checkers board (ex: "rrrrrrrrrrrr________bbbbbbbbbbbb").
	*/
	void validate(const checkers_board_t& board);

	/**Generates moves for a given checkers board (throws std::runtime_error on boards with invalid formatting).
	\param board		String representation of the checkers board (ex: "rrrrrrrrrrrr________bbbbbbbbbbbb").
	\param player		Player to find moves for.
	\return				List of possibly moves for the given player from the given board.
	*/
	checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);
}

namespace std
{
	/**Checkers player to string function.
	\param player		Player to convert.
	\return 			String value of the enum typed player.
	*/
	std::string to_string(const skynet::checkers_player_t& player);

	/**Checkers status to string function.
	\param status		Status to convert.
	\return 			String value of the enum typed status.
	*/
	std::string to_string(const skynet::checkers_status_t& status);

	/**Checkers board list to string function.
	\param boards		List of checkers boards to convert.
	\return 			JSON formatted array of checkers boards.
	*/
	std::string to_string(const skynet::checkers_board_list_t& boards);
}

#endif
