#pragma once

#include <optional>
#include <set>
#include <utility>

#include "../helpers/char_source.h"
#include "../helpers/compile_exception.h"
#include "../helpers/token.h"

class lexer {
    void skip_white_space();

    std::optional<token> next_token();

    std::map<std::string, token_type> token_type_list;
    char_source source;
    std::set<char> special_symbols;

public:
    lexer(std::map<std::string, token_type> tokens, std::set<char> symbols);

    //todo Не заргужать в оперативку
    std::vector<token> get_tokens(const std::string &file_name);
};
