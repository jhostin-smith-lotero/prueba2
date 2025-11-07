#include "Json.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace util {

namespace {
[[noreturn]] void throwError(const std::string& message, std::size_t pos) {
    std::ostringstream oss;
    oss << "Error de JSON en la posición " << pos << ": " << message;
    throw std::runtime_error(oss.str());
}
}

double JsonValue::asNumber(double def) const {
    if (auto value = std::get_if<double>(&data_)) {
        return *value;
    }
    if (auto value = std::get_if<std::string>(&data_)) {
        try {
            return std::stod(*value);
        } catch (...) {
            return def;
        }
    }
    return def;
}

int JsonValue::asInt(int def) const {
    return static_cast<int>(asNumber(static_cast<double>(def)));
}

bool JsonValue::asBool(bool def) const {
    if (auto value = std::get_if<bool>(&data_)) {
        return *value;
    }
    if (auto number = std::get_if<double>(&data_)) {
        return *number != 0.0;
    }
    if (auto text = std::get_if<std::string>(&data_)) {
        if (*text == "true" || *text == "TRUE" || *text == "True") {
            return true;
        }
        if (*text == "false" || *text == "FALSE" || *text == "False") {
            return false;
        }
    }
    return def;
}

const std::string& JsonValue::asString(const std::string& def) const {
    if (auto value = std::get_if<std::string>(&data_)) {
        return *value;
    }
    static std::string fallback;
    fallback = def;
    return fallback;
}

const JsonValue::array_t& JsonValue::asArray() const {
    if (!isArray()) {
        throw std::runtime_error("El valor JSON no es un arreglo");
    }
    return std::get<array_t>(data_);
}

const JsonValue::object_t& JsonValue::asObject() const {
    if (!isObject()) {
        throw std::runtime_error("El valor JSON no es un objeto");
    }
    return std::get<object_t>(data_);
}

const JsonValue& JsonValue::at(const std::string& key) const {
    if (!isObject()) {
        throw std::runtime_error("Acceso a clave en un valor que no es objeto");
    }
    const auto& obj = std::get<object_t>(data_);
    auto it = obj.find(key);
    if (it == obj.end()) {
        static JsonValue nullValue;
        return nullValue;
    }
    return it->second;
}

const JsonValue& JsonValue::operator[](std::size_t index) const {
    if (!isArray()) {
        throw std::runtime_error("Acceso por índice en un valor que no es arreglo");
    }
    const auto& arr = std::get<array_t>(data_);
    if (index >= arr.size()) {
        static JsonValue nullValue;
        return nullValue;
    }
    return arr[index];
}

JsonValue JsonValue::parse(const std::string& text) {
    std::size_t pos = 0;
    auto value = parseValue(text, pos);
    skipWhitespace(text, pos);
    if (pos != text.size()) {
        throwError("Se encontraron caracteres adicionales después del JSON válido", pos);
    }
    return value;
}

JsonValue JsonValue::parseFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + path);
    }
    std::ostringstream oss;
    oss << in.rdbuf();
    return parse(oss.str());
}

JsonValue JsonValue::parseValue(const std::string& text, std::size_t& pos) {
    skipWhitespace(text, pos);
    if (pos >= text.size()) {
        throwError("Se esperaba un valor JSON", pos);
    }
    char c = text[pos];
    if (c == '{') {
        return parseObject(text, pos);
    }
    if (c == '[') {
        return parseArray(text, pos);
    }
    if (c == '"') {
        return parseString(text, pos);
    }
    if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
        return parseNumber(text, pos);
    }
    if (text.compare(pos, 4, "true") == 0) {
        pos += 4;
        return JsonValue(true);
    }
    if (text.compare(pos, 5, "false") == 0) {
        pos += 5;
        return JsonValue(false);
    }
    if (text.compare(pos, 4, "null") == 0) {
        pos += 4;
        return JsonValue(nullptr);
    }
    throwError("Caracter inesperado", pos);
}

