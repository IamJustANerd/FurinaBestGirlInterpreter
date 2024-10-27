#include "../Parser.h"
#include "../Lexer.h"
#include <iostream>
#include <memory>

// Function to print the AST
void PrintAST(const std::unique_ptr<ASTNode> &node, int depth = 0)
{
    if (!node)
        return;

    // Print the current node with indentation based on depth
    std::cout << depth << ' ' << node->curValue.value << " (Type: " << static_cast<int>(node->curValue.type) << ")\n";

    // Recursively print the left and right children
    PrintAST(node->left, depth + 1);
    PrintAST(node->right, depth + 1);
}

void TestParser(const std::string &input)
{
    Lexer lexer(input);
    Parser parser(lexer);

    try
    {
        // Parse the input and get the AST
        std::unique_ptr<ASTNode> ast = parser.Parse();

        // Print the resulting AST
        std::cout << "AST for input \"" << input << "\":\n";
        PrintAST(ast);
    }
    catch (const std::exception &e)
    {
        std::cout << "Error: " << e.what() << '\n';
    }
}

int main()
{
    std::cout << "Test Case 1:" << '\n';
    TestParser("BUAT SKIBIDI");

    std::cout << "Test Case 2:" << '\n';
    TestParser("SKIBIDI ADALAH @BINER[100]");

    std::cout << "Test Case 3:" << '\n';
    TestParser("SKIBIDI KE OKTAL");

    std::cout << "Test Case 4:" << '\n';
    TestParser("TERIAK SKIBIDI");

    return 0;
}