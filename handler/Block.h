#pragma once
#include <vector>
#include "../variables/Variable.h"
#include "../functions/Function.h"

class Block {
public:
    std::vector<var::Variable*> variables;
    std::vector<Function*> functions;

    explicit Block();
};