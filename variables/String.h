#pragma once
#include "./Variable.h"

namespace var {
    class String : public Variable {
    public:
        std::string value;

        String(std::string name, std::string value, VariableProtector protector);

        std::string toString() override;
    };
}