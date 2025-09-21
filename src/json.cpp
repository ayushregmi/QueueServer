#include "json.h"

const std::string &JSON::as_string() const
{
    if (type == Type::String)
        return str_value;
    else
        throw std::runtime_error("Not a string");
}

int JSON::as_int() const
{
    if (type == Type::Integer)
    {
        return int_value;
    }
    else
    {
        throw std::runtime_error("Not an int");
    }
}

double JSON::as_double() const
{
    if (type == Type::Double)
    {
        return double_value;
    }
    else
    {
        throw std::runtime_error("Not a double");
    }
}

bool JSON::as_bool() const
{
    if (type == Type::Double)
    {
        return bool_value;
    }
    else
    {
        throw std::runtime_error("Not a boolean");
    }
}

const std::vector<JSON> &JSON::as_array() const
{
    if (type == Type::Array)
    {
        return array_value;
    }
    else
    {
        throw std::runtime_error("Not an array");
    }
}

const std::unordered_map<std::string, JSON> &JSON::as_object() const
{
    if (type == Type::Object)
    {
        return object_value;
    }
    else
    {
        throw std::runtime_error("Not an object");
    }
}

JSON &JSON::operator[](const std::string &key)
{
    if (type == Type::Null)
    {
        type = Type::Object;
    }

    if (type != Type::Object)
    {
        throw std::runtime_error("Json value is not an object");
    }
    return object_value[key];
}

JSON::operator std::string() const
{
    return this->dump();
}

const JSON &JSON::operator[](const std::string &key) const
{
    if (type != Type::Object)
    {
        throw std::runtime_error("Json value is not an object");
    }
    auto it = object_value.find(key);
    if (it == object_value.end())
    {
        throw std::out_of_range("Key not found in JSON object");
    }
    return it->second;
}

std::string JSON::dump() const
{
    std::string r;
    switch (type)
    {
    case Type::Null:
        return "null";
    case Type::Integer:
        return std::to_string(int_value);
    case Type::Boolean:
        return bool_value ? "True" : "False";
    case Type::String:
        return "\"" + str_value + "\"";
    case Type::Double:
        return std::to_string(double_value);
    case Type::Array:
        r = "[";
        for (size_t i = 0; i < array_value.size(); i++)
        {
            r += array_value[i].dump();
            if (i + 1 != array_value.size())
                r += ",";
        }
        return r + ']';
    case Type::Object:
        r = "{";
        for (auto i = object_value.begin(); i != object_value.end(); i++)
        {
            r += "\"" + i->first + "\" : " + i->second.dump();
            if (std::next(i) != object_value.end())
                r += ",";
        }
        return r + "}";
    }
    return "";
}

JSON JSON::loads(const std::string &json_str)
{
    std::istringstream stream(json_str);
    return parse_value(stream);
}

JSON JSON::parse_value(std::istringstream &stream)
{

    char c;
    stream >> std::skipws >> c;

    if (c == 'n' || c == 't' || c == 'f')
    {
        std::string n = "";
        n += c;
        while (stream.get(c))
        {
            n += c;
            if (c == '[' || c == ']' || c == '}' || c == '{' || c == '"' || c == '\'' || c == '\\')
            {
                throw std::runtime_error("Invalid Token at: " + n);
            }
        }

        if (n == "true")
        {
            return JSON(true);
        }
        else if (n == "false")
        {
            return JSON(false);
        }
        else if (n == "null")
        {
            return JSON();
        }
        throw std::runtime_error("Invalid Expression: " + n);
    }

    if (c == '"')
    {
        return parse_string(stream);
    }

    if (c == '[')
    {
        std::vector<JSON> r;
        stream >> std::skipws >> c;
        if (c == ']')
            return JSON(r);
        stream.putback(c);

        while (true)
        {
            r.push_back(parse_value(stream));
            stream >> std::skipws >> c;
            if (c == ']')
                break;
            if (c != ',')
                throw std::runtime_error("Expected , in array");
        }
        return JSON(r);
    }

    if (c == '{')
    {
        std::unordered_map<std::string, JSON> r;
        stream >> std::skipws >> c;
        if (c == '}')
            return JSON(r);
        stream.putback(c);

        while (true)
        {
            stream >> std::skipws >> c;
            if (c != '"')
                throw std::runtime_error("Expected string key in object");
            std::string key = parse_string(stream);
            stream >> std::skipws >> c;
            if (c != ':')
                throw std::runtime_error("Expected ':' after key");
            JSON value = parse_value(stream);
            r[key] = value;

            stream >> std::skipws >> c;
            if (c == '}')
                break;
            if (c != ',')
                throw std::runtime_error("Expected ',' in object");
        }

        return JSON(r);
    }

    stream.putback(c);
    double num;
    stream >> num;
    if (stream.fail())
        throw std::runtime_error("Invalid number");
    if (num == static_cast<int>(num))
    {
        return JSON(static_cast<int>(num));
    }
    return JSON(num);
}

std::string JSON::parse_string(std::istringstream &stream)
{
    std::string r;
    char c;
    while (stream.get(c))
    {
        if (c == '"')
        {
            break;
        }
        if (c == '\\')
            stream.get(c);
        r += c;
    }
    return r;
}

std::ostream &operator<<(std::ostream &os, const JSON &json)
{
    os << json.dump();
    return os;
}