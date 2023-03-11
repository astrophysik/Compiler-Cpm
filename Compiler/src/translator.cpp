#include "translator.h"


std::string translator::command_translate(expression_node *node) {
    if (auto *n = dynamic_cast<number_node *>(node)) {
        return n->number.value;
    } else if (auto *v = dynamic_cast<variable_node *>(node)) {
        return v->variable.value;
    } else if (auto *s = dynamic_cast<string_node *>(node)) {
        return "std::string(" + s->string.value + ")";
    } else if (auto *b = dynamic_cast<binary_operation_node *>(node)) {
        return command_translate(b->left.get()) + " " + b->op.value + " " + command_translate(b->right.get());
    } else if (auto *f = dynamic_cast<function_call *>(node)) {
        std::string command = f->function.value + "(";
        for (const auto &arg : f->args) {
            command += command_translate(arg.get());
        }
        command += ")";
        return command;
    } else if (auto *d = dynamic_cast<variable_declaration *>(node)) {
        return d->modifer.value + " " + d->variable->variable.value + " = " + command_translate(d->value.get()) + ";";
    }
    throw compile_exception("Undefined pointer");
}

void translator::write_header(std::ofstream &ofstream, const std::string &include) {
    ofstream << include << "\n";
    ofstream << "int main() {\n";
}

void translator::write_source(std::ofstream &ofstream, const std::shared_ptr<statement_node> &root) {
    auto code = dynamic_cast<statement_node *>(root.get());
    for (const auto &command : code->expressions) {
        auto cmd = '\t' + command_translate(command.get()) + ";\n";
        ofstream << cmd;
    }
}

void translator::write_tail(std::ofstream &ofstream) {
    ofstream << "\n\treturn 0;\n}";
}

void translator::generate_cpp_code(const std::shared_ptr<statement_node> &root, const std::string &include) {
    std::ofstream temp_file("temporary_cpp_code.cpp");
    if (!temp_file.is_open()) {
        throw compile_exception("Cannot create temporary file");
    }
    write_header(temp_file, include);
    write_source(temp_file, root);
    write_tail(temp_file);
}
