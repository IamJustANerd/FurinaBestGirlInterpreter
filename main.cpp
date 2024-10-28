#include <iostream>
#include <fstream>
#include <string>
#include "Lexer.h"       
#include "Parser.h"      
#include "Interpreter.h" 
#include "Program.h"

// Buat buka file
std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <type_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    try
    {
        // Baca file
        std::string fileContent = readFile(filename);

        // Buat ngambil token
        Lexer lexer(fileContent);

        // Buat ngubah token jadi statements
        Parser parser(lexer);

        // Program literally cuman vector yang berisi statements
        Program program;
        program.statements = parser.GetStatements();

        // Interpret program
        Interpreter interpreter;
        interpreter.ExecuteProgram(program);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}