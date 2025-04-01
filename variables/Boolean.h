#pragma once
#include "./Variable.h"

namespace var {
    class Boolean : public Variable {
    public:
        bool value;

        Boolean(std::string name, bool value, VariableProtector protector);

        std::string toString() override;
    };
}