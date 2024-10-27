#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType
{
    VAR_NAME,       // 0 ->
    CREATE,         // 1 -> #
    ASSIGN,         // 2 -> =
    CONVERSE_FROM,  // 3 -> @BINER[x], @OKTAL[y], @DECIMAL[z], @HEXADECIMAL[a]
    CONVERSE_TO,    // 4 -> !BINER, !OKTAL, !DECIMAL, !HEXADECIMAL
    END,            // 5 ->
    PRINT,          // 6 -> ~
    ERROR           // 7 -> 
};

/*
    Untuk menyimpan jenis dan value token
    Contohnya, untuk:
    SKIBIDI = !OKTAL SKI + BIDI
    Tokennya adalah:
    -> SKIBIDI (VAR_NAME)
    -> = (ASSIGN)
    -> OKTAL (CONVERSE_TO)
    -> SKI (VAR_NAME)
    -> + (PLUS)
    -> BIDI (VAR_NAME)
*/ 
struct Token
{
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string &v) : type(t), value(v) {}
};

class Lexer
{
public:
    Lexer(const std::string &input);
    Token GetNextToken();

private:
    std::string input;
    int pos;
    char currentChar;

    void Advance();
    void SkipWhitespace();
    std::string ReturnString();
    Token ConverseFrom();
    Token ConverseTo();
};

#endif