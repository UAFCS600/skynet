#ifndef EXPR_PARSER_HPP
#define EXPR_PARSER_HPP
 
#include <string>
#include <queue>
#include <utility>
#include <cctype>
#include <cmath>
#include <vector>

// token types
enum token_type_t
{
    NUM,    // Number of form [0-9]
    E_OP,    // Expression operation: '+' or '-'
    F_OP,    // Factor operation: '*' or '/'
    T_OP,    // Term operation: '^'
    RPAREN, // ')'
    LPAREN, // '('
    END,    // '.'
    VAR,    // 'x'
    FUNC,   // Function: "sin", "cos", "log"
    INVALID // Anything else
};

// Lexer tokens of <type, lexeme>
typedef std::pair<token_type_t,std::string> token_t;


// Expression evaluator constructed from string expression
class expr_parser_t
{
    public:
        // Lex input expression and 
        expr_parser_t(const std::string & expr);
        
        // Evaluate f(x) w/ given x value, returns true if there are no errors
        double eval(double x = 0);
        
        // Returns whether the last evaluation was valid
        bool valid();
        
        // Get error string
        std::string getErrors();

    private:
        // Check next token and return if it matches the type.
        // Also advances to next token_t if it matches.
        bool match(token_type_t the_type);
        
        // Stores value from match
        std::string _cur_val;
        
        // Parse expression (expr -> expr + term | expr - term | term)
        double parse_expr(double);

        // Parse term (expr -> term * factor | term / factor | factor)
        double parse_term(double);

        // Parse factor (factor -> INT | (expr))
        double parse_factor(double);

        double parse_value(double);

        // Vector of tokens from parser
        std::vector<token_t> _tokens;
        
        // Tracks evaluator index
        int _index;
        
        // Queue of parse errors
        std::string _errors;

};

#endif