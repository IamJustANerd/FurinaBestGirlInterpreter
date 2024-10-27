#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser.h"
#include "Program.h"
#include <map>
#include <stdexcept>

class Interpreter
{
public:
    void Interpret(const std::unique_ptr<ASTNode> &root)
    {
        if (!root)
            return;

        switch (root->curValue.type)
        {
        case TokenType::CREATE:
            ExecuteCreate(root);
            break;
        case TokenType::ASSIGN:
            ExecuteAssign(root);
            break;
        case TokenType::PRINT:
            ExecutePrint(root);
            break;
        case TokenType::CONVERSE_TO:
            ExecuteConversion(root);
            break;
            
        default:
            throw std::runtime_error("Unknown AST node type.");
        }
    }

    void ExecuteProgram(const Program &program);

private:
    // Deklarasi variable
    void ExecuteCreate(const std::unique_ptr<ASTNode> &node);
    // Assign value ke variable
    void ExecuteAssign(const std::unique_ptr<ASTNode> &node);
    // Print nilai variable
    void ExecutePrint(const std::unique_ptr<ASTNode> &node);
    // Konversi nilai variable ke bentuk lain (biner, oktal, desimal, hexadesimal)
    void ExecuteConversion(const std::unique_ptr<ASTNode> &node);
};

#endif
