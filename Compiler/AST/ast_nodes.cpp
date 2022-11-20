#include "ast_nodes.h"

void statement_node::add_node(const std::shared_ptr<expression_node> &node) {
    expressions.push_back(node);
}

variable_node::variable_node(token _name) : variable(std::move(_name)) {}

ctype_node::ctype_node(token _type) : ctype(std::move(_type)) {}

number_node::number_node(token _num) : number(std::move(_num)) {}

string_node::string_node(token str) : string(std::move(str)) {}

binary_operation_node::binary_operation_node(token op, std::shared_ptr<expression_node> lft,
                                             std::shared_ptr<expression_node> rht)
        : op(std::move(op)), left(std::move(lft)), right(std::move(rht)) {}

function_call::function_call(token func, std::vector<std::shared_ptr<expression_node>> _args)
        : function(std::move(func)), args(std::move(_args)) {}

variable_declaration::variable_declaration(token mod, std::shared_ptr<variable_node> var,
                                           std::shared_ptr<expression_node> val)
        : modifer(std::move(mod)), variable(std::move(var)), value(std::move(val)) {}

ffi_arg::ffi_arg(std::shared_ptr<ctype_node> _arg_type, std::shared_ptr<variable_node> _arg_name) : arg_type(
        std::move(_arg_type)), arg_name(std::move(_arg_name)) {}

ffi_func_decl::ffi_func_decl(std::shared_ptr<ctype_node> _return_type, std::shared_ptr<variable_node> _func_name,
                             std::list<std::shared_ptr<ffi_arg>> _args) : return_type(
        std::move(_return_type)), func_name(std::move(_func_name)), args(std::move(_args)) {}