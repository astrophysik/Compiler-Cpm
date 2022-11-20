#pragma once

#include <memory>
#include <vector>
#include <list>

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

struct ctype_node : expression_node {
    token ctype;

    ctype_node(token _type);
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

struct variable_declaration : expression_node {
    token modifer;
    std::shared_ptr<variable_node> variable;
    std::shared_ptr<expression_node> value;

    variable_declaration(token mod, std::shared_ptr<variable_node> var, std::shared_ptr<expression_node> val);
};

struct ffi_arg : expression_node {
    std::shared_ptr<ctype_node> arg_type;
    std::shared_ptr<variable_node> arg_name;

    ffi_arg(std::shared_ptr<ctype_node> _arg_type, std::shared_ptr<variable_node> _arg_name);
};

struct ffi_func_decl : expression_node {
    std::shared_ptr<ctype_node> return_type;
    std::shared_ptr<variable_node> func_name;
    std::list<std::shared_ptr<ffi_arg>> args;

    ffi_func_decl(std::shared_ptr<ctype_node> _return_type, std::shared_ptr<variable_node> _func_name, std::list<std::shared_ptr<ffi_arg>> _args);
};
