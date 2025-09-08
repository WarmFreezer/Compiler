#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <optional>

#include "scanner.h"
#include "parser.h" 
#include "generator.h"

//Possible improvements:
// Parallelize compilation by splitting statements by line

// cmake --build build && ./build/cat ./test.cat
using namespace std;

enum class TokenType;
struct Token;
string Analyze(const vector<Token>& tokens);

int main(int argc, char* argv[])
{
    //Incorrect usage error
    if (argc != 2)
    {
        cerr << "Incorrect usage. \nCorrect usage: ";
        cerr << "cat <input.cat>" << endl;
        return EXIT_FAILURE;
    }

    //Grab contents of file to be compiled
    fstream input(argv[1], ios::in);
    stringstream contentsStream;
    contentsStream << input.rdbuf(); //<-- Stored here
    input.close();

    //Turn string into tokens
    string contents = contentsStream.str();

    Scanner scanner = Scanner(contents);
    vector<Token> tokens = scanner.Scan(); //<--
     
    Parser parser = Parser(tokens);
    optional<Exit> tree = parser.Parse();

    if (!tree.has_value())
    {
        cerr << "No exit statement found" << endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(tree.value());
    fstream assembly("/home/dev/Compiler/out.asm", ios::out);
    assembly << generator.Generate(); 
    assembly.close();

    system("nasm -felf64 out.asm");
    system("ld -o out out.o");

    return EXIT_SUCCESS;
};
