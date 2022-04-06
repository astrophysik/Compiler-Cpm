#pragma once

#include <memory>
#include <vector>

#include "../helpers/token.h"

struct expression_node {
    virtual ~expression_node() = default;
};

struct statement_node {
    std::vector<std::shared_ptr<expression_node>> expressions;

    void add_node(const std::shared_ptr<expression_node> &node);
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

    binary_operation_node(token op, std::shared_ptr<expression_node> lft, std::shared_ptr<expression_node> rht);
};

struct function_call : expression_node {
    token function;
    std::vector<std::shared_ptr<expression_node>> args;

    function_call(token func, std::vector<std::shared_ptr<expression_node>> _args);
};
