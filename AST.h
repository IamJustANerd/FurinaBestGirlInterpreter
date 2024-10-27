#ifndef AST_H
#define AST_H

#include "Lexer.h"
#include <memory>
#include <vector>
#include <string>

// Base class for all AST nodes
class ASTNode
{
public:
    // Constructor
    ASTNode(const Token& token) : left(nullptr), right(nullptr), curValue(token) {}

    virtual ~ASTNode() = default; // Virtual destructor for proper cleanup

    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    Token curValue;
};

#endif // AST_H