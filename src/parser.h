#pragma once

#include <variant>
#include <vector>
#include <iostream>

#include "scanner.h"

using namespace std;

struct ExprIntLit
{
    Token int_lit;
};

struct ExprId
{
    Token id;
};

struct Expr
{
    variant<ExprIntLit, ExprId> var;
};

struct StmtExit
{
    Expr expr;
};

struct StmtLet
{
    Token id;
    Expr expr;
};

struct Stmt
{
    variant<StmtExit, StmtLet> var;
};

struct Prgm
{
    vector<Stmt> stmts;
};

class Parser
{
    public:
        inline explicit Parser(vector<Token> tokens) : tokens(move(tokens)){}

        optional<Expr> ParseExpr()
        {
            if (peek().has_value() && peek().value().type == TokenType::int_lit)
            {
                return Expr{.var = ExprIntLit {.int_lit = Consume()}};
            }
            else if (peek().has_value() && peek().value().type == TokenType::id) 
            {
                return Expr{.var = ExprId {.id = Consume()}};
            }
            else
            {
                return {};
            }
        }

        optional<Stmt> ParseStmt()
        {
            if 
            (
                peek().value().type == TokenType::exit 
                && peek(1).value().type == TokenType::open_paren
            )
            {
                Consume();
                Consume();

                StmtExit stmtExit;

                if (auto nodeExpr = ParseExpr())
                {
                    stmtExit = StmtExit{.expr = nodeExpr.value()};
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

                return Stmt {.var = stmtExit};
            }
            else if 
            (
                peek().has_value() && peek().value().type == TokenType::let && 
                peek(1).has_value() && peek(1).value().type == TokenType::id &&
                peek(2).has_value() && peek(2).value().type == TokenType::assign
            )
            {
                Consume();
                StmtLet stmt_let = StmtLet{.id = Consume()};
                Consume();
                if (auto expr = ParseExpr())
                {
                    stmt_let.expr = expr.value();
                }
                else
                {
                    cerr << "Invalid Expression: Expected an expr" << endl;
                    exit(EXIT_FAILURE);
                }

                if (peek().has_value() && peek().value().type == TokenType::period)
                {
                    Consume();
                }
                else
                {
                    cerr << "Expected a ." << endl;
                    exit(EXIT_FAILURE);
                }

                return Stmt {.var = stmt_let};
            }
            else
            {
                return {};
            }
        }

        optional<Prgm> ParsePrgm()
        {
            Prgm prgm;
            while (peek().has_value())
            {
                if (auto stmt = ParseStmt())
                {
                    prgm.stmts.push_back(stmt.value());
                }
                else
                {
                    cerr << "Invalid Expression. Expected a statement";
                    exit(EXIT_FAILURE);
                }
            }
            return prgm;
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