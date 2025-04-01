#include "Null.h"

namespace var {
    Null::Null(std::string name, VariableProtector protector) 
        : Variable(std::move(name), protector) {}

    std::string Null::toString() {
        return "null";
    }
}