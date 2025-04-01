#pragma once
#include "./Variable.h"
#include <variant>

namespace var {
    class Number : public Variable {
    public:
        std::variant<float, NaN, Infinity> value;
        Number(std::string name, std::variant<float, NaN, Infinity> value, VariableProtector protector);

        std::string toString() override;
    };
}