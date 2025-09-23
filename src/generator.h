#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "parser.h"

using namespace std;
class Generator
{
    public: 
        inline explicit Generator(Prgm prgm) : prgm(move(prgm)){}
    
        [[nodiscard]] string GeneratePrgm() 
        {
            out << "global _start\n_start:\n";

            //Where we generate stmts
            for (const Stmt& stmt : prgm.stmts)
            {
                GenerateStmt(stmt);
            }
            
            out << "    mov rax, 0\n";
            Push("rax");
            out << "    mov rax, 60\n";
            Pop("rdi");
            out << "    syscall\n";

            return out.str();
        }

    private:
        struct Var 
        {   
            size_t stackLoc;
        };

        void Push(const string reg)
        {
            out << "    push " << reg << endl;
            stackSize++;
        }

        void Pop(const string reg)
        {
            out << "    pop " << reg << endl;
            stackSize--;
        }

        void GenerateStmt(const Stmt& stmt) 
        {
            struct StmtVisitor
            {
                Generator &generator;

                void operator()(const StmtExit stmtExit) 
                {
                    generator.GenerateExpr(stmtExit.expr);
                    generator.out << "    mov rax, 60\n";
                    generator.Pop("rdi");
                    generator.out << "    syscall\n";
                }

                void operator()(const StmtLet stmtLet)
                {
                    if (generator.vars.contains(stmtLet.id.val.value()))
                    {
                        cerr << "Identifier already used: " << stmtLet.id.val.value() << endl;
                        exit(EXIT_FAILURE);
                    }
                    
                    generator.vars.insert({stmtLet.id.val.value(), Var {.stackLoc = generator.stackSize}});
                    generator.GenerateExpr(stmtLet.expr);
                }    
            };

            StmtVisitor visitor(*this);
            visit(visitor, stmt.var);
        }

        void GenerateExpr(const Expr expr) 
        {
            struct ExprVisitor 
            {
                Generator &generator;
                
                void operator()(const ExprIntLit exprIntLit) 
                {
                    generator.out << "    mov rax, " << exprIntLit.int_lit.val.value() << endl;
                    generator.Push("rax");
                }

                void operator()(const ExprId exprId) 
                {
                    if (!generator.vars.contains(exprId.id.val.value()))
                    {
                        cerr << "Undeclared Identifier" << exprId.id.val.value() << endl;
                        exit(EXIT_FAILURE);
                    }
                    
                    const auto& var = generator.vars.at(exprId.id.val.value());

                    generator.Push("QWORD [rsp + " + to_string((generator.stackSize - var.stackLoc - 1) * 8)  + "]");
                }
            };

            ExprVisitor visitor(*this);
            visit(visitor, expr.var);
        }

        const Prgm prgm;
        stringstream out;
        size_t stackSize = 0;
        unordered_map<string, Var> vars;
};