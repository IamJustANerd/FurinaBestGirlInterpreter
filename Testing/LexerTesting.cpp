#include "../Lexer.h"
#include <cassert>
#include <iostream>

/*
    Note:
    -> Ntah kenapa '_' ga terbaca (perlu modifikasi checkingnya keknya)
*/
void testLexer()
{
    int n = 1;

    // Test case 1 (PASSED)
    std::string input = "BUAT SKIBIDI";
    std::cout << input << '\n';
    Lexer lexer(input);
    Token token1 = lexer.GetNextToken();
    assert(token1.type == TokenType::CREATE && token1.value == "#");

    Token token2 = lexer.GetNextToken();
    assert(token2.type == TokenType::VAR_NAME && token2.value == "SKIBIDI");

    std::cout << "CASE " << n << " PASSED" << "\n\n";
    n++;

    // Test case 2 (PASSED)
    input = "SKIBIDI ADALAH @BINER[100]";
    std::cout << input << '\n';
    Lexer lexer2(input);
    token1 = lexer2.GetNextToken();
    assert(token1.type == TokenType::VAR_NAME && token1.value == "SKIBIDI");

    token2 = lexer2.GetNextToken();
    assert(token2.type == TokenType::ASSIGN && token2.value == "=");

    Token token3 = lexer2.GetNextToken();
    std::cout << token3.value << '\n';
    assert(token3.type == TokenType::CONVERSE_FROM && token3.value == "BINER[100]");

    std::cout << "CASE " << n << " PASSED" << "\n\n";
    n++;

    // Test case 3 (PASSED)
    input = "SKIBIDI KE OKTAL";
    std::cout << input << '\n';
    Lexer lexer3(input);
    token1 = lexer3.GetNextToken();
    assert(token1.type == TokenType::VAR_NAME && token1.value == "SKIBIDI");

    token2 = lexer3.GetNextToken();
    std::cout << token2.value << '\n';
    assert(token2.type == TokenType::CONVERSE_TO && token2.value == "OKTAL");

    std::cout << "CASE " << n << " PASSED" << "\n\n";
    n++;

    // Test case 4 (PASSED)
    input = "BUAT x\nx ADALAH @BINER[1010]\nTERIAK x";
    std::cout << input << '\n';
    Lexer lexer4(input);
    token1 = lexer4.GetNextToken();
    assert(token1.type == TokenType::CREATE);

    token2 = lexer4.GetNextToken();
    assert(token2.type == TokenType::VAR_NAME && token2.value == "x");

    token3 = lexer4.GetNextToken();
    assert(token3.type == TokenType::VAR_NAME && token3.value == "x");

    Token token4 = lexer4.GetNextToken();
    assert(token4.type == TokenType::ASSIGN);

    Token token5 = lexer4.GetNextToken();
    assert(token5.type == TokenType::CONVERSE_FROM && token5.value == "BINER[1010]");

    Token token6 = lexer4.GetNextToken();
    assert(token6.type == TokenType::PRINT);

    Token token7 = lexer4.GetNextToken();
    assert(token7.type == TokenType::VAR_NAME && token7.value == "x");

    std::cout << "CASE " << n << " PASSED" << "\n\n";
    n++;
}

int main()
{
    testLexer();
    std::cout << "All lexer tests passed!" << std::endl;
    return 0;
}