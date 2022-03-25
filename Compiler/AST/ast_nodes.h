#pragma once

#include <vector>
#include <memory>

#include "../helpers/token.h"

struct expression_node {
    virtual ~expression_node() = default;
};

struct statement_node : expression_node {
    std::vector<std::shared_ptr<expression_node>> code_lines;

    void add_node(const std::shared_ptr<expression_node>& node);
};

struct variable_node : expression_node {
    token variable;

    variable_node(token _name);
};

struct number_node : expression_node {
    token number;

    number_node(token _num);
};

struct string_node : expression_node {
    token string;

    string_node(token str);
};

struct binary_operation_node : expression_node {
    token op;
    std::shared_ptr<expression_node> left;
    std::shared_ptr<expression_node> right;

    binary_operation_node(token op,std::shared_ptr<expression_node> lft,std::shared_ptr<expression_node> rht);
};

struct unary_operation_node : expression_node {
    token function;
    std::shared_ptr<expression_node> arg;

    unary_operation_node(token func, std::shared_ptr<expression_node> _arg);
};

struct supplier : expression_node {
    token function;

    supplier(token func);
};

struct modifier_node : expression_node {
    token modifier;
    std::shared_ptr<expression_node> var;

    modifier_node(token mod, std::shared_ptr<expression_node> _var);
};
