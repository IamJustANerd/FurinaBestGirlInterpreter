#include "Interpreter.h"
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <bitset>

// Pakai map agar bisa pakai string sebagai index (keren wak)
std::map<std::string, std::string> symbolTable;

// Fungsi untuk mendapatkan tipe bilangan dari suatu value
std::string returnType(const std::string &numberType)
{
    std::string temp = "";
    
    int i = 0;
    while(numberType[i] != '[')
    {
        temp += numberType[i];
        i += 1;
    }

    return temp;
}

// Fungsi untuk mengkonversi semua jenis ke bentuk desimal (agar enak diubah ke bentuk lain)
int convertToDecimal(const std::string &number, const std::string &base)
{
    if (base == "BINER")
    {
        return std::stoi(number, nullptr, 2); // konversi ke biner
    }
    else if (base == "OKTAL")
    {
        return std::stoi(number, nullptr, 8); // Konversi ke oktal
    }
    else if (base == "DESIMAL")
    {
        return std::stoi(number); // udah desimal :(
    }
    else if (base == "HEXADESIMAL")
    {
        return std::stoi(number, nullptr, 16); // Konversi ke hexa
    }
    else
    {
        throw std::invalid_argument("Unknown base: " + base);
    }
}

// Fungsi untuk mengkonversi nilai ke bentuk yang kita perlukan (BINER, OKTAL, DESIMAL, HEXADESIMAL)
std::string convertFromDecimal(int decimalValue, const std::string &targetBase)
{
    if (targetBase == "BINER")
    {
        // Konversi ke biner dan hilangin angka 0 di depan (agar ga kepanjangan)
        std::string binaryStr = std::bitset<32>(decimalValue).to_string(); // Ambil biner sebagai string
        // Cari angka 1 pertama, lalu buang semua 0 sebelumnya
        size_t firstOne = binaryStr.find('1');
        if (firstOne != std::string::npos)
        {
            return "BINER[" + binaryStr.substr(firstOne) + "]";
        }
        else
        {
            return "BINER[0]"; // Kalau nilainya 0
        }
    }
    else if (targetBase == "OKTAL")
    {
        std::ostringstream oss;
        oss << std::oct << decimalValue; // Konversi ke oktal
        return "OKTAL[" + oss.str() + "]";
    }
    else if (targetBase == "DESIMAL")
    {
        return "DESIMAL[" + std::to_string(decimalValue) + "]"; // udah desimal
    }
    else if (targetBase == "HEXADESIMAL")
    {
        std::ostringstream oss;
        oss << std::hex << std::uppercase << decimalValue; // konversi ke hexa
        return "HEXADESIMAL[" + oss.str() + "]";
    }
    else
    {
        throw std::invalid_argument("Unknown target base: " + targetBase);
    }
}

void Interpreter::ExecuteProgram(const Program &program)
{
    for(int i = 0; i < program.statements.size(); i++)
    {
        Interpret(program.statements[i]);
    }
}

void Interpreter::ExecuteCreate(const std::unique_ptr<ASTNode> &node)
{
    // Cek apakah AST nya benar atau tidak
    if (!node || !node->left)
        throw std::runtime_error("Invalid CREATE statement.");

    // Ambil nama variable (ada di anak di sebelah kiri)
    std::string varName = node->left->curValue.value;

    // Cek apakah nama variablenya sudah ada atau belum
    if (symbolTable.find(varName) != symbolTable.end())
        throw std::runtime_error("Variable '" + varName + "' already exists.");

    // Tambahkan index variablenya ke map
    symbolTable[varName] = "";

    // Debugging purpose
    // std::cout << "Created variable: " << varName << std::endl;
}

void Interpreter::ExecuteAssign(const std::unique_ptr<ASTNode> &node)
{
    // Cek apakah AST nya benar atau tidak
    if (!node || !node->left || !node->right)
        throw std::runtime_error("Invalid ASSIGN statement.");

    // Ambil nama variable (ada di anak di sebelah kiri)
    std::string varName = node->left->curValue.value;

    // Cek dulu apakah variablenya ada atau enggak
    if (symbolTable.find(varName) == symbolTable.end())
        throw std::runtime_error("Variable '" + varName + "' is not defined.");

    // Kalau ada, masukin nilai yang ada di child kanan ke variablenya
    std::string value = node->right->curValue.value;
    symbolTable[varName] = value;

    // Debugging purpose
    // std::cout << "Assigned value '" << value << "' to variable '" << varName << "'" << std::endl;
}

void Interpreter::ExecutePrint(const std::unique_ptr<ASTNode> &node)
{
    // Cek apakah AST nya benar atau tidak
    if (!node || !node->left)
        throw std::runtime_error("Invalid PRINT statement.");

    // Ambil nama variablenya (ada di child kiri)
    std::string varName = node->left->curValue.value;

    // Cek apakah variablenya ada atau tidak
    if (symbolTable.find(varName) == symbolTable.end())
        throw std::runtime_error("Variable '" + varName + "' is not defined.");

    // Kalau variablnye ini belum punya nilai (""), throw error
    if (symbolTable[varName] == "")
        throw std::runtime_error("Variable '" + varName + "' has no value yet.");

    // Print valuenya
    std::cout << varName << ": " << symbolTable[varName] << std::endl;
}

