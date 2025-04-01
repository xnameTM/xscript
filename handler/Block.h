#pragma once
#include <vector>
#include "../variables/Variable.h"
#include "../functions/Function.h"
#include "../variables/Boolean.h"
#include "../variables/Null.h"
#include "../variables/Number.h"
#include "../variables/String.h"
#include "../variables/Undefined.h"

class Block {
public:
    using BlockVariableType = std::variant<var::Boolean, var::Null, var::Number, var::String, var::Undefined>;

    std::vector<BlockVariableType> variables;
    std::vector<Function*> functions;

    explicit Block();

    var::VariableType getVariableValue(std::string name);
};
