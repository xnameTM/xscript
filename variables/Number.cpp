#include "Number.h"

namespace var {
    Number::Number(std::string name, std::variant<float, NaN, Infinity> value, VariableProtector protector)
        : Variable(std::move(name), protector), value(value) {}

    std::string Number::toString() {
        return convertToString(&value);
    }
}