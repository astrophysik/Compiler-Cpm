#include "compiler.h"

compiler::compiler(const std::map<std::string, token_type>& token_type_list,
                   const std::set<char>& special_symbols, const std::map<std::string, uint16_t>& functions_arity)
    : lxr(token_type_list, special_symbols), prs(token_type_list, functions_arity) {}

void compiler::compile(const std::string & input, const std::string & output, const std::string & cpp_compiler) {
    auto tokens = lxr.get_tokens(input);
    auto root = prs.parse(tokens);
    generate_cpp_code(root);
    run(output, cpp_compiler);
}

void compiler::generate_cpp_code(const std::shared_ptr<expression_node> &root) {
    std::ofstream temp_file("temporary_cpp_code.cpp");
    if (!temp_file.is_open()) {
        throw compile_exception("Cannot create temporary file");
    }
    write_header(temp_file);
    write_source(temp_file, root);
    write_tail(temp_file);
}

std::string compiler::command_translate(expression_node *node) {
    if (auto * n = dynamic_cast<number_node *>(node)) {
        return n->number.value;
    } else if (auto * v = dynamic_cast<variable_node *>(node)) {
        return v->variable.value;
    } else if (auto * s = dynamic_cast<string_node *>(node)) {
        return "std::string(" + s->string.value + ")";
    } else if (auto * b = dynamic_cast<binary_operation_node *>(node)) {
        return command_translate(b->left.get()) + " " + b->op.value + " " + command_translate(b->right.get());
    } else if (auto * u = dynamic_cast<unary_operation_node *>(node)) {
        return u->function.value + "(" + command_translate(u->arg.get()) + ")";
    } else if (auto * sup = dynamic_cast<supplier *>(node)) {
        return sup->function.value + "()";
    }
    throw compile_exception("Undefind pointer");
}

void compiler::write_header(std::ofstream &ofstream) {
    ofstream << LIB << "\n";
    ofstream << "int main() {\n";
}

void compiler::write_source(std::ofstream &ofstream, const std::shared_ptr<expression_node> &root) {
    auto code = dynamic_cast<statement_node *>(root.get());
    for (const auto& command : code->code_lines) {
        auto cmd = '\t' + command_translate(command.get()) + ";\n";
        ofstream << cmd;
    }
}

void compiler::write_tail(std::ofstream &ofstream) {
    ofstream << "\n\treturn 0;\n}";
}

void compiler::run(const std::string &output, const std::string &cpp_compiler) {
    system((cpp_compiler + std::string(" temporary_cpp_code.cpp -std=c++17 -o ") + output).c_str());
    system((delete_file_command() + " temporary_cpp_code.cpp").c_str());
}
std::string compiler::delete_file_command() {
#ifdef _WIN32
    return "del";
#else
    return "rm";
#endif
}