void Interpreter::ExecuteConversion(const std::unique_ptr<ASTNode> &node)
{
    // Cek apakah AST nya benar atau tidak
    if (!node || !node->left)
        throw std::runtime_error("Invalid CONVERSION statement.");

    // Ambil nama variablenya (ada di child kiri)
    std::string varName = node->left->curValue.value;

    // Cek apakah variable nya ada atau tidak
    if (symbolTable.find(varName) == symbolTable.end())
        throw std::runtime_error("Variable '" + varName + "' is not defined.");

    // Debugging purpose
    // std::cout << "Converting value of variable '" << varName << "' (current value: " << symbolTable[varName] << ")" << std::endl;

    std::string conversionType = node->curValue.value;

    // Konversi berdasarkan command yang diberikan
    if (conversionType == "BINER")
    {
        // Format dari value suatu variable adalah "TIPE[x]". Ekstrak nilai x
        std::string numberValue = symbolTable[varName].substr(symbolTable[varName].find('[') + 1);
        numberValue = numberValue.substr(0, numberValue.find(']'));

        // Dapatkan tipe bilangannya
        std::string numberType = returnType(symbolTable[varName]);

        // Pertama, ubah ke bentuk desimal dulu
        int decimalNumberValue = convertToDecimal(numberValue, numberType); 

        // Setelah itu, update ke bentuk yang diinginkan
        numberValue = convertFromDecimal(decimalNumberValue, "BINER");

        // Update nilai variable ke bentuk "BINER[x]"
        symbolTable[varName] = numberValue;

        // Debugging purpose
        // std::cout << "Converted '" << varName << " to " << numberValue << " for variable '" << varName << "'" << std::endl;
    }
    else if (conversionType == "OKTAL")
    {
        // Format dari value suatu variable adalah "TIPE[x]". Ekstrak nilai x
        std::string numberValue = symbolTable[varName].substr(symbolTable[varName].find('[') + 1);
        numberValue = numberValue.substr(0, numberValue.find(']'));

        // Dapatkan tipe bilangannya
        std::string numberType = returnType(symbolTable[varName]);

        // Pertama, ubah ke bentuk desimal dulu
        int decimalNumberValue = convertToDecimal(numberValue, numberType);

        // Setelah itu, update ke bentuk yang diinginkan
        numberValue = convertFromDecimal(decimalNumberValue, "OKTAL");

        // Update nilai variable ke bentuk "OKTAL[x]"
        symbolTable[varName] = numberValue;

        // Debugging purpose
        // std::cout << "Converted '" << varName << " to " << numberValue << " for variable '" << varName << "'" << std::endl;
    }
    else if (conversionType == "DESIMAL")
    {
        // Format dari value suatu variable adalah "TIPE[x]". Ekstrak nilai x
        std::string numberValue = symbolTable[varName].substr(symbolTable[varName].find('[') + 1);
        numberValue = numberValue.substr(0, numberValue.find(']'));

        // Dapatkan tipe bilangannya
        std::string numberType = returnType(symbolTable[varName]);

        // Pertama, ubah ke bentuk desimal dulu
        int decimalNumberValue = convertToDecimal(numberValue, numberType);

        // Setelah itu, update ke bentuk yang diinginkan
        numberValue = convertFromDecimal(decimalNumberValue, "DESIMAL");

        // Update nilai variable ke bentuk "DESIMAL[x]"
        symbolTable[varName] = numberValue;

        // Debugging purpose
        // std::cout << "Converted '" << varName << " to " << numberValue << " for variable '" << varName << "'" << std::endl;
    }
    else if (conversionType == "HEXADESIMAL")
    {
        // Format dari value suatu variable adalah "TIPE[x]". Ekstrak nilai x
        std::string numberValue = symbolTable[varName].substr(symbolTable[varName].find('[') + 1);
        numberValue = numberValue.substr(0, numberValue.find(']'));

        // Dapatkan tipe bilangannya
        std::string numberType = returnType(symbolTable[varName]);

        // Pertama, ubah ke bentuk desimal dulu
        int decimalNumberValue = convertToDecimal(numberValue, numberType);

        // Setelah itu, update ke bentuk yang diinginkan
        numberValue = convertFromDecimal(decimalNumberValue, "HEXADESIMAL");

        // Update nilai variable ke bentuk "HEXADESIMAL[x]"
        symbolTable[varName] = numberValue;

        // Debugging purpose
        // std::cout << "Converted '" << varName << " to " << numberValue << " for variable '" << varName << "'" << std::endl;
    }
    else
    {
        throw std::runtime_error("Unknown conversion type: " + conversionType);
    }
}