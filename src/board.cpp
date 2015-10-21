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
			std::cout << "got tripped" << std::endl;
			return false;
		}
	}

	int numPieces=0;
	for(int i=0; i<board.size(); ++i)
	{
		if (board[i]=='B' && board[i]=='b' && board[i]=='R' && board[i]=='r')
			{numPieces++;}
	}
	if(!numPieces)
	{return false;}
	//std::cout << board << std::endl;
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
			if (board[i]=='B')
			{	
				if(i==0)	//corner with one jump
				{
					if(tolower(board[i+5])=='r' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
				}
				else if (i==3)	//corner
				{
					if(tolower(board[i+4])=='r' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
				}
				else if (i==28)	//corner
				{
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
				}
				else if (i==31) //corner
				{
					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
				}
				else if (i<4) // we can only jump back
				{
					if(tolower(board[i+5])=='r' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='r' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
				}
				else if (i>27)	//we can only jump forward
				{
					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
				}
				else if(i%4==0)		//left edge
				{
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='r' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='B';
						validMoves.push_back(temp);
					}

				}
				else if (i%4==3)	//right edge
				{
					if(tolower(board[i+4])=='r' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
				}
				else if (row%2)  //odd rows
				{

					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+3])=='r' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+3]='_';
						temp[i+7]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='r' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='B';
						validMoves.push_back(temp);
					}
				}
				else
				{
					if(tolower(board[i-4])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-3])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-3]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+5])=='r' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+5]='_';
						temp[i+9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='r' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='B';
						validMoves.push_back(temp);
					}
				}
			}
			else if(board[i]=='b')
			{
						
				if (i==28)	//corner
				{
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='b';
						validMoves.push_back(temp);
					}
				}
				else if (i==31) //corner
				{
					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='b';
						validMoves.push_back(temp);
					}
				}
				else if (i>27)	//we can only jump forward
				{
					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='b';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='b';
						validMoves.push_back(temp);
					}
				}
				else if(i%4==0)		//left edge
				{
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='b';
						validMoves.push_back(temp);
					}

				}
				else if (i%4==3)	//right edge
				{

					if(tolower(board[i-4])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='b';
						validMoves.push_back(temp);
					}
				}
				else if (row%2)  //odd rows
				{

					if(tolower(board[i-5])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}

				}
				else
				{
					if(tolower(board[i-4])=='r' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='B';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-3])=='r' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-3]='_';
						temp[i-7]='B';
						validMoves.push_back(temp);
					}

				}
			}
		}
	}
	else if (toLowerCase(player)=="red")
	{
		int row=0;
		for(int i=0; i<board.size(); ++i)
		{
			if (board[i]=='R')
			{

				if(i==0)	//corner with one jump
				{
					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
				}
				else if (i==3)	//corner
				{
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
				}
				else if (i==28)	//corner
				{
					if(tolower(board[i-4])=='b' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
				}
				else if (i==31) //corner
				{
					if(tolower(board[i-5])=='b' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
				}
				else if (i<4) // we can only jump back
				{
					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
				}
				else if (i>27)	//we can only jump forward
				{
					if(tolower(board[i-5])=='b' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='b' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
				}
				else if(i%4==0)		//left edge
				{
					if(tolower(board[i-4])=='b' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='R';
						validMoves.push_back(temp);
					}

				}
				else if (i%4==3)	//right edge
				{
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='b' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
				}
				else if (row%2)  //odd rows
				{

					if(tolower(board[i-5])=='b' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-4])=='b' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+3])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+3]='_';
						temp[i+7]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='R';
						validMoves.push_back(temp);
					}
				}
				else
				{
					if(tolower(board[i-4])=='b' && board[i-9]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-9]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i-3])=='b' && board[i-7]=='_')
					{
						std::string temp(board);
						temp[i-3]='_';
						temp[i-7]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+5]='_';
						temp[i+9]='R';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='R';
						validMoves.push_back(temp);
					}
				}
			}
			else if (board[i]=='r')
			{
				if(i==0)	//corner with one jump
				{
					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+5]='_';
						temp[i+9]='r';
						validMoves.push_back(temp);
					}
				}
				else if (i==3)	//corner
				{
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='r';
						validMoves.push_back(temp);
					}
				}

				else if (i<4) // we can only jump back
				{
					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i-5]='_';
						temp[i-9]='r';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i-4]='_';
						temp[i-7]='r';
						validMoves.push_back(temp);
					}
				}
				else if(i%4==0)		//left edge
				{

					if(tolower(board[i+4])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='r';
						validMoves.push_back(temp);
					}

				}
				else if (i%4==3)	//right edge
				{
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='r';
						validMoves.push_back(temp);
					}
	
				}
				else if (row%2)  //odd rows
				{


					if(tolower(board[i+3])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+3]='_';
						temp[i+7]='r';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+9]='r';
						validMoves.push_back(temp);
					}
				}
				else
				{

					if(tolower(board[i+5])=='b' && board[i+9]=='_')
					{
						std::string temp(board);
						temp[i+5]='_';
						temp[i+9]='r';
						validMoves.push_back(temp);
					}
					if(tolower(board[i+4])=='b' && board[i+7]=='_')
					{
						std::string temp(board);
						temp[i+4]='_';
						temp[i+7]='r';
						validMoves.push_back(temp);
					}
				}

			}
		
		}
	}

	//return if we have jumps
	//this way we ensure that we have jumps instead 
	//of moves
	if (validMoves.size())
	{
		return validMoves;
	}

	//black moves
	if(toLowerCase(player)=="black")
	{
		int row=0;
		for(int i=0; i<board.size(); ++i)
		{
			//black king
			if (board[i]=='B')
			{
					if (i==3)	//corner
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='B';
							validMoves.push_back(temp);
						}
					}
					else if (i==28)	//corner
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='B';
							validMoves.push_back(temp);
						}
					}
					else if (i<4) // we can only jump back
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='B';
							validMoves.push_back(temp);
						}
				

					}
					else if (i>27)	//we can only jump forward
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
					}
					else if(i%8==4 || i%8==3)		//left edge
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='B';
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
					else if (row%2)  //odd rows
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='B';
							validMoves.push_back(temp);
						}
						if (board[i+5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+5]='B';
							validMoves.push_back(temp);
						}
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='B';
							validMoves.push_back(temp);
						}
						if (board[i+5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+5]='B';
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
						if (board[i-5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-5]='B';
							validMoves.push_back(temp);
						}
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='B';
							validMoves.push_back(temp);
						}
						if (board[i+3]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+3]='B';
							validMoves.push_back(temp);
						}
					}
			}
			else if (board[i]=='b')	//black pawn
			{
				
					if (i==3)	//corner
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='b';
							validMoves.push_back(temp);
						}
					}
					else if(i%8==4 || i%8==3)		//edges
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='b';
							validMoves.push_back(temp);
						}

					}
					else if (row%2)  //odd rows
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
	else if (toLowerCase(player)=="red") //red moves
	{
		int row=0;
		for(int i=0; i<board.size(); ++i)
		{
			if (board[i]=='R')
			{
				if (i==3)	//corner
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='R';
							validMoves.push_back(temp);
						}
					}
					else if (i==28)	//corner
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='R';
							validMoves.push_back(temp);
						}
					}
					else if (i<4) // we can only jump back
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='R';
							validMoves.push_back(temp);
						}
				

					}
					else if (i>27)	//we can only jump forward
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
					}
					else if(i%8==4 || i%8==3)		//left edge
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='R';
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
					else if (row%2)  //odd rows
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='R';
							validMoves.push_back(temp);
						}
						if (board[i+5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+5]='R';
							validMoves.push_back(temp);
						}
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='R';
							validMoves.push_back(temp);
						}
						if (board[i+5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+5]='R';
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
						if (board[i-5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-5]='R';
							validMoves.push_back(temp);
						}
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='R';
							validMoves.push_back(temp);
						}
						if (board[i+3]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+3]='R';
							validMoves.push_back(temp);
						}
					}
			}
			else if (board[i]=='r')
			{
					if (i==8)	//corner
					{
						if (board[i-4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i-4]='r';
							validMoves.push_back(temp);
						}
					}
					else if(i%8==4 || i%8==3)		//edges
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='r';
							validMoves.push_back(temp);
						}

					}
					else if (row%2)  //odd rows
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='r';
							validMoves.push_back(temp);
						}
						if (board[i+5]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+5]='r';
							validMoves.push_back(temp);
						}
			
					}
					else
					{
						if (board[i+4]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+4]='r';
							validMoves.push_back(temp);
						}
						if (board[i+3]=='_')
						{
							std::string temp(board);
							temp[i]='_';
							temp[i+3]='r';
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

