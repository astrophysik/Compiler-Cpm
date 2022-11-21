#include "lexer.h"

lexer::lexer(std::map<std::string, token_type> tokens, std::set<char> symbols)
    : _token_type_list(std::move(tokens)), _special_symbols(std::move(symbols)) {}

void lexer::open(const std::string &file_name) {
    _source.open(file_name);
}

void lexer::close() {
    _source.close();
}

std::vector<token> lexer::next_command() {
    std::vector<token> tokens;
    for (auto item = next_token(); item.has_value(); item = next_token()) {
        tokens.push_back(item.value());
        if (item->type == _token_type_list.at("semicolon")) {
            break;
        }
    }
    return tokens;
}

void lexer::skip_white_space() {
    while (_source.has_next()) {
        char ch = _source.next();
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            _source.back();
            break;
        }
    }
}
/* */

void lexer::skip_comment() {
    if (_source.next() != '/') {
        _source.back();
        return;
    }
    char ch = _source.next();
    if (ch == '*') {
        ch = _source.next();
        while (!(ch == '*' && _source.next() == '/')) {
            ch = _source.next();
        }
    }
}

void lexer::go_to_enter() {
    while (_source.has_next() && _source.next() != '\n') {}
}

std::optional<token> lexer::next_token() {
    skip_white_space();
    skip_comment();
    skip_white_space();
    if (!_source.has_next()) {
        return std::nullopt;
    }
    char ch = _source.next();
    bool is_string = ch == '\"';
    std::string current;
    if (_special_symbols.find(ch) == _special_symbols.end()) {
        while (_source.has_next()) {
            current += ch;
            ch = _source.next();
            if (!is_string && _special_symbols.find(ch) != _special_symbols.end()) {
                _source.back();
                break;
            }
            if (is_string && ch == '\"') {
                current += ch;
                break;
            }
        }
    } else {
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            current += ch;
        }
    }
    auto it = std::find_if(_token_type_list.begin(), _token_type_list.end(), [&current](const auto &type) {
        return std::regex_match(current, type.second.regex);
    });
    if (it == _token_type_list.end()) {
        throw compile_exception("\n" + std::to_string(_source.lines_pos()) + " | Invalid language token '" + current + "'\n");
    }
    token tkn = token(current, it->second, _source.char_pos() - current.size() + 1);
    /*hack for c type*/
    if (it->second.name == "ctype") {
        while (_source.has_next()) {
            skip_white_space();
            ch = _source.next();
            if (ch == '*') {
                tkn.value += ch;
            } else {
                _source.back();
                break;
            }
        }
    }
    return {tkn};
}
