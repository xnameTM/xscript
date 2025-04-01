#include "Boolean.h"

namespace var {
    Boolean::Boolean(std::string name, bool value, VariableProtector protector) 
        : Variable(std::move(name), protector), value(value) {}

    std::string Boolean::toString() {
        return value ? "true" : "false";
    }
}