#include "Undefined.h"

namespace var {
    Undefined::Undefined(std::string name, VariableProtector protector)
        : Variable(std::move(name), protector) {}

    std::string Undefined::toString() {
        return "undefined";
    }
}