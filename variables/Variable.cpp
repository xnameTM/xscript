#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include <utility>
#include <variant>
#include <sstream>
#include <memory>
#include <utility>
#include <charconv>

namespace var {
    struct NaN {
        bool operator==(const NaN&) const { return true; }
    };

    struct Infinity {
    public:
        bool isPositive = true;
        explicit Infinity(bool isPositive = true) : isPositive(isPositive) {}
        bool operator==(const Infinity&) const { return true; }
    };

    using std::string;
    using std::holds_alternative;

    using VariableType = std::variant<string, float, bool, void*, NaN, Infinity>;

    inline bool isNumeric(const std::string &str) {
        if (str.empty())
            return false;

        size_t start = 0;
        if (str[0] == '-') {
            if (str.size() == 1)
                return false;
            start = 1;
        }

        bool decimalPointFound = false;
        for (size_t i = start; i < str.size(); ++i) {
            if (str[i] == '.') {
                if (decimalPointFound)
                    return false;
                decimalPointFound = true;
            } else if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    inline float convertToFloat(const string &str) {
        if (isNumeric(str)) {
            std::istringstream iss(str);
            float f;
            iss >> f;
            return f;
        }
        throw std::runtime_error("Invalid float conversion");
    }

    inline string convertToString(const VariableType &value) {
        if (holds_alternative<string>(value)) {
            return std::get<string>(value);
        } else if (holds_alternative<float>(value)) {
            float f = std::get<float>(value);
            char buffer[32];
            auto [ptr, ec] = std::to_chars(buffer, buffer + sizeof(buffer), f, std::chars_format::general);
            if (ec == std::errc())
                return std::string(buffer, ptr);
            throw std::runtime_error("Conversion error");
        } else if (holds_alternative<NaN>(value)) {
            return "NaN";
        } else if (holds_alternative<bool>(value)) {
            return std::get<bool>(value) ? "true" : "false";
        } else if (holds_alternative<void*>(value)) {
            return "null";
        } else if (holds_alternative<Infinity>(value)) {
            return std::get<Infinity>(value).isPositive ? "Infinity" : "-Infinity";
        }
        
        return "null";
    }

    enum class VariableProtector {
        CONSTANT,
        MUTABLE
    };

    class Variable {
    public:

        const string name;
        VariableType value;
        const VariableProtector protector = VariableProtector::MUTABLE;
        explicit Variable(string name, VariableType value) : name(std::move(name)), value(std::move(value)) {}
        Variable(string name, VariableType value, VariableProtector protector) : name(std::move(name)), value(std::move(value)), protector(protector) {}
        virtual string toString() = 0;

        static VariableType add(const VariableType &value, const VariableType &other) {
            if (holds_alternative<string>(value)) {
                string s = convertToString(value);

                return s.append(convertToString(other));
            } else if (holds_alternative<float>(value)) {
                float f = std::get<float>(value);

                if (holds_alternative<string>(other))
                    return convertToString(f).append(convertToString(other));
                if (holds_alternative<float>(other))
                    return f + std::get<float>(other);;
                if (holds_alternative<bool>(other))
                    return f + (std::get<bool>(other) ? 1.f : 0.f);
                if (holds_alternative<void*>(other))
                    return f;
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return std::get<Infinity>(other);
            } else if (holds_alternative<bool>(value)) {
                bool b = std::get<bool>(value);

                if (holds_alternative<string>(other))
                    return convertToString(b).append(convertToString(other));
                if (holds_alternative<float>(other))
                    return (b ? 1.f : 0.f) + std::get<float>(other);
                if (holds_alternative<bool>(other))
                    return (b ? 1.f : 0.f) + (std::get<bool>(other) ? 1.f : 0.f);
                if (holds_alternative<void*>(other))
                    return b ? 1.f : 0.f;
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return std::get<Infinity>(other);
            } else if (holds_alternative<void*>(value)) {
                if (holds_alternative<string>(other))
                    return convertToString(value).append(convertToString(other));
                if (holds_alternative<float>(other))
                    return std::get<float>(other);
                if (holds_alternative<bool>(other))
                    return std::get<bool>(other) ? 1.f : 0.f;
                if (holds_alternative<void*>(other))
                    return 0.f;
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return std::get<Infinity>(other);
            } else if (holds_alternative<NaN>(value)) {
                if (holds_alternative<string>(other))
                    return convertToString(value).append(convertToString(other));

                return NaN();
            } else if (holds_alternative<Infinity>(value)) {
                if (holds_alternative<string>(other))
                    return convertToString(value).append(convertToString(other));

                if (holds_alternative<Infinity>(other)) {
                    Infinity i1 = std::get<Infinity>(other);
                    Infinity i2 = std::get<Infinity>(value);

                    if (i1.isPositive == i2.isPositive)
                        return Infinity(i1.isPositive);

                    return NaN();
                }

                return std::get<Infinity>(value);
            }

            return nullptr;
        }

        static VariableType subtract(VariableType &value, VariableType &other) {
            if (holds_alternative<string>(value)) {
                if (holds_alternative<string>(other)) {
                    string s1 = convertToString(value);
                    string s2 = convertToString(other);
                    if (isNumeric(s1) && isNumeric(s2))
                        return convertToFloat(s1) - convertToFloat(s2);
                    return NaN();
                }
                if (holds_alternative<float>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return convertToFloat(s) - std::get<float>(other);
                    return NaN();
                }
                if (holds_alternative<bool>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return convertToFloat(s) - (std::get<bool>(other) ? 1.f : 0.f);
                    return NaN();
                }
                if (holds_alternative<void*>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return convertToFloat(s);
                    return NaN();
                }
                if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
                if (holds_alternative<Infinity>(other)) {
                    return Infinity(!std::get<Infinity>(other).isPositive);
                }
            } else if (holds_alternative<float>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return std::get<float>(value) - convertToFloat(s);
                    return NaN();
                }
                if (holds_alternative<float>(other)) {
                    return std::get<float>(value) - std::get<float>(other);
                }
                if (holds_alternative<bool>(other)) {
                    return std::get<float>(value) - (std::get<bool>(other) ? 1.f : 0.f);
                }
                if (holds_alternative<void*>(other)) {
                    return std::get<float>(value);
                }
                if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
                if (holds_alternative<Infinity>(other)) {
                    return Infinity(!std::get<Infinity>(other).isPositive);
                }
            } else if (holds_alternative<bool>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return (std::get<bool>(value) ? 1.f : 0.f) - convertToFloat(s);
                    return NaN();
                }
                if (holds_alternative<float>(other)) {
                    return (std::get<bool>(value) ? 1.f : 0.f) - std::get<float>(other);
                }
                if (holds_alternative<bool>(other)) {
                    return (std::get<bool>(value) ? 1.f : 0.f) - (std::get<bool>(other) ? 1.f : 0.f);
                }
                if (holds_alternative<void*>(other)) {
                    return (std::get<bool>(value) ? 1.f : 0.f);
                }
                if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
                if (holds_alternative<Infinity>(other)) {
                    return Infinity(!std::get<Infinity>(other).isPositive);
                }
            } else if (holds_alternative<void*>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return -convertToFloat(s);
                    return NaN();
                }
                if (holds_alternative<float>(other)) {
                    return -std::get<float>(other);
                }
                if (holds_alternative<bool>(other)) {
                    return -(std::get<bool>(other) ? 1.f : 0.f);
                }
                if (holds_alternative<void*>(other)) {
                    return 0.f;
                }
                if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
                if (holds_alternative<Infinity>(other)) {
                    return Infinity(!std::get<Infinity>(other).isPositive);
                }
            } else if (holds_alternative<NaN>(value)) {
                return NaN();
            } else if (holds_alternative<Infinity>(value)) {
                if (holds_alternative<string>(other)) {
                    if (isNumeric(convertToString(other)))
                        return std::get<Infinity>(value);

                    return NaN();
                }

                if (holds_alternative<NaN>(other))
                    return NaN();

                if (holds_alternative<Infinity>(other)) {
                    Infinity i1 = std::get<Infinity>(value);
                    Infinity i2 = std::get<Infinity>(other);

                    if (i1.isPositive == i2.isPositive)
                        return NaN();

                    return Infinity(i1.isPositive);
                }

                return std::get<Infinity>(value);
            }

            return 0.f;
        }

