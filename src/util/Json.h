#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

namespace util {

class JsonValue {
public:
    using object_t = std::map<std::string, JsonValue>;
    using array_t = std::vector<JsonValue>;
    using value_t = std::variant<std::nullptr_t, bool, double, std::string, array_t, object_t>;

    JsonValue() : data_(nullptr) {}
    explicit JsonValue(value_t value) : data_(std::move(value)) {}

    bool isNull() const { return std::holds_alternative<std::nullptr_t>(data_); }
    bool isBool() const { return std::holds_alternative<bool>(data_); }
    bool isNumber() const { return std::holds_alternative<double>(data_); }
    bool isString() const { return std::holds_alternative<std::string>(data_); }
    bool isArray() const { return std::holds_alternative<array_t>(data_); }
    bool isObject() const { return std::holds_alternative<object_t>(data_); }

    double asNumber(double def = 0.0) const;
    int asInt(int def = 0) const;
    bool asBool(bool def = false) const;
    const std::string& asString(const std::string& def = "") const;
    const array_t& asArray() const;
    const object_t& asObject() const;

    const JsonValue& at(const std::string& key) const;
    const JsonValue& operator[](const std::string& key) const { return at(key); }
    const JsonValue& operator[](std::size_t index) const;

    static JsonValue parse(const std::string& text);
    static JsonValue parseFile(const std::string& path);

private:
    static JsonValue parseValue(const std::string& text, std::size_t& pos);
    static JsonValue parseObject(const std::string& text, std::size_t& pos);
    static JsonValue parseArray(const std::string& text, std::size_t& pos);
    static JsonValue parseString(const std::string& text, std::size_t& pos);
    static JsonValue parseNumber(const std::string& text, std::size_t& pos);
    static JsonValue parseLiteral(const std::string& text, std::size_t& pos, const std::string& literal, JsonValue value);
    static void skipWhitespace(const std::string& text, std::size_t& pos);

    value_t data_;
};

} // namespace util
