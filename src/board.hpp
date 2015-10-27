#ifndef CHECKERBOARD_HPP
#define CHECKERBOARD_HPP

#include <string>
#include <vector>

namespace ai
{
typedef std::string checkers_player_t;
typedef std::string checkers_board_t;
typedef std::vector<checkers_board_t> checkers_board_list_t;
bool is_valid_checkers_board(const checkers_board_t& board);
checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);
}

std::string getBoard(const ai::checkers_board_t & rhs);
#endif
