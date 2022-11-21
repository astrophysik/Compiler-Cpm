#include "parser.h"

#include <memory>

parser::parser(std::map<std::string, token_type> types, std::map<std::string, uint16_t> arity,
               std::map<std::string, bool> function_value)
        : _token_type_list(std::move(types)), _operators_arity(std::move(arity)),
          _function_value(std::move(function_value)) {}

std::shared_ptr<expression_node> parser::parse_statement(std::vector<token> tokens) {
    _src.open(std::move(tokens));
    std::shared_ptr<expression_node> command = parse_expression();
    require({_token_type_list.at("semicolon")});
    return command;
}

std::shared_ptr<expression_node> parser::parse_expression() {
    std::vector<token_type> expected = {_token_type_list.at("modifier"), _token_type_list.at("variable"),
                                        _token_type_list.at("function"), _token_type_list.at("ctype"),
                                        _token_type_list.at("number")};
    auto current = require(expected);
    if (current.type.name == "modifier") {
        token variable = require({_token_type_list.at("variable")});
        if (_defined_variables.find(variable.value) != _defined_variables.end()) {
            throw compile_exception("Multiply declaration of variable \"" + variable.value + "\"");
        }
        _defined_variables.insert({variable.value, current.value == "val"});
        token assignment = require({_token_type_list.at("assign")});
        return std::shared_ptr<expression_node>(
                new variable_declaration(current, std::make_shared<variable_node>(variable), parse_formula()));
    } else if (current.type.name == "variable") {
        if (_defined_variables.find(current.value) != _defined_variables.end() &&
            !_defined_variables.at(current.value)) {
            _src.dec();
            return parse_var_assign(parse_factor());
        } else if (_defined_variables.find(current.value) != _defined_variables.end() &&
                   _defined_variables.at(current.value)) {
            throw compile_exception(
                    "You cannot modify variable \"" + current.value + "\" because its was declared as const");
        } else {
            throw compile_exception("You cannot use variable \"" + current.value + "\" before its declaration");
        }
    } else if (current.type.name == "ctype") {
        token func_name = require({_token_type_list.at("variable")});
        require({_token_type_list.at("lbracket")});
        auto args = parse_ffi_args();
        require({_token_type_list.at("rbracket")});
        return std::shared_ptr<expression_node>(
                new ffi_func_decl(std::make_shared<ctype_node>(current), std::make_shared<variable_node>(func_name),
                                  args));
    } else if (current.type.name == "number") {
        _src.dec();
        return parse_formula();
    } else {
        _src.dec();
        auto function_node = parse_function();
        return function_node;
    }
}

std::list<std::shared_ptr<ffi_arg>> parser::parse_ffi_args() {
    std::vector<token_type> expected = {_token_type_list.at("ctype"), _token_type_list.at("rbracket")};
    auto current = require(expected);
    if (current.type.name == "rbracket") {
        _src.dec();
        return {};
    }
    _src.dec();
    return parse_ffi_non_empty_args();
}

std::list<std::shared_ptr<ffi_arg>> parser::parse_ffi_non_empty_args() {
    auto arg_type = require({_token_type_list.at("ctype")});
    auto arg_name = match({_token_type_list.at("variable")});
    if (!arg_name) {
        arg_name = token("", _token_type_list.at("variable"), arg_type.pos + 1);
    }
    auto comma = match({_token_type_list.at("comma")});
    std::list<std::shared_ptr<ffi_arg>> tail;
    if (comma) {
        tail = parse_ffi_non_empty_args();
    }
    auto head = std::make_shared<ffi_arg>(
            std::make_shared<ctype_node>(arg_type), std::make_shared<variable_node>(arg_name.value()));
    tail.push_front(head);
    return tail;
}

std::shared_ptr<expression_node> parser::parse_var_assign(const std::shared_ptr<expression_node> &variable_node) {
    auto var_name = dynamic_cast<struct variable_node *>(variable_node.get())->variable.value;
    token assignment = require({_token_type_list.at("assign")});
    auto right_formula = parse_formula();
    return std::shared_ptr<expression_node>(new binary_operation_node(assignment, variable_node, right_formula));
}

std::shared_ptr<expression_node> parser::parse_function() {
    auto current = require({_token_type_list.at("function")});
    return std::shared_ptr<expression_node>(new function_call(current, parse_function_arg(current)));
}

std::vector<std::shared_ptr<expression_node>> parser::parse_function_arg(const token &function) {
    require({_token_type_list.at("lbracket")});
    std::vector<std::shared_ptr<expression_node>> args;
    while (!match({_token_type_list.at("rbracket")})) {
        args.push_back(parse_formula());
    }
    if (_operators_arity.at(function.value) > args.size()) {
        throw compile_exception("Wrong count of arguments for function '" + function.value + "'");
    }
    return args;
}

std::shared_ptr<expression_node> parser::parse_brackets() {
    if (match({_token_type_list.at("lbracket")})) {
        std::shared_ptr<expression_node> expr = parse_formula();
        require({_token_type_list.at("rbracket")});
        return expr;
    } else if (match({_token_type_list.at("function")})) {
        _src.dec();
        auto func = parse_function();
        std::string func_name = dynamic_cast<struct function_call *>(func.get())->function.value;
        if (!_function_value.at(func_name)) {
            throw compile_exception(func_name + " hasn't return type");
        }
        return func;
    } else {
        return parse_factor();
    }
}

std::shared_ptr<expression_node> parser::parse_formula() {
    auto left = parse_brackets();
    auto op = match({_token_type_list.at("operator")});
    while (op) {
        auto right = parse_brackets();
        left = std::shared_ptr<expression_node>(new binary_operation_node(op.value(), left, right));
        op = match({_token_type_list.at("operator")});
    }
    return left;
}

std::shared_ptr<expression_node> parser::parse_factor() {
    token current = _src.next();
    if (current.type.name == "number") {
        return std::shared_ptr<expression_node>(new number_node(current));
    } else if (current.type.name == "variable") {
        if (_defined_variables.find(current.value) == _defined_variables.end()) {
            throw compile_exception("You cannot use variable \"" + current.value + "\" before its declaration");
        }
        return std::shared_ptr<expression_node>(new variable_node(current));
    } else if (current.type.name == "string") {
        return std::shared_ptr<expression_node>(new string_node(current));
    } else {
        generate_exception(
                {_token_type_list.at("number"), _token_type_list.at("string"), _token_type_list.at("variable")});
        return {};
    }
}

std::optional<token> parser::match(const std::vector<token_type> &expected) {
    if (_src.has_next()) {
        token current = _src.next();
        if (std::find(expected.begin(), expected.end(), current.type) != expected.end()) {
            return {current};
        } else {
            _src.dec();
        }
    }
    return {};
}

token parser::require(const std::vector<token_type> &expected) {
    auto current = match(expected);
    if (!current) {
        generate_exception(expected);
    }
    return current.value();
}

void parser::generate_exception(const std::vector<token_type> &expected) {
    std::string error_command;
    uint32_t token_pos = _src.pos();
    while (_src.has_prev()) {
        _src.dec();
    }
    uint32_t char_pos = 1;
    for (uint32_t i = 0; i < token_pos; ++i) {
        auto t = _src.next();
        error_command += std::string(t.pos - char_pos, ' ') + t.value;
        char_pos = t.pos + t.value.size();
    }
    std::ostringstream oss;
    std::copy(expected.begin(), expected.end(), std::ostream_iterator<token_type>(oss, "or "));
    throw compile_exception("In command\n" + error_command + " expected " + oss.str().substr(0, oss.str().size() - 3));
}
