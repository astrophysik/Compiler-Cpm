#include "ast_nodes.h"

void statement_node::add_node(const std::shared_ptr<expression_node> &node) {
    expressions.push_back(node);
}

variable_node::variable_node(token _name) : variable(std::move(_name)) {}

number_node::number_node(token _num) : number(std::move(_num)) {}

string_node::string_node(token str) : string(std::move(str)) {}

binary_operation_node::binary_operation_node(token op, std::shared_ptr<expression_node> lft, std::shared_ptr<expression_node> rht)
    : op(std::move(op)), left(std::move(lft)), right(std::move(rht)) {}

function_call::function_call(token func, std::vector<std::shared_ptr<expression_node>> _args)
    : function(std::move(func)), args(std::move(_args)) {}

variable_declaration::variable_declaration(token mod, std::shared_ptr<variable_node> var, std::shared_ptr<expression_node> val)
    : modifer(std::move(mod)), variable(std::move(var)), value(std::move(val)) {}