#pragma once

#include <memory>
#include <vector>

#include "../helpers/token.h"

struct expression_node {
    virtual ~expression_node() = default;
};

// Кажется странным, что у тебя statement наследуется от expression
// когда это в целом два разных вида узла.
struct statement_node : expression_node {
    std::vector<std::shared_ptr<expression_node>> code_lines; // Наверное лучше expressions

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

struct unary_operation_node : expression_node {
    token function;
    std::shared_ptr<expression_node> arg;

    unary_operation_node(token func, std::shared_ptr<expression_node> _arg);
};

// Мне кажется странным, что ты таким образом описываешь вызов функции без аргументов.
// Лучше было бы вместо этого типа и unary_operation_node создать тип function_call,
// который бы хранил в себе функцию и список аргументов, это было бы более расширяемо.
struct supplier : expression_node {
    token function;

    supplier(token func);
};

// Судя по всему это не используемый код, его лучше не оставлять.
struct modifier_node : expression_node {
    token modifier;
    std::shared_ptr<expression_node> var;

    modifier_node(token mod, std::shared_ptr<expression_node> _var);
};
