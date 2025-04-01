#pragma once
#include <vector>
#include "../variables/Variable.cpp"
#include "../functions/Function.cpp"

class Block {
    public:
        std::vector<var::Variable> variables;
        std::vector<Function> functions;

        explicit Block() {
            variables = std::vector<var::Variable>();
            functions = std::vector<Function>();
        }
};
