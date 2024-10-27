#include "Lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string &input) : input(input), pos(0)
{
    currentChar = input[pos];
}

void Lexer::Advance()
{
    pos++;
    if (pos >= input.length())
    {
        currentChar = '\0'; // End of input
    }
    else
    {
        currentChar = input[pos];
    }
}

void Lexer::SkipWhitespace()
{
    // Skip spasi, EOF, etc
    while (currentChar != '\0' && std::isspace(currentChar))
    {
        Advance();
    }
}

std::string Lexer::ReturnString()
{
    std::string result;
    while (currentChar != '\0' && std::isalnum(currentChar))
    {
        result += currentChar;
        Advance();
    }
    return result;
}

Token Lexer::ConverseFrom()
{
    Advance(); // Skip '@'

    // Ambil tipe data (Biner, Oktal, etc)
    std::string type;
    while (std::isalnum(currentChar))
    {
        type += currentChar;
        Advance();
    }

    // Ambil valuenya
    if (currentChar == '[')
    {
        Advance();
        std::string value;
        while (currentChar != ']' && currentChar != '\0')
        {
            value += currentChar;
            Advance();
        }
        if (currentChar == ']')
        {
            Advance(); // Skip ']'
        }
        return Token(TokenType::CONVERSE_FROM, type + "[" + value + "]");
    }

    return Token(TokenType::ERROR, "ERROR: Malformed @<value_type>[<value>]");
}

Token Lexer::ConverseTo()
{
    // Skip ' '
    Advance();

    std::string result;
    while (currentChar != '\0' && std::isalnum(currentChar))
    {
        result += currentChar;
        Advance();
    }
    return Token(TokenType::CONVERSE_TO, result);
}

Token Lexer::GetNextToken()
{
    // Selama belum EOF
    while (currentChar != '\0')
    {
        if (std::isspace(currentChar))
        {
            SkipWhitespace();
            continue;
        }
        if (std::isalpha(currentChar))
        {
            std::string temp = ReturnString();

            // std::cout << temp << '\n';

            // Cek apakah ini perintah atau nama variable
            if (temp == "BUAT")
            {
                // std::cout << "CREATE" << '\n';
                return Token(TokenType::CREATE, "#");
            }
            else if (temp == "ADALAH")
            {
                // std::cout << "ASSIGN" << '\n';
                return Token(TokenType::ASSIGN, "=");
            }
            else if (temp == "TERIAK")
            {
                // std::cout << "PRINT" << '\n';
                return Token(TokenType::PRINT, "~");
            }
            else if (temp == "KE")
            {
                // std::cout << "CONVERSE TO" << '\n';
                // Harusnya di depannya ini tipe bilangan
                return ConverseTo();
            }
            else
            {
                // std::cout << "NAMA" << '\n';
                return Token(TokenType::VAR_NAME, temp);
            }
        }
        if (currentChar == '@')
        {
            return ConverseFrom();
        }

        // Untuk yang tidak teridentifikasi, langsung error
        return Token(TokenType::ERROR, std::string(1, currentChar));
    }

    return Token(TokenType::END, "");
}