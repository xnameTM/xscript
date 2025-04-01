#include "Function.h"

Function::Function() : name(""), parameters() {}

Function::Function(std::string name) : name(std::move(name)), parameters() {}

Function::Function(std::string name, std::vector<std::string> parameters) :
    name(std::move(name)), parameters(std::move(parameters)) {}

var::VariableType Function::execute(std::vector<var::VariableType>& args) {
    // Domyślna implementacja - do nadpisania w klasach pochodnych
    return nullptr;
}