#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "AST.h"
#include <map>
#include <string>
#include <vector>

class Parser
{
public:
    Parser(Lexer &lexer);
    std::unique_ptr<ASTNode> Parse(); // Start point

    Lexer &lexer;
    Token currentToken;

    void AdvanceToken();
    std::vector<std::unique_ptr<ASTNode>> GetStatements();

private:
    // Handle berbagai kasus (deklarasi, assign, print, etc)
    std::unique_ptr<ASTNode> ParseStatement();
    std::unique_ptr<ASTNode> ParseCreate();
    std::unique_ptr<ASTNode> ParseAssign(Token saveToken);
    std::unique_ptr<ASTNode> ParsePrint();
    std::unique_ptr<ASTNode> ParseConversion(Token saveToken);

    void Error(const std::string &message);
};

#endif
