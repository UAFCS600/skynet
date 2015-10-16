#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
typedef std::string checkers_player_t;
typedef std::string checkers_status_t;
typedef std::string checkers_board_t;
typedef std::vector<checkers_board_t> checkers_board_list_t;
bool is_valid_checkers_board(const checkers_board_t& board);
checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);
std::string getBoard(const checkers_board_t & rhs);

#endif