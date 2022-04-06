#pragma once

#include <fstream>

#include "../AST/ast_nodes.h"
#include "../helpers/compile_exception.h"
#include "../helpers/lib.h"

class translator {
    static std::string command_translate(expression_node *node);

    static void write_header(std::ofstream &ofstream, const std::string &include);

    static void write_source(std::ofstream &ofstream, const std::shared_ptr<statement_node> &root);

    static void write_tail(std::ofstream &ofstream);

public:
    static void generate_cpp_code(const std::shared_ptr<statement_node> &root, const std::string &include);
};
