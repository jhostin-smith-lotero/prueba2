#include "StringUtils.h"

#include <algorithm>
#include <cctype>
#include <locale>
#include <unordered_map>

namespace util {

std::string trim(const std::string& text) {
    if (text.empty()) {
        return {};
    }
    auto begin = text.begin();
    while (begin != text.end() && std::isspace(static_cast<unsigned char>(*begin))) {
        ++begin;
    }
    if (begin == text.end()) {
        return {};
    }
    auto end = text.end();
    do {
        --end;
    } while (std::isspace(static_cast<unsigned char>(*end)) && end != begin);
    return std::string(begin, end + 1);
}

std::string toUpper(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    return result;
}

static char normalizeChar(unsigned char c) {
    static const std::unordered_map<unsigned char, char> accentMap = {
        {0xE1, 'A'}, {0xE9, 'E'}, {0xED, 'I'}, {0xF3, 'O'}, {0xFA, 'U'},
        {0xC1, 'A'}, {0xC9, 'E'}, {0xCD, 'I'}, {0xD3, 'O'}, {0xDA, 'U'},
        {0xF1, 'N'}, {0xD1, 'N'}
    };
    auto it = accentMap.find(c);
    if (it != accentMap.end()) {
        return it->second;
    }
    if (std::isalnum(c)) {
        return static_cast<char>(std::toupper(c));
    }
    return '\0';
}

std::string normalizarClave(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for (unsigned char c : text) {
        char normalized = normalizeChar(c);
        if (normalized != '\0') {
            result.push_back(normalized);
        }
    }
    return result;
}

}
