#pragma once

#include <string>
#include <variant>

namespace comp {

using std::string;

enum class GTokenKind { INT, ID, OP, SEP, DIR, TAG, CHAR, STRING, RE, ACT, PRO, EPI, END };

struct GToken {
    GTokenKind kind;

    std::variant<int, char, string> val;

    int as_int() const { return std::get<int>(val); }

    char as_char() const { return std::get<char>(val); }

    string as_string() const { return std::get<string>(val); }
};

} // namespace comp
