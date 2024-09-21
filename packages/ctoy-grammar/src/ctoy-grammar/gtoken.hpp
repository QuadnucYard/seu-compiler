#pragma once

#include <string>
#include <variant>

namespace comp {

using std::string;

struct GToken {
    enum { INT, ID, OP, SEP, DIR, TAG, CHAR, STRING, RE, ACT, PRO, EPI, END } type;

    std::variant<int, char, string> val;

    int _int() const { return std::get<int>(val); }

    char _char() const { return std::get<char>(val); }

    string _string() const { return std::get<string>(val); }
};

} // namespace comp
