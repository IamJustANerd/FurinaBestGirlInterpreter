#ifndef PROGRAM_H
#define PROGRAM_H

#include "Parser.h"
#include <vector>
#include <memory>

class Program
{
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    // Default constructor
    Program() = default;

    // Move constructor
    Program(Program &&other) noexcept : statements(std::move(other.statements)) {}

    // Move assignment operator
    Program &operator=(Program &&other) noexcept
    {
        if (this != &other)
        {
            statements = std::move(other.statements);
        }
        return *this;
    }

    // Delete copy constructor and copy assignment operator
    Program(const Program &) = delete;
    Program &operator=(const Program &) = delete;
};

#endif