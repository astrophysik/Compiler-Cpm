#pragma once

#include <iostream>
#include <regex>
#include <string>


struct token_type {
    std::string name;
    std::regex regex;

    token_type(std::string _name, const std::string &_regex)
        : name(std::move(_name)), regex(_regex) {}

    friend bool operator==(const token_type &lhs, const token_type &rhs) {
        return lhs.name == rhs.name;
    }

    friend bool operator!=(const token_type &lhs, const token_type &rhs) {
        return lhs.name != rhs.name;
    }

    friend std::ostream &operator<<(std::ostream &ostream, const token_type &rhs) {
        return ostream << rhs.name << " ";
    }
};

struct token {
    std::string value;
    token_type type;

    token(std::string _value, token_type _type)
        : value(std::move(_value)), type(std::move(_type)) {}
};
