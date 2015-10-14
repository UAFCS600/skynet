#include "board.hpp"
#include <algorithm>

/*
 *	Note specifically NOT pass by reference 
 *	since we'd be passing a const object
 *
 */
std::string toLower(std::string orig)
{
	return std::transform(orig.begin(), orig.end(), orig.begin(), ::tolower);
}

std::ostream & operator<<(std::ostream & os, checkers_board_t & rhs)
{
	int row=1; //keeps track of which row we're on
	for(int i=rhs.size(); i>=0; --i)
	{
		if(row%2==0)
		{
			os << rhs[i];
			os << " ";
		}
		else
		{
			os << " ";
			os << rhs[i];
		}
		if (i%4==0)
		{
			++row;
			os << endl;
		}
	}
	return os;
}

checkers_board_t init_board()
{
	checkers_board_t theBoard="rrrrrrrrrrrr________bbbbbbbbbbbb";
	return theBoard;
}

bool is_valid_checkers_board(const checkers_board_t& board)
{
	//reality check to make sure the board is 
	//the right size
	if (board.size()!=32)
	{
		return false;
	}

	//reality check to validate that only r, R, b, B,
	// and _ are used in the board
	for(int i=0; i<board.size(); ++i)
	{
		if (board[i]!="_" && board[i]!="B" && board[i]!="b" && board[i]!="R" && board[i]!="r")
		{
			return false
		}
	}
	cout << board << endl;
	return true;
}


checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player)
{
	//returns an empty vector if the board is invalid
	checkers_board_list_t validMoves;
	if (!is_valid_checkers_board(board))
	{
		return validMoves;
	}
	//generate the jumps first
	//will return if the size of the list 
	//is non 0 after looking for jumps
	//since jumps have to be taken if available

	//black jumps
	if(toLower(player)=="black")
	{}
	else if (toLower(player)=="red") //red jump
	{}
	else
	{return validMoves;}
}

