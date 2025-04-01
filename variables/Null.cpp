#pragma once
#include "Variable.cpp"

namespace var {
    class Null : public Variable {
    public:
        explicit Null(std::string name, VariableProtector protector) : Variable(std::move(name), &protector) {}

        std::string toString() override {
            return "null";
        }
    };
}