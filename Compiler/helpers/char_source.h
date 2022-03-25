#pragma once

#include <fstream>

#include "compile_exception.h"

struct char_source {
    char_source() = default;
    char_source(const std::string& file_name);

    char next();

    void open(const std::string& file_name);

    void back();

    bool has_next();

    size_t pos() const;

private:
    long long position = 1;
    std::ifstream ifstream;
};
