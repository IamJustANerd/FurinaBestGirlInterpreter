#include "Parser.h"
#include <iostream>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(Token(TokenType::END, ""))
{
    currentToken = lexer.GetNextToken(); // Dapetin token dari lexer (udah aman harus e)
    // std::cout << "TOKEN DI PARSER SEKARANG: ";
    // std::cout << currentToken.value << ' ' << static_cast<int>(currentToken.type) << '\n';
}

void Parser::AdvanceToken()
{
    currentToken = lexer.GetNextToken();
    // std::cout << "Advance token to" << ' ' << currentToken.value << ' ' << static_cast<int>(currentToken.type) << '\n';
}

void Parser::Error(const std::string &message)
{
    // Just in case kalau ada error, langsung throw (debugging purpose juga)
    throw std::runtime_error("Parser Error: " + message);
}

std::unique_ptr<ASTNode> Parser::Parse()
{
    return ParseStatement();
}

std::unique_ptr<ASTNode> Parser::ParseStatement()
{
    if (currentToken.type == TokenType::CREATE)
    {
        // std::cout << "CREATE STATEMENT" << '\n';
        return ParseCreate();
    }
    else if (currentToken.type == TokenType::PRINT)
    {
        // std::cout << "PRINT STATEMENT" << '\n';
        return ParsePrint();
    }
    else if (currentToken.type == TokenType::VAR_NAME)
    {
        // Save the variable name
        Token saveToken = currentToken;
        AdvanceToken(); // Advance to the next token

        if (currentToken.type == TokenType::ASSIGN)
        {
            // Handle assignment
            // std::cout << "ASSIGN STATEMENT" << '\n';
            return ParseAssign(saveToken);
        }
        else if (currentToken.type == TokenType::CONVERSE_TO)
        {
            // std::cout << "CONVERSE STATEMENT" << '\n';
            // Handle conversion
            return ParseConversion(saveToken);
        }
        else
        {
            Error("Expected ASSIGN or CONVERSE_TO after variable name.");
        }
    }

    Error("Unknown command.");
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::ParseCreate()
{
    // Siapkan temp statement untuk nampung statement ini
    std::unique_ptr<ASTNode> temp = std::make_unique<ASTNode>(currentToken);
    
    AdvanceToken(); // Skip BUAT

    // Kalau tipe token setelahnya bukan variable, return error
    if (currentToken.type != TokenType::VAR_NAME)
    {
        Error("Expected variable name after 'BUAT'.");
        return nullptr;
    }

    // Masukkan nama variable nya ke child sebelah kiri
    temp->left = std::make_unique<ASTNode>(currentToken);

    // Advance ke token selanjutnya (untuk command yang akan datang)
    AdvanceToken(); 

    // Kembalikan statement ini
    return temp;
}

std::unique_ptr<ASTNode> Parser::ParseAssign(Token saveToken)
{
    // Siapkan temp statement untuk nampung statement ini (harusnya currentToken assign atau "ADALAH")
    std::unique_ptr<ASTNode> temp = std::make_unique<ASTNode>(currentToken);

    AdvanceToken(); // SKIP "ADALAH"

    // Kalau token selanjutnya bukan value dari variable, return error
    if (currentToken.type != TokenType::CONVERSE_FROM)
    {
        Error("Expected value after ADALAH.");
        return nullptr;
    }

    // Masukkan nama variable nya ke child sebelah kiri
    temp->left = std::make_unique<ASTNode>(saveToken);

    // Masukkan value dari variable nya ke child sebelah kanan
    temp->right = std::make_unique<ASTNode>(currentToken);

    // Advance ke token selanjutnya (untuk command yang akan datang)
    AdvanceToken();

    // Kembalikan statement ini
    return temp;
}

std::unique_ptr<ASTNode> Parser::ParsePrint()
{
    // Siapkan temp statement untuk nampung statement ini
    std::unique_ptr<ASTNode> temp = std::make_unique<ASTNode>(currentToken);

    AdvanceToken(); // SKIP "TERIAK"

    // Kalau setelahnya bukan nama variable, artinya salah input (error)
    if (currentToken.type != TokenType::VAR_NAME)
    {
        Error("Setelah 'TERIAK', harusnya diisi nama variable sayang :3");
    }

    // Masukkan nama variable nya ke child sebelah kiri
    temp->left = std::make_unique<ASTNode>(currentToken);

    // Advance ke token selanjutnya (untuk command yang akan datang)
    AdvanceToken();

    return temp;
}

std::unique_ptr<ASTNode> Parser::ParseConversion(Token saveToken)
{
    // Siapkan temp statement untuk nampung statement ini (harusnya currentToken adalah tipe konversi)
    std::unique_ptr<ASTNode> temp = std::make_unique<ASTNode>(currentToken);

    // Advance ke token selanjutnya (untuk command yang akan datang)
    AdvanceToken();

    // Masukkan nama variable nya ke child sebelah kiri
    temp->left = std::make_unique<ASTNode>(saveToken);

    return temp;
}

std::vector<std::unique_ptr<ASTNode>> Parser::GetStatements()
{
    std::vector<std::unique_ptr<ASTNode>> statements;

    while(currentToken.type != TokenType::END)
    {
        statements.push_back(Parse());
    }

    return statements;
}