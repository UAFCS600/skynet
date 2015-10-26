/*
 * expr_parser.cpp
 * Author: Aven Bross
 * Date: 10/18/2015
 *
 * Recursive descent parser for custom sigmoid
 */

#include "expr_parser.hpp"

// Lex input expression into tokens
expr_parser_t::expr_parser_t(const std::string & expr)
{
	bool concat_num = false;
	bool decimal = false;
	std::string value;
	int i = 0;

	while(i < expr.size())
	{
		char c = expr[i++];
		std::string s(1,c);

		if(concat_num)
		{
			if(c >= '0' && c <= '9')
			{
				value += c;
				continue;
			}
			else if(c == '.' && !decimal)
			{
				decimal=true;
				value += c;
				continue;
			}

			tokens.push_back(token_t(NUM,value));
			value = "";
			concat_num = false;
			decimal = false;
		}

		switch(c)
		{
			case '0': case '1':
			case '2': case '3':
			case '4': case '5':
			case '6': case '7':
			case '8': case '9':
				concat_num = true;
				value += c;
				break;
			case '.':
				concat_num = true;
				decimal = true;
				value += c;
				break;
			case '+':
			case '-':
				tokens.push_back(token_t(E_OP,s));
				break;
			case '*':
			case '/':
				tokens.push_back(token_t(T_OP,s));
				break;
			case '^':
				tokens.push_back(token_t(F_OP,s));
				break;
			case '(':
				tokens.push_back(token_t(LPAREN,s));
				break;
			case ')':
				tokens.push_back(token_t(RPAREN,s));
				break;
			case 'x':
				tokens.push_back(token_t(VAR,s));
				break;
			case 'e':
				tokens.push_back(token_t(EXP,s));
				break;
			case 's':
				if(expr[i] == 'i' && expr[i + 1] == 'n')
				{
					tokens.push_back(token_t(FUNC,"sin"));
					i += 2;
				}
				else
				{
					tokens.push_back(token_t(INVALID,s));
				}
				break;
			case 'c':
				if(expr[i] == 'o' && expr[i + 1] == 's')
				{
					tokens.push_back(token_t(FUNC,"cos"));
					i += 2;
				}
				else
				{
					tokens.push_back(token_t(INVALID,s));
				}
				break;
			case 'l':
				if(expr[i] == 'o' && expr[i + 1] == 'g')
				{
					tokens.push_back(token_t(FUNC,"log"));
					i += 2;
				}
				else
				{
					tokens.push_back(token_t(INVALID,s));
				}
				break;
			default:
				tokens.push_back(token_t(INVALID,s));
		}
	}

	// Extra check to catch single number values
	if(value.compare("")) tokens.push_back(token_t(NUM,value));
}

// Evaluate f(x) w/ given x value
double expr_parser_t::operator()(double x)
{
	// Set index to first token
	index = 0;

	// Evaluate expression with given x
	double value = parse_expr(x);

	// Deal with excess tokens
	if(match(ANY))
	{
		throw std::runtime_error("Invalid token \'" + cur_val + "\'.");
	}

	return value;
}

// Check next token and return if it matches the type
// If true it advances index to next token
bool expr_parser_t::match(token_type_t the_type)
{
	if(index == tokens.size()) return false;

	token_t t = tokens[index];

	if(t.first == the_type || the_type == ANY)
	{
		index++;
		cur_val = t.second;
		return true;
	}

	return false;
}

// Parse: expr -> term + expr | term - expr | term
double expr_parser_t::parse_expr(double x)
{
	double value = parse_term(x);

	while(match(E_OP))
	{
		if(cur_val[0] == '+')
		{
			value += parse_term(x);
		}
		else
		{
			value -= parse_term(x);
		}
	}

	return value;
}

// Parse: term -> factor * term | factor / term | factor
double expr_parser_t::parse_term(double x)
{
	double value = parse_factor(x);

	while(match(T_OP))
	{
		if(cur_val[0] == '*')
		{
			value *= parse_factor(x);
		}
		else
		{
			double term = parse_factor(x);

			if(term == 0)
			{
				throw std::runtime_error("Attempted division by 0.");
			}

			value /= term;
		}
	}

	return value;
}

// Parse: factor -> signval ^ factor | signval
double expr_parser_t::parse_factor(double x)
{
	double value = parse_signval(x);
	
	while(match(F_OP))
	{
		value = std::pow(value, parse_signval(x));
	}
	
	return value;
}

// Parse: signval -> + value | - value | value
double expr_parser_t::parse_signval(double x)
{
	if(match(E_OP))
	{
		double sign = -1;
		
		if(cur_val[0]=='+')
		{
			sign = 1;
		}
		
		return sign * parse_value(x);
	}
	else
	{
		return parse_value(x);
	}
}

// Parse: value -> NUM | VAR | EXP | (expr) | sin(expr) | cos(expr) | log(expr)
double expr_parser_t::parse_value(double x)
{
	if(match(NUM))
	{
		return std::stod(cur_val);
	}

	if(match(VAR))
	{
		return x;
	}

	if(match(EXP))
	{
		return std::exp(1.0);
	}

	if(match(LPAREN))
	{
		double val = parse_expr(x);
		if(match(RPAREN))
		{
			return val;
		}
		else
		{
			throw std::runtime_error("Missing ending \')\'.");
		}
	}

	if(match(FUNC))
	{
		char func = cur_val[0];

		if(match(LPAREN))
		{
			double val = parse_expr(x);

			if(match(RPAREN))
			{
				if(func == 's')
				{
					return std::sin(val);
				}
				else if(func == 'c')
				{
					return std::cos(val);
				}
				else
				{
					if(val <= 0)
					{
						throw std::runtime_error("Attempted log(x) with x<=0.");
					}

					return std::log(val);
				}
			}
			else
			{
				throw std::runtime_error("Missing ending \')\'.");
			}
		}
		else
		{
			throw std::runtime_error("Missing \'(\' after function.");
		}
	}

	if(match(ANY))
	{
		throw std::runtime_error("Invalid token \'" + cur_val + "\'.");
	}

	return 0;
}
