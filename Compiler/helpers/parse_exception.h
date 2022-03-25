#pragma once

#include <exception>
#include <string>

struct parse_exception : std::exception {
    explicit parse_exception(std::string str) : msg(std::move(str)) {}

    const char * what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};