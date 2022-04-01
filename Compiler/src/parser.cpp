#include "parser.h"

parser::parser(std::map<std::string, token_type> types, std::map<std::string, uint16_t> arity)
    : _token_type_list(std::move(types)), _operators_arity(std::move(arity)) {}

std::shared_ptr<expression_node> parser::parse(std::vector<token> tokens) {
    _src.open(std::move(tokens));
    std::shared_ptr<expression_node> command = parse_expression();
    require({_token_type_list.at("semicolon")});
    return command;
}

std::shared_ptr<expression_node> parser::parse_expression() {
    std::vector<token_type> expected = {_token_type_list.at("modifier"), _token_type_list.at("variable"), _token_type_list.at("function")};
    auto current = require(expected);
    if (current.type.name == "modifier") {
        token variable = require({_token_type_list.at("variable")});
        if (_used_variables.find(variable.value) != _used_variables.end()) {
            throw compile_exception("Multiply declaration of variable \"" + variable.value + "\"");
        }
        _used_variables.insert(variable.value);
        if (current.value == "val") {
            _const_variables.insert(variable.value);
        }
        variable.value = current.value + " " + variable.value;
        return parse_var_assign(std::shared_ptr<expression_node>(new variable_node(variable)));
    } else if (current.type.name == "variable") {
        if (_used_variables.find(current.value) != _used_variables.end() &&
            _const_variables.find(current.value) == _const_variables.end()) {
            _src.dec();
            return parse_var_assign(parse_factor());
        } else if (_const_variables.find(current.value) != _const_variables.end()) {
            throw compile_exception("You cannot modify variable \"" + current.value + "\" because its was declared as const");
        } else {
            throw compile_exception("You cannot use variable \"" + current.value + "\" before its declaration");
        }
    } else {
        auto function_node = parse_formula_or_function();
        return function_node;
    }
}

std::shared_ptr<expression_node> parser::parse_var_assign(const std::shared_ptr<expression_node> &variable_node) {
    auto var_name = dynamic_cast<struct variable_node *>(variable_node.get())->variable.value;
    token assignment = require({_token_type_list.at("assign")});
    auto right_formula = parse_formula_or_function();
    return std::shared_ptr<expression_node>(new binary_operation_node(assignment, variable_node, right_formula));
}

std::shared_ptr<expression_node> parser::parse_formula_or_function() {
    auto current = match({_token_type_list.at("function")});
    if (current) {
        require({_token_type_list.at("lbracket")});
        if (match({_token_type_list.at("rbracket")})) {
            if (_operators_arity.at(current->value) == 0) {
                return std::shared_ptr<expression_node>(new supplier(current.value()));
            } else {
                throw compile_exception("function " + current->value + " cannot start without args");
            }
        } else {
            auto func = std::shared_ptr<expression_node>(new unary_operation_node(current.value(), parse_formula()));
            require({_token_type_list.at("rbracket")});
            return func;
        }
    } else {
        return parse_formula();
    }
}

std::shared_ptr<expression_node> parser::parse_brackets() {
    if (match({_token_type_list.at("lbracket")})) {
        std::shared_ptr<expression_node> expr = parse_formula();
        require({_token_type_list.at("rbracket")});
        return expr;
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
        if (_used_variables.find(current.value) == _used_variables.end()) {
            throw compile_exception("You cannot use variable \"" + current.value + "\" before its declaration");
        }
        return std::shared_ptr<expression_node>(new variable_node(current));
    } else if (current.type.name == "string") {
        return std::shared_ptr<expression_node>(new string_node(current));
    } else {
        generate_exception({_token_type_list.at("number"), _token_type_list.at("string"), _token_type_list.at("variable")});
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
    int64_t count = 0;
    while (_src.has_prev() && _src.look_back().type != _token_type_list.at("semicolon")) {
        _src.dec();
        count++;
    }
    for (int64_t i = 0; i < count; ++i) {
        error_command += _src.next().value + " ";
    }
    std::ostringstream oss;
    std::copy(expected.begin(), expected.end(), std::ostream_iterator<token_type>(oss, "or "));
    throw compile_exception("In command\n" + error_command + "expected " + oss.str().substr(0, oss.str().size() - 3));
}
