#pragma once

#include <string>
#include <unordered_map>

namespace comp {
template <typename T = std::string>
using dict = std::unordered_map<std::string, T>;

std::string unescape_regex(std::string_view s);

struct wildcard_matcher {
    using matcher_type = bool (*)(int);

    static bool dot(int c) { return c != '\n'; }

    static bool d(int c) { return isdigit(c); }

    static bool D(int c) { return !isdigit(c); }

    static bool w(int c) { return isalnum(c); }

    static bool W(int c) { return !isalnum(c); }

    static bool s(int c) { return isspace(c); }

    static bool S(int c) { return !isspace(c); }

    static matcher_type get(int c) {
        switch (c) {
        case '.': return wildcard_matcher::dot;
        case 'd': return wildcard_matcher::d;
        case 'D': return wildcard_matcher::D;
        case 's': return wildcard_matcher::s;
        case 'S': return wildcard_matcher::S;
        case 'w': return wildcard_matcher::w;
        case 'W': return wildcard_matcher::W;
        default:  return nullptr;
        }
    }
};

} // namespace comp
