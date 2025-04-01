#pragma once
#include "./Variable.h"

namespace var {
    class Null : public Variable {
    public:
        explicit Null(std::string name, VariableProtector protector);

        std::string toString() override;
    };
}