#ifndef EXPR_PARSER_HPP
#define EXPR_PARSER_HPP

#include <string>
#include <utility>
#include <cmath>
#include <vector>
#include <stdexcept>

// Token types
enum token_type_t
{
	ANY,	// Never assigned, used to match any token
	NUM,	// Number of from [0-9]
	E_OP,	// Expression operation: '+' or '-'
	F_OP,	// Factor operation: '*' or '/'
	T_OP,	// Term operation: '^'
	RPAREN,	// ')'
	LPAREN,	// '('
	END,	// '.'
	VAR,	// 'x'
	EXP,	// 'e'
	FUNC,	// Function: "sin", "cos", "log"
	INVALID	// Invalid
};

// Lexer tokens of <type, lexeme>
typedef std::pair<token_type_t,std::string> token_t;

// Expression evaluator constructed from string expression
class expr_parser_t
{
	public:
		// Lex input expression into tokens
		expr_parser_t(const std::string & expr);

		// Evaluate expression
		double operator()(double x);

	private:
		// Check next token and return if it matches the type
		// If true it advances index to next token
		bool match(token_type_t the_type);

		// Parse: expr -> term + expr | term - expr | term
		double parse_expr(double x);

		// Parse: term -> factor * term | factor / term | factor
		double parse_term(double x);

		// Parse: factor -> value ^ factor | value
		double parse_factor(double x);

		// Parse: value -> NUM | VAR | EXP | (expr) | sin(expr) | cos(expr) | log(expr) | pow(expr)
		double parse_value(double x);

		// Stores value from match
		std::string cur_val;

		// Vector of tokens from parser
		std::vector<token_t> tokens;

		// Tracks evaluator index
		size_t index;
};

#endif