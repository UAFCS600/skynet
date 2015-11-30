#ifndef AI_HPP
#define AI_HPP

#include <string>
#include <utility>
#include <vector>

//Wrap everything in a namespace?
namespace skynet
{
	//Neural Net Stuff
	class neuralnet_t
	{
		public:
			typedef size_t layer_t;
			typedef std::vector<size_t> layer_list_t;
			typedef size_t weight_t;
			typedef std::vector<double> weight_list_t;
			typedef size_t input_t;
			typedef std::vector<double> input_list_t;
			typedef size_t sigmoid_t;

			neuralnet_t(const layer_list_t& layers,const weight_list_t& weights);
			double evaluate(const input_list_t& inputs,const sigmoid_t& sigmoid_index);
	};
	typedef std::vector<neuralnet_t> neuralnet_list_t;

	//Checkers Board Stuff
	enum checkers_player_t
	{
		RED,
		BLACK
	};

	checkers_player_t checkers_player_from_string(const std::string& str);

	enum checkers_status_t
	{
		RED_TURN,
		BLACK_TURN,
		RED_WON,
		BLACK_WON
	};

	checkers_status_t checkers_status_from_string(const std::string& str);

	typedef std::string checkers_board_t;
	typedef std::vector<checkers_board_t> checkers_board_list_t;

	void validate(const checkers_board_t& board);

	checkers_board_list_t move_generator(const checkers_board_t& board,const checkers_player_t& player);

	//Alpha Beta
	class alpha_beta_t
	{
		public:
			alpha_beta_t(const double alpha,const double beta,const size_t depth,const size_t max_depth);

			checkers_board_t search(const checkers_board_t& start,const neuralnet_t& net,const checkers_player_t& player);

		private:
			double alpha_m;
			double beta_m;
			size_t depth_m;
			size_t max_depth_m;

			typedef std::pair<checkers_board_t,double> return_t;
			return_t search_recursive(const checkers_board_t& start,const neuralnet_t& net,const checkers_player_t& player);
	};

	//Game Playing
	checkers_status_t play(const neuralnet_t& red,const neuralnet_t& black,const checkers_board_t& board=checkers_board_t());

	//Tournament
	typedef size_t point_t;
	typedef std::pair<neuralnet_list_t,point_t> ranking_t;
	typedef std::vector<ranking_t> ranking_list_t;
	ranking_list_t tournament(const neuralnet_list_t& nets);

	//Evolution
	neuralnet_list_t evolve(const ranking_list_t& ranking);
}

namespace std
{
	std::string to_string(const skynet::checkers_player_t& player);
	std::string to_string(const skynet::checkers_status_t& status);
	std::string to_string(const skynet::checkers_board_list_t& boards);
}

#endif
