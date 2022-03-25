#include "ast_nodes.h"

void statement_node::add_node(const std::shared_ptr<expression_node>& node) {
    code_lines.push_back(node);
}

variable_node::variable_node(token _name) : variable(std::move(_name)){}

number_node::number_node(token _num) : number(std::move(_num)) {}

string_node::string_node(token str) : string(std::move(str)) {}

binary_operation_node::binary_operation_node(token op,std::shared_ptr<expression_node> lft,std::shared_ptr<expression_node> rht)
    : op(std::move(op)), left(std::move(lft)), right(std::move(rht)) {}

unary_operation_node::unary_operation_node(token func, std::shared_ptr<expression_node> _arg)
    : function(std::move(func)), arg(std::move(_arg)) {}

supplier::supplier(token func) : function(std::move(func)) {}

modifier_node::modifier_node(token mod, std::shared_ptr<expression_node> _var) : modifier(std::move(mod)), var(std::move(_var)) {}
