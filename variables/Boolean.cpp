#pragma once
#include "Variable.cpp"

namespace var {
    class Boolean : public Variable {
    public:
        bool value;
        Boolean(std::string name, VariableProtector protector, bool value) : Variable(std::move(name), &protector), value(value) {}

        std::string toString() override {
            return value ? "true" : "false";
        }
    };
}