        static VariableType multiply(VariableType &value, VariableType &other) {
            if (holds_alternative<Infinity>(value)) {
                return multiply_infinity(std::get<Infinity>(value), other);
            } else if (holds_alternative<Infinity>(other)) {
                return multiply_infinity(std::get<Infinity>(other), value);
            }

            if (holds_alternative<string>(value)) {
                if (holds_alternative<string>(other)) {
                    string s1 = convertToString(value);
                    string s2 = convertToString(other);
                    if (isNumeric(s1) && isNumeric(s2))
                        return convertToFloat(s1) * convertToFloat(s2);
                    return NaN();
                } else if (holds_alternative<float>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return convertToFloat(s) * std::get<float>(other);
                    return NaN();
                } else if (holds_alternative<bool>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return convertToFloat(s) * (std::get<bool>(other) ? 1.f : 0.f);
                    return NaN();
                } else if (holds_alternative<void*>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return 0.f;
                    return NaN();
                } else if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
            } else if (holds_alternative<float>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return std::get<float>(value) * convertToFloat(s);
                    return NaN();
                } else if (holds_alternative<float>(other)) {
                    return std::get<float>(value) * std::get<float>(other);
                } else if (holds_alternative<bool>(other)) {
                    return std::get<float>(value) * (std::get<bool>(other) ? 1.f : 0.f);
                } else if (holds_alternative<void*>(other)) {
                    return 0.f;
                } else if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
            } else if (holds_alternative<bool>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return (std::get<bool>(value) ? 1.f : 0.f) * convertToFloat(s);
                    return NaN();
                } else if (holds_alternative<float>(other)) {
                    return (std::get<bool>(value) ? 1.f : 0.f) * std::get<float>(other);
                } else if (holds_alternative<bool>(other)) {
                    return (std::get<bool>(value) ? 1.f : 0.f) * (std::get<bool>(other) ? 1.f : 0.f);
                } else if (holds_alternative<void*>(other)) {
                    return 0.f;
                } else if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
            } else if (holds_alternative<void*>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return 0.f;
                    return NaN();
                } else if (holds_alternative<float>(other)) {
                    return 0.f;
                } else if (holds_alternative<bool>(other)) {
                    return 0.f;
                } else if (holds_alternative<void*>(other)) {
                    return 0.f;
                } else if (holds_alternative<NaN>(other)) {
                    return NaN();
                }
            } else if (holds_alternative<NaN>(value)) {
                return NaN();
            }

            return 0.f;
        }

        static VariableType divide(VariableType &value, VariableType &other) {
            if (holds_alternative<string>(value)) {
                if (holds_alternative<string>(other)) {
                    string s1 = convertToString(value);
                    string s2 = convertToString(other);
                    if (isNumeric(s1) && isNumeric(s2))
                        return calculate_division(convertToFloat(s1), convertToFloat(s2));
                    return NaN();
                }
                if (holds_alternative<float>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return calculate_division(convertToFloat(s), std::get<float>(other));
                    return NaN();
                }
                if (holds_alternative<bool>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return calculate_division(convertToFloat(s), std::get<bool>(other) ? 1.f : 0.f);
                    return NaN();
                }
                if (holds_alternative<void*>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return calculate_division(convertToFloat(s), 0.f);
                    return NaN();
                }
                if (holds_alternative<NaN>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return NaN();
                    return NaN();
                }
                if (holds_alternative<Infinity>(other)) {
                    string s = convertToString(value);
                    if (isNumeric(s))
                        return 0.f;
                    return NaN();
                }
            } else if (holds_alternative<float>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return calculate_division(std::get<float>(value), convertToFloat(s));
                    return NaN();
                }
                if (holds_alternative<float>(other))
                    return calculate_division(std::get<float>(value), std::get<float>(other));
                if (holds_alternative<bool>(other))
                    return calculate_division(std::get<bool>(value), std::get<bool>(other) ? 1.f : 0.f);
                if (holds_alternative<void*>(other))
                    return calculate_division(std::get<float>(value), 0.f);
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return 0.f;
            } else if (holds_alternative<bool>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return calculate_division(std::get<bool>(value) ? 1.f : 0.f, convertToFloat(s));
                    return NaN();
                }
                if (holds_alternative<float>(other))
                    return calculate_division(std::get<bool>(value) ? 1.f : 0.f, std::get<float>(other));
                if (holds_alternative<bool>(other))
                    return calculate_division(std::get<bool>(value) ? 1.f : 0.f, std::get<bool>(other) ? 1.f : 0.f);
                if (holds_alternative<void*>(other))
                    return calculate_division(std::get<bool>(value) ? 1.f : 0.f, 0.f);
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return 0.f;
            } else if (holds_alternative<void*>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s))
                        return calculate_division(0.f, convertToFloat(s));
                    return NaN();
                }
                if (holds_alternative<float>(other))
                    return calculate_division(0.f, std::get<float>(other));
                if (holds_alternative<bool>(other))
                    return calculate_division(0.f, std::get<bool>(other) ? 1.f : 0.f);
                if (holds_alternative<void*>(other))
                    return NaN();
                if (holds_alternative<NaN>(other))
                    return NaN();
                if (holds_alternative<Infinity>(other))
                    return 0.f;
            } else if (holds_alternative<NaN>(value)) {
                return NaN();
            } else if (holds_alternative<Infinity>(value)) {
                if (holds_alternative<string>(other)) {
                    string s = convertToString(other);
                    if (isNumeric(s)) {
                        bool isPositive = std::get<Infinity>(value).isPositive;
                        float f = convertToFloat(s);
                        if (f == 0.f)
                            return NaN();

                        return Infinity(isPositive == (f > 0.f));
                    }
                    return NaN();
                } else if (holds_alternative<float>(other)) {
                    bool isPositive = std::get<Infinity>(value).isPositive;
                    float f = std::get<float>(other);
                    if (f == 0.f)
                        return NaN();

                    return Infinity(isPositive == (f > 0.f));
                } else if (holds_alternative<bool>(other)) {
                    bool isPositive = std::get<Infinity>(value).isPositive;
                    bool b = std::get<bool>(other);
                    if (b)
                        return Infinity(isPositive);
                    return NaN();
                } else if (holds_alternative<void*>(other)) {
                    return NaN();
                } else if (holds_alternative<NaN>(other)) {
                    return NaN();
                } else if (holds_alternative<Infinity>(other)) {
                    return NaN();
                }
            }

            return 0.f;
        }
    private:
        static VariableType calculate_division(const float f1, const float f2) {
            if (f1 == 0.f && f2 == 0.f)
                return NaN();
            if (f2 == 0.f)
                return Infinity(f1 > 0.f);
            return f1 / f2;
        }

        static VariableType multiply_infinity(Infinity i, VariableType other) {
            bool isPositive = i.isPositive;

            if (holds_alternative<string>(other)) {
                string s = convertToString(other);
                if (isNumeric(s)) {
                    float f = convertToFloat(s);
                    if (f == 0.f)
                        return NaN();
                    return Infinity(isPositive == (f > 0.f));
                }
                return NaN();
            } else if (holds_alternative<float>(other)) {
                float f = std::get<float>(other);
                if (f == 0.f)
                    return NaN();
                return Infinity(isPositive == (f > 0.f));
            } else if (holds_alternative<bool>(other)) {
                bool b = std::get<bool>(other);
                if (b)
                    return Infinity(isPositive);
                return NaN();
            } else if (holds_alternative<void*>(other)) {
                return NaN();
            } else if (holds_alternative<NaN>(other)) {
                return NaN();
            } else if (holds_alternative<Infinity>(other)) {
                Infinity i2 = std::get<Infinity>(other);
                return Infinity(i.isPositive == i2.isPositive);
            }

            return NaN();
        }
    };
}
