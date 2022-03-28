#include "lexer.h"

lexer::lexer(std::map<std::string, token_type> tokens, std::set<char> symbols)
    : _token_type_list(std::move(tokens)), _special_symbols(std::move(symbols)) {}

void lexer::open(const std::string &file_name) {
    _source.open(file_name);
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

void lexer::skip_comment() {
    if (_source.next() != '/') {
        _source.back();
        return;
    }
    char ch = _source.next();
    if (ch == '/') {
        go_to_enter();
    } else {
        size_t pos = _source.lines_pos();
        if (ch == '\n') {
            pos--;
        }
        throw compile_exception("\n" + std::to_string(pos) + " | One slash instead of two expected\n");
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
        return {};
    }
    char ch = _source.next();
    bool is_string = ch == '\"';
    std::string current;
    if (_special_symbols.find(ch) == _special_symbols.end()) {
        while (_source.has_next()) {
            current += ch;
            ch = _source.next();
            if (!is_string && _special_symbols.find(ch) != _special_symbols.end()) {
                if (ch != ' ' && ch != '\n' && ch != '\t') {
                    _source.back();
                }
                break;
            }
            if (is_string && ch == '\"') {
                current += ch;
                break;
            }
        }
    } else {
        if (ch != ' ' && ch != '\n' && ch != '\t') {
            current = {ch};
        }
    }
    auto it = std::find_if(_token_type_list.begin(), _token_type_list.end(), [&current](const auto &type) {
        return std::regex_match(current, type.second.regex);
    });
    if (it == _token_type_list.end()) {
        throw compile_exception("\n" + std::to_string(_source.lines_pos()) + " | Invalid language token '" + current + "'\n");
    }
    return {token(current, it->second)};
}
