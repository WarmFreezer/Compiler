#pragma once

#include <vector>
#include <iostream>
#include <sstream>

#include "parser.h"

using namespace std;
class Generator
{
    public: 
        inline explicit Generator(Exit root) : root(move(root)){}
    
        [[nodiscard]] string Generate() const
        {
            stringstream out;
            out << "global _start\n_start:\n";
            out << "    mov rax, 60\n";
            out << "    mov rdi, " << root.expr.int_lit.val.value() << endl;
            out << "    syscall\n";

            return out.str();
        }

    private:
        const Exit root;
};