JsonValue JsonValue::parseObject(const std::string& text, std::size_t& pos) {
    object_t obj;
    ++pos; // skip '{'
    skipWhitespace(text, pos);
    if (pos < text.size() && text[pos] == '}') {
        ++pos;
        return JsonValue(obj);
    }
    while (pos < text.size()) {
        skipWhitespace(text, pos);
        if (text[pos] != '"') {
            throwError("Se esperaba una clave de cadena", pos);
        }
        JsonValue key = parseString(text, pos);
        skipWhitespace(text, pos);
        if (pos >= text.size() || text[pos] != ':') {
            throwError("Se esperaba ':' después de la clave", pos);
        }
        ++pos; // skip ':'
        JsonValue value = parseValue(text, pos);
        obj.emplace(key.asString(), std::move(value));
        skipWhitespace(text, pos);
        if (pos >= text.size()) {
            throwError("Se esperaba ',' o '}'", pos);
        }
        if (text[pos] == '}') {
            ++pos;
            break;
        }
        if (text[pos] != ',') {
            throwError("Se esperaba ',' entre pares", pos);
        }
        ++pos; // skip ','
    }
    return JsonValue(obj);
}

JsonValue JsonValue::parseArray(const std::string& text, std::size_t& pos) {
    array_t arr;
    ++pos; // skip '['
    skipWhitespace(text, pos);
    if (pos < text.size() && text[pos] == ']') {
        ++pos;
        return JsonValue(arr);
    }
    while (pos < text.size()) {
        JsonValue value = parseValue(text, pos);
        arr.emplace_back(std::move(value));
        skipWhitespace(text, pos);
        if (pos >= text.size()) {
            throwError("Se esperaba ',' o ']'", pos);
        }
        if (text[pos] == ']') {
            ++pos;
            break;
        }
        if (text[pos] != ',') {
            throwError("Se esperaba ',' entre elementos", pos);
        }
        ++pos;
    }
    return JsonValue(arr);
}

JsonValue JsonValue::parseString(const std::string& text, std::size_t& pos) {
    if (text[pos] != '"') {
        throwError("Se esperaba comillas dobles", pos);
    }
    ++pos; // skip '"'
    std::string result;
    while (pos < text.size()) {
        char c = text[pos++];
        if (c == '"') {
            break;
        }
        if (c == '\\') {
            if (pos >= text.size()) {
                throwError("Secuencia de escape incompleta", pos);
            }
            char esc = text[pos++];
            switch (esc) {
                case '"': result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                case '/': result.push_back('/'); break;
                case 'b': result.push_back('\b'); break;
                case 'f': result.push_back('\f'); break;
                case 'n': result.push_back('\n'); break;
                case 'r': result.push_back('\r'); break;
                case 't': result.push_back('\t'); break;
                case 'u': {
                    if (pos + 4 > text.size()) {
                        throwError("Secuencia unicode incompleta", pos);
                    }
                    std::string hex = text.substr(pos, 4);
                    pos += 4;
                    char16_t code = static_cast<char16_t>(std::stoi(hex, nullptr, 16));
                    if (code <= 0x7F) {
                        result.push_back(static_cast<char>(code));
                    } else {
                        // basic UTF-8 encoding
                        if (code <= 0x7FF) {
                            result.push_back(static_cast<char>(0xC0 | ((code >> 6) & 0x1F)));
                            result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                        } else {
                            result.push_back(static_cast<char>(0xE0 | ((code >> 12) & 0x0F)));
                            result.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
                            result.push_back(static_cast<char>(0x80 | (code & 0x3F)));
                        }
                    }
                    break;
                }
                default:
                    throwError("Secuencia de escape desconocida", pos);
            }
        } else {
            result.push_back(c);
        }
    }
    return JsonValue(result);
}

JsonValue JsonValue::parseNumber(const std::string& text, std::size_t& pos) {
    std::size_t start = pos;
    if (text[pos] == '-') {
        ++pos;
    }
    while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) {
        ++pos;
    }
    if (pos < text.size() && text[pos] == '.') {
        ++pos;
        while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) {
            ++pos;
        }
    }
    if (pos < text.size() && (text[pos] == 'e' || text[pos] == 'E')) {
        ++pos;
        if (pos < text.size() && (text[pos] == '+' || text[pos] == '-')) {
            ++pos;
        }
        while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) {
            ++pos;
        }
    }
    double value = std::stod(text.substr(start, pos - start));
    return JsonValue(value);
}

JsonValue JsonValue::parseLiteral(const std::string& text, std::size_t& pos, const std::string& literal, JsonValue value) {
    if (text.compare(pos, literal.size(), literal) != 0) {
        throwError("Literal inesperado", pos);
    }
    pos += literal.size();
    return value;
}

void JsonValue::skipWhitespace(const std::string& text, std::size_t& pos) {
    while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos]))) {
        ++pos;
    }
}

} // namespace util
