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
        // Step 1: Read the type file
        std::string fileContent = readFile(filename);

        // Step 2: Create a lexer and tokenize the input
        Lexer lexer(fileContent);

        // Step 3: Create a parser and parse the tokens
        Parser parser(lexer);
        Program program;
        program.statements = parser.GetStatements(); // Assuming parse() returns a Program object

        // Step 4: Create an interpreter and execute the program
        Interpreter interpreter;
        interpreter.ExecuteProgram(program); // Assuming execute() takes a Program object
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}