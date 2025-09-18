#pragma once

#include <iostream>
#include <optional>
#include <vector>

using namespace std;

//Types of tokens
enum class TokenType
{
    id,
    let, 
    exit,
    assign,
    period,
    int_lit, 
    open_paren,
    close_paren
};

//Token construct 
struct Token
{
    TokenType type;
    optional<string> val;
};

class Scanner
{
    public: 
        inline Scanner(const string& src) : src(move(src)){}
    
        //Split program into tokens
        inline vector<Token> Scan()
        {
            vector<Token> tokens;
            string buffer = "";

            //Parse through the program
            while(peek().has_value())
            {
                if (isalpha(peek().value()))
                {
                    buffer += Consume();
                    while (peek().has_value() && isalnum(peek().value()))
                    {
                        buffer += Consume();
                    }

                    if (buffer == "exit")
                    {
                        tokens.push_back({.type = TokenType::exit});
                        buffer = "";
                        continue;
                    }
                    else if (buffer == "let")
                    {
                        tokens.push_back({.type = TokenType::let});
                        buffer = "";
                        continue;
                    }
                    else
                    {
                        tokens.push_back({.type = TokenType::id});
                        buffer = "";
                        continue;
                    }
                }
                else if (isdigit(peek().value()))
                {
                    while (peek().has_value() && isdigit(peek().value()))
                    {
                        buffer += Consume(); 
                    }
                    
                    tokens.push_back({.type = TokenType::int_lit, .val = buffer});
                    buffer = "";
                    continue;
                }
                else if (peek().value() == '(')
                {
                    Consume();
                    tokens.push_back({.type = TokenType::open_paren});
                    continue;
                }
                else if (peek().value() == ')')
                {
                    Consume();
                    tokens.push_back({.type = TokenType::close_paren});
                    continue;
                }
                else if (peek().value() == '.')
                {
                    Consume();
                    tokens.push_back({.type = TokenType::period});
                    continue;
                }
                else if (peek().value() == '=')
                {
                    Consume();
                    tokens.push_back({.type = TokenType::assign});
                    continue;
                }
                else if (isspace(peek().value()))
                {
                    Consume();
                    continue;
                }
                else
                {
                    cerr << "Invalid Token";
                    exit(EXIT_FAILURE);
                }
            }

            return tokens;
        }

    private:
        [[nodiscard]] inline optional<char> peek(int offset = 0) const 
        {
            optional<char> returnMe;

            //If out of bounds
            if (index + offset >= src.length() || index + offset < 0)
            {
                returnMe = {};
            }
            //If valid index
            else
            {
                returnMe = src[index + offset];
            }
            
            return returnMe;
        }

        inline char Consume()
        {
            char x = src[index];
            cout << x;
            return src[index++];
        }

        const string src;
        int index = 0;
};