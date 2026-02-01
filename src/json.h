#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>

class JSON
{

public:
    enum class Type
    {
        Null,
        Boolean,
        Integer,
        Double,
        String,
        Array,
        Object
    };

private:
    Type type;
    bool bool_value;
    int int_value;
    double double_value;
    std::string str_value;

    std::vector<JSON> array_value;
    std::unordered_map<std::string, JSON> object_value;

public:
    JSON() : type(Type::Null) {}
    JSON(bool b) : type(Type::Boolean), bool_value(b) {}
    JSON(const char *c) : type(Type::String), str_value(c) {}
    JSON(char c) : type(Type::String), str_value(1, c) {}
    JSON(int i) : type(Type::Integer), int_value(i) {}
    JSON(double d) : type(Type::Double), double_value(d) {}
    JSON(std::string s) : type(Type::String), str_value(s) {}
    JSON(std::vector<JSON> a) : type(Type::Array), array_value(a) {}
    JSON(std::unordered_map<std::string, JSON> obj) : type(Type::Object), object_value(obj) {}

    Type get_type() { return type; }

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    const std::string &as_string() const;
    std::vector<JSON> &as_array();
    const std::unordered_map<std::string, JSON> &as_object() const;

    JSON &operator[](const std::string &key);
    operator std::string() const;
    // JSON &operator=(const JSON);
    const JSON &operator[](const std::string &key) const;

    JSON parse_nested_json(const std::string &key) const;

    std::string dump() const;
    static JSON loads(const std::string &json_str);

    bool is_null() const;
    bool contains(const std::string &key) const;

private:
    static JSON parse_value(std::istringstream &stream);
    static std::string parse_string(std::istringstream &stream);
};

std::ostream &operator<<(std::ostream &os, const JSON &json);

#endif