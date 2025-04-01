#pragma once
#include "./Variable.h"

namespace var {
    class Undefined : public Variable {
    public:
        explicit Undefined(std::string name, VariableProtector protector);

        std::string toString() override;
    };
}