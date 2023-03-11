#pragma once

#include <optional>
#include <set>
#include <map>
#include <utility>

#include "../helpers/char_source.h"
#include "../helpers/compile_exception.h"
#include "../helpers/token.h"

class lexer {
    void skip_white_space();

    void skip_comment();

    void go_to_enter();

    std::optional<token> next_token();

    std::map<std::string, token_type> _token_type_list;
    std::set<char> _special_symbols;
    char_source _source;

public:
    lexer(std::map<std::string, token_type> tokens, std::set<char> symbols);

    void open(const std::string &file_name);

    std::vector<token> next_command();
};
