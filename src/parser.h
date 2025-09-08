#pragma once

#include <vector>
#include <iostream>

#include "scanner.h"

using namespace std;

struct Expr
{
    Token int_lit;
};

struct Exit
{
    Expr expr;
};

class Parser
{
    public:
        inline explicit Parser(vector<Token> tokens) : tokens(move(tokens)){}

        optional<Expr> ParseExpr()
        {
            if (peek().has_value() && peek().value().type == TokenType::int_lit)
            {
                return Expr{.int_lit = Consume()};
            }
        }

        optional<Exit> Parse()
        {
            optional<Exit> exitNode;
            while(peek().has_value())
            {
                if 
                (
                    peek().value().type == TokenType::exit 
                    && peek(1).value().type == TokenType::open_paren
                )
                {
                    Consume();
                    Consume();
                    if (auto nodeExpr = ParseExpr())
                    {
                        exitNode = Exit{.expr = nodeExpr.value()};
                    }
                    else
                    {
                        cerr << "Invalid expression" << endl;
                        exit(EXIT_FAILURE);
                    }
                    
                    if(peek().has_value() && peek().value().type == TokenType::close_paren)
                    {
                        Consume();
                    }
                    else
                    {
                        cerr  << "Expected a \")\"" << endl;
                        exit(EXIT_FAILURE);
                    }

                    if (peek().has_value() && peek().value().type == TokenType::period)
                    {
                        Consume();
                    }
                    else
                    {
                        cerr << "Expected a \".\"" << endl;
                        exit(EXIT_FAILURE);
                    }
                }
            }
            index = 0;
            return exitNode;
        }

    private:
        [[nodiscard]] inline optional<Token> peek(int amount = 0) const 
        {
            optional<Token> returnMe;

            //If out of bounds
            if (index + amount >= tokens.size() || index + amount < 0)
            {
                returnMe = {};
            }
            //If valid index
            else
            {
                returnMe = tokens[index + amount];
            }
            
            return returnMe;
        }

        inline Token Consume()
        {
            return tokens[index++];
        }

        const vector<Token> tokens;
        size_t index = 0;

};