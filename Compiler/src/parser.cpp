#include "parser.h"

parser::parser(std::map<std::string, token_type> types, std::map<std::string, uint16_t> arity)
    : token_type_list(std::move(types)), operators_arity(std::move(arity)) {}

std::shared_ptr<expression_node> parser::parse(std::vector<token> tokens) {
    src.open(std::move(tokens));
    std::shared_ptr<expression_node> command = parse_expression();
    require({token_type_list.at("semicolon")});
    return command;
    std::shared_ptr<expression_node> root(new statement_node());
    while (src.has_next()) {
        std::shared_ptr<expression_node> command = parse_expression();
        require({token_type_list.at("semicolon")});
        return command;
        dynamic_cast<statement_node *>(root.get())->add_node(command);
    }
    return root;
}

std::shared_ptr<expression_node> parser::parse_expression() {
    if (match({token_type_list.at("modifier")})) {
        src.dec();
        auto mod = match({token_type_list.at("modifier")});
        token variable = require({token_type_list.at("variable")});
        if (used_variables.find(variable.value) != used_variables.end()) {
            throw compile_exception("Multiply declaration of variable \"" + variable.value + "\"");
        }
        used_variables.insert(variable.value);
        if (mod->value == "val") {
            const_variables.insert(variable.value);
        }
        variable.value = mod->value + " " + variable.value;
        return parse_var_assign(std::shared_ptr<expression_node>(new variable_node(variable)));
    } else if (match({token_type_list.at("variable")})) {
        src.dec();
        token var = require({token_type_list.at("variable")});
        if (used_variables.find(var.value) != used_variables.end() &&
            const_variables.find(var.value) == const_variables.end()) {
            src.dec();
            return parse_var_assign(parse_factor());
        } else if (const_variables.find(var.value) != const_variables.end()) {
            throw compile_exception("You cannot modify variable \"" + var.value + "\" because its was declared as const");
        } else {
            throw compile_exception("You cannot use variable \"" + var.value +"\" before its declaration");
        }
    } else {
        auto function_node = parse_formula_or_function();
        return function_node;
    }
}

std::shared_ptr<expression_node> parser::parse_var_assign(const std::shared_ptr<expression_node> &variable_node) {
    auto var_name = dynamic_cast<struct variable_node *>(variable_node.get())->variable.value;
    token assignment = require({token_type_list.at("assign")});
    auto right_formula = parse_formula_or_function();
    return std::shared_ptr<expression_node>(new binary_operation_node(assignment, variable_node, right_formula));
}

std::shared_ptr<expression_node> parser::parse_formula_or_function() {
    auto current = match({token_type_list.at("function")});
    if (current) {
        require({token_type_list.at("lbracket")});
        if (match({token_type_list.at("rbracket")})) {
            if (operators_arity.at(current->value) == 0) {
                return std::shared_ptr<expression_node>(new supplier(current.value()));
            } else {
                throw compile_exception("function " + current->value + " cannot start without args");
            }
        } else {
            auto func =  std::shared_ptr<expression_node>(new unary_operation_node(current.value(), parse_formula()));
            require({token_type_list.at("rbracket")});
            return func;
        }
    } else {
        return parse_formula();
    }
}

std::shared_ptr<expression_node> parser::parse_brackets() {
    if (match({token_type_list.at("lbracket")})) {
        std::shared_ptr<expression_node> expr = parse_formula();
        require({token_type_list.at("rbracket")});
        return expr;
    } else {
        return parse_factor();
    }
}

std::shared_ptr<expression_node> parser::parse_formula() {
    auto left = parse_brackets();
    auto op = match({token_type_list.at("operator")});
    while (op) {
        auto right = parse_brackets();
        left = std::shared_ptr<expression_node>(new binary_operation_node(op.value(), left, right));
        op = match({token_type_list.at("operator")});
    }
    return left;
}

std::shared_ptr<expression_node> parser::parse_factor() {
    token current = src.next();
    if (current.type.name == "number") {
        return std::shared_ptr<expression_node>(new number_node(current));
    } else if (current.type.name == "variable") {
        if (used_variables.find(current.value) == used_variables.end()) {
            throw compile_exception("You cannot use variable \"" + current.value +"\" before its declaration");
        }
        return std::shared_ptr<expression_node>(new variable_node(current));
    } else if (current.type.name == "string") {
        return std::shared_ptr<expression_node>(new string_node(current));
    }
    else {
        generate_exception({token_type_list.at("number"), token_type_list.at("string"), token_type_list.at("variable")});
        return {};
    }
}

std::optional<token> parser::match(const std::vector<token_type> &expected) {
    if (src.has_next()) {
        token current = src.next();
        if (std::find(expected.begin(), expected.end(), current.type) != expected.end()) {
            return {current};
        } else {
            src.dec();
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
    while (src.has_prev() && src.look_back().type != token_type_list.at("semicolon")) {
        src.dec();
        count++;
    }
    for (int64_t i = 0; i < count; ++i) {
        error_command += src.next().value + " ";
    }
    std::ostringstream oss;
    std::copy(expected.begin(), expected.end(), std::ostream_iterator<token_type>(oss, "or "));
    throw compile_exception("In command\n" + error_command + "expected " + oss.str().substr(0, oss.str().size() - 3));
}
