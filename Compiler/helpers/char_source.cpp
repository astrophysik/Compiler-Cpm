#include "char_source.h"

char_source::char_source(const std::string &file_name) : _ifstream(file_name) {}

char char_source::next() {
    char ch;
    _ifstream.get(ch);
    if (ch == '\n') {
        _number_of_lines++;
    }
    return ch;
}

void char_source::open(const std::string &file_name) {
    _ifstream.open(file_name);
    if (!_ifstream.is_open()) {
        throw compile_exception("Cannot open input file or does not have permission");
    }
}

void char_source::close() {
    if (_ifstream.is_open()) {
        _ifstream.close();
    }
    _number_of_lines = 1;
}

void char_source::back() {
    _ifstream.unget();
}

bool char_source::has_next() {
    return _ifstream && _ifstream.peek() != EOF;
}

uint32_t char_source::char_pos() {
    return _ifstream.tellg();
}

uint32_t char_source::lines_pos() {
    return _number_of_lines;
}
