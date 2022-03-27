#pragma once

#include <utility>

#include "lexer.h"
#include "../AST/ast_nodes.h"

class parser {
    class token_buffer {
        std::vector<token> buffer;
        int64_t pos;

    public:
        token_buffer() = default;

        token_buffer(std::vector<token>  buf) : buffer(std::move(buf)), pos(0) {}

        void open(std::vector<token> buf) {
            buffer = std::move(buf);
            pos = 0;
        }

        bool has_next() const {
            return pos < buffer.size();
        }

        bool has_prev() const {
            return pos > 0;
        }

        token next() {
            return buffer[pos++];
        }

        token look_back() {
            return buffer[pos - 1];
        }

        void dec() {
            pos--;
        }
    };

    std::map<std::string, token_type> _token_type_list;
    std::map<std::string, uint16_t> _operators_arity;
    std::set<std::string> _used_variables;
    std::set<std::string> _const_variables;
    token_buffer _src;

    std::optional<token> match(const std::vector<token_type> & expected);

    token require(const std::vector<token_type> & expected);

    void generate_exception(const std::vector<token_type> & expected);

    std::shared_ptr<expression_node> parse_expression();

    std::shared_ptr<expression_node> parse_var_assign(const std::shared_ptr<expression_node>& variable_node);

    std::shared_ptr<expression_node> parse_formula_or_function();

    std::shared_ptr<expression_node> parse_brackets();

    std::shared_ptr<expression_node> parse_formula();

    std::shared_ptr<expression_node> parse_factor();

public:
    parser(std::map<std::string, token_type> types, std::map<std::string, uint16_t> arity);

    std::shared_ptr<expression_node> parse(std::vector<token> tokens);
};
