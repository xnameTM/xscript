#pragma once
#include <string>
#include <cctype>
#include <algorithm>

inline std::string ltrim(const std::string &str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    return std::string(start, str.end());
}

inline std::string rtrim(const std::string &str) {
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();
    return std::string(str.begin(), end);
}

inline std::string trim(const std::string &str) {
    return rtrim(ltrim(str));
}