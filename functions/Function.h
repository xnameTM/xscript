#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../variables/Variable.h"

class Function {
public:
    std::string name;
    std::vector<std::string> parameters;
    
    Function();
    explicit Function(std::string name);
    Function(std::string name, std::vector<std::string> parameters);
    
    virtual var::VariableType execute(std::vector<var::VariableType>& args);
};