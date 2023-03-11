#pragma once

#include <utility>

#include "../AST/ast_nodes.h"
#include "lexer.h"

class parser {
    class token_buffer {
        std::vector<token> _buffer;
        uint32_t _pos;

    public:
        token_buffer() = default;

        token_buffer(std::vector<token> buf) : _buffer(std::move(buf)), _pos(0) {}

        void open(std::vector<token> buf) {
            _buffer = std::move(buf);
            _pos = 0;
        }

        bool has_next() const {
            return _pos < _buffer.size();
        }

        bool has_prev() const {
            return _pos > 0;
        }

        token next() {
            return _buffer[_pos++];
        }

        token look_back() {
            return _buffer[_pos - 1];
        }

        void dec() {
            _pos--;
        }

        uint32_t pos() {
            return _pos;
        }
    };

    std::map<std::string, token_type> _token_type_list;
    std::map<std::string, uint16_t> _operators_arity;
    std::map<std::string, bool> _function_value;
    std::map<std::string, bool> _defined_variables;
    token_buffer _src;

    std::optional<token> match(const std::vector<token_type> &expected);

    token require(const std::vector<token_type> &expected);

    void generate_exception(const std::vector<token_type> &expected);

    std::shared_ptr<expression_node> parse_expression();

    std::shared_ptr<expression_node> parse_var_assign(const std::shared_ptr<expression_node> &variable_node);

    std::shared_ptr<expression_node> parse_function();

    std::vector<std::shared_ptr<expression_node>> parse_function_args(const token & function);

    std::shared_ptr<expression_node> parse_brackets();

    std::shared_ptr<expression_node> parse_formula();

    std::shared_ptr<expression_node> parse_factor();

public:
    parser(std::map<std::string, token_type> types, std::map<std::string, uint16_t> arity, std::map<std::string, bool> function_value);

    std::shared_ptr<expression_node> parse_statement(std::vector<token> tokens);
};
