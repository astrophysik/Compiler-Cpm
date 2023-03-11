#pragma once

#include <fstream>

#include "compile_exception.h"

struct char_source {
    char_source() = default;
    char_source(const std::string &file_name);

    char next();

    void open(const std::string &file_name);

    void back();

    bool has_next();

    uint32_t char_pos();

    uint32_t lines_pos();

private:
    uint32_t _number_of_lines = 1;
    std::ifstream _ifstream;
};
