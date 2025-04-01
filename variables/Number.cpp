#pragma once
#include "Variable.cpp"
#include <variant>

namespace var {
    class Number : public Variable {
    public:
        std::variant<float, NaN> value;
        Number(std::string name, VariableProtector protector, std::variant<float, NaN> value) : Variable(std::move(name), &value, protector), value(value) {}

        std::string toString() override {
            return convertToString(&value);
        }
    };
}