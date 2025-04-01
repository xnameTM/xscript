#include "./string.h"
#include <cctype>
#include <algorithm>

std::string ltrim(const std::string &str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
    return std::string(start, str.end());
}

std::string rtrim(const std::string &str) {
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char ch) {
        return std::isspace(ch);
    }).base();
    return std::string(str.begin(), end);
}

std::string trim(const std::string &str) {
    return rtrim(ltrim(str));
}