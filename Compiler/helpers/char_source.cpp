#include "char_source.h"

char_source::char_source(const std::string &file_name) : ifstream(file_name) {}

char char_source::next() {
    char ch;
    ifstream.get(ch);
    if (ch == '\n') {
        number_of_lines++;
    }
    return ch;
}

void char_source::open(const std::string &file_name) {
    ifstream.open(file_name);
    if (!ifstream.is_open()) {
        throw compile_exception("Cannot open input file or does not have permission");
    }
}

void char_source::back() {
    ifstream.unget();
}

bool char_source::has_next() {
    return ifstream && ifstream.peek() != EOF;
}

uint32_t char_source::char_pos() {
    return ifstream.tellg();
}

uint32_t char_source::lines_pos() {
    return number_of_lines;
}
