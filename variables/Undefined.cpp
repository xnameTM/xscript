#pragma once
#include "Variable.cpp"

namespace var {
    class Undefined : public Variable {
    public:
        explicit Undefined(std::string name, VariableProtector protector) : Variable(std::move(name), &protector) {}

        std::string toString() override {
            return "undefined";
        }
    };
}