#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <variant>
#include "../utils/Tree.h"
#include "./Block.h"
#include "../variables/Variable.h"

using var::VariableType;

class Handler {
public:
    Block block;
    explicit Handler(std::vector<Tree*> &nodes);

private:
    VariableType handleLine(Tree& node);
};