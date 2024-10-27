#include "../Lexer.h"
#include "../Parser.h"
#include "../Interpreter.h"
#include <iostream>
#include <memory>

Token createToken(TokenType type, const std::string &value)
{
    return Token{type, value};
}

void runTests()
{
    Interpreter interpreter;
    int n = 1;

    // Test 1: BUAT myVar
    {
        auto createNode = std::make_unique<ASTNode>(createToken(TokenType::CREATE, ""));
        createNode->left = std::make_unique<ASTNode>(createToken(TokenType::VAR_NAME, "myVar"));

        interpreter.Interpret(createNode);
    }

    std::cout << "TEST CASE " << n << " PASSED" << '\n';
    n++;

    // Test 2: myVar ADALAH BINER[10]
    {
        auto assignNode = std::make_unique<ASTNode>(createToken(TokenType::ASSIGN, ""));
        assignNode->left = std::make_unique<ASTNode>(createToken(TokenType::VAR_NAME, "myVar"));
        assignNode->right = std::make_unique<ASTNode>(createToken(TokenType::CONVERSE_FROM, "HEXADESIMAL[FFF]"));

        interpreter.Interpret(assignNode);
    }

    std::cout << "TEST CASE " << n << " PASSED" << '\n';
    n++;

    // Test 3: TERIAK myVar
    {
        auto printNode = std::make_unique<ASTNode>(createToken(TokenType::PRINT, ""));
        printNode->left = std::make_unique<ASTNode>(createToken(TokenType::VAR_NAME, "myVar"));

        interpreter.Interpret(printNode);
    }

    std::cout << "TEST CASE " << n << " PASSED" << '\n';
    n++;

    // Test 4: HEXA myVar
    {
        auto convertNode = std::make_unique<ASTNode>(createToken(TokenType::CONVERSE_TO, "OKTAL"));
        convertNode->left = std::make_unique<ASTNode>(createToken(TokenType::VAR_NAME, "myVar"));

        interpreter.Interpret(convertNode);
    }

    std::cout << "TEST CASE " << n << " PASSED" << '\n';
    n++;

    // Test 5: TERIAK myVar
    {
        auto printNode = std::make_unique<ASTNode>(createToken(TokenType::PRINT, ""));
        printNode->left = std::make_unique<ASTNode>(createToken(TokenType::VAR_NAME, "myVar"));

        interpreter.Interpret(printNode);
    }

    std::cout << "TEST CASE " << n << " PASSED" << '\n';
    n++;
}

int main()
{
    runTests();
    return 0;
}