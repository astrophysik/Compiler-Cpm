#pragma once

#include <exception>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct compile_exception : std::exception {
    explicit compile_exception(std::string str) : msg(std::move(str)) {}


    const char *what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};