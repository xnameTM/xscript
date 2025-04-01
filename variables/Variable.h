#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <memory>
#include <sstream>

namespace var {

    // Deklaracje struktur
    struct NaN {
        bool operator==(const NaN&) const { return true; }
        std::string toString() const {
            return "NaN";
        }
    };

    struct Infinity {
    public:
        bool isPositive = true;
        explicit Infinity(bool isPositive = true) : isPositive(isPositive) {}
        bool operator==(const Infinity&) const { return true; }
        std::string toString() const {
            return isPositive ? "Infinity" : "-Infinity";
        }
    };

    // Typedefs i aliasy
    using std::string;
    using std::holds_alternative;
    using VariableType = std::variant<string, float, bool, void*, NaN, Infinity>;

    // Deklaracje funkcji pomocniczych
    bool isNumeric(const std::string &str);
    float convertToFloat(const string &str);
    string convertToString(const VariableType &value);

    // Enumeracja dla ochrony zmiennych
    enum class VariableProtector {
        CONSTANT,
        MUTABLE
    };

    // Deklaracje wyprzedzające klas pochodnych

    // Główna klasa Variable
    class Variable {
    public:
        const string name;
        const VariableProtector protector = VariableProtector::MUTABLE;
        
        Variable(string name, VariableProtector protector) : name(std::move(name)), protector(protector) {}
        virtual string toString() = 0;
        virtual ~Variable() = default;

        bool isProtected() const;

        static VariableType add(const VariableType &value, const VariableType &other);
        static VariableType subtract(VariableType &value, VariableType &other);
        static VariableType multiply(VariableType &value, VariableType &other);
        static VariableType divide(VariableType &value, VariableType &other);
        static std::unique_ptr<Variable> getVariable(string name, VariableProtector protector, VariableType value);

    private:
        static VariableType calculate_division(float f1, float f2);
        static VariableType multiply_infinity(Infinity i, VariableType other);
    };
}
