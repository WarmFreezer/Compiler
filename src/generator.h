#pragma once

#include <vector>
#include <iostream>
#include <sstream>

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
            
            out << "    syscall\n";

            return out.str();
        }

    private:
        const Prgm prgm;
        stringstream out;

        void GenerateStmt(const Stmt& stmt) 
        {
            struct StmtVisitor
            {
                Generator &generator;

                void operator()(const StmtExit stmtExit) 
                {
                    generator.GenerateExpr(stmtExit.expr);
                    generator.out << "    mov rax, 60\n";
                    generator.out << "    pop rdi\n";
                }

                void operator()(const StmtLet stmtLet)
                {

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
                    generator.out << "    push rax\n";
                }

                void operator()(const ExprId exprId) 
                {
                    
                }
            };

            ExprVisitor visitor(*this);
            visit(visitor, expr.var);
        }
};