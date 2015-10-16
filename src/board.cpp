#include "board.hpp"
#include <algorithm>
#include <locale>

/*
 *	Note specifically NOT pass by reference 
 *	since we'd be passing a const object
 *
 */
std::string toLowerCase(std::string  orig)
{
	std::string rv="";
	for(int i=0; i<orig.size(); ++i)
	{
		rv+=std::tolower(orig[i]);
	}
	return rv;
}

std::string getBoard(const checkers_board_t & rhs)
{
	std::ostringstream oss;
	int row=1; //keeps track of which row we're on
	for(int i=rhs.size(); i>=0; --i)
	{
		if(row%2==0)
		{
			oss << rhs[i];
			oss << " ";
		}
		else
		{
			oss << " ";
			oss << rhs[i];
		}
		if (i%4==0)
		{
			++row;
			oss << std::endl;
		}
	}
	return oss.str();
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
		if (board[i]!='_' && board[i]!='B' && board[i]!='b' && board[i]!='R' && board[i]!='r')
		{
			return false;
		}
	}
	std::cout << board << std::endl;
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
	if(toLowerCase(player)=="black")
	{
		int row=0;
		for(int i=0; i<board.size(); ++i)
		{
			//black king
			if (board[i]=='B')
			{
				//odd rows
				if (row%2==1)
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='B';
						validMoves.push_back(temp);
					}
					if (board[i-5]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-5]='B';
						validMoves.push_back(temp);
					}
					if (board[i+3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+3]='B';
						validMoves.push_back(temp);
					}

					if (board[i+4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+4]='B';
						validMoves.push_back(temp);
					}
				}
				else
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='B';
						validMoves.push_back(temp);
					}
					if (board[i+5]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+5]='B';
						validMoves.push_back(temp);
					}
					if (board[i-3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-3]='B';
						validMoves.push_back(temp);
					}

					if (board[i+4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+4]='B';
						validMoves.push_back(temp);
					}
				}

			}
			else if (board[i]=='b')	//black pawn
			{
				//odd rows
				if (row%2==1)
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='b';
						validMoves.push_back(temp);
					}
					if (board[i-5]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-5]='b';
						validMoves.push_back(temp);
					}
				}
				else
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='b';
						validMoves.push_back(temp);
					}
					if (board[i-3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-3]='b';
						validMoves.push_back(temp);
					}
				}
				
			}
			//increment the rows
			if (i!=0  && i%4==0 )
			{
				row++;
			}
		}
	}
	else if (toLowerCase(player)=="red") //red jump
	{
		int row=0;
		for(int i=0; i<board.size(); ++i)
		{
			if (board[i]=='R')
			{
				//even rows
				if (row%2==0)
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='R';
						validMoves.push_back(temp);
					}
					if (board[i-5]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-5]='R';
						validMoves.push_back(temp);
					}
					if (board[i+3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+3]='R';
						validMoves.push_back(temp);
					}

					if (board[i+4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+4]='R';
						validMoves.push_back(temp);
					}
				}
				else
				{
					if (board[i-4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-4]='R';
						validMoves.push_back(temp);
					}
					if (board[i+5]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+5]='R';
						validMoves.push_back(temp);
					}
					if (board[i-3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i-3]='R';
						validMoves.push_back(temp);
					}

					if (board[i+4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+4]='R';
						validMoves.push_back(temp);
					}
				}
			}
			else if (board[i]=='r')
			{
				//even rows
				if (row%2==0)
				{
					if (board[i+3]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+3]='r';
						validMoves.push_back(temp);
					}
					if (board[i+4]=='_')
					{
						std::string temp(board);
						temp[i]='_';
						temp[i+4]='r';
						validMoves.push_back(temp);
					}
				}
			}
			//increment the rows
			if (i!=0  && i%4==0 )
			{
				row++;
			}
		}

	}

	return validMoves;
}

