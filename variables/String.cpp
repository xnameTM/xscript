#include "String.h"

namespace var {
    String::String(std::string name, std::string value, VariableProtector protector) 
        : Variable(std::move(name), protector), value(std::move(value)) {}
        
    std::string String::toString() {
        return value;
    }
}