#pragma once

#include <fstream>

#include "../AST/ast_nodes.h"
#include "../helpers/compile_exception.h"

class translator {
    static std::string command_translate(expression_node *node);

    static void write_header(std::ofstream &ofstream, const std::string &lib);

    static void write_source(std::ofstream &ofstream, const std::shared_ptr<expression_node> &root);

    static void write_tail(std::ofstream &ofstream);

public:
    static void generate_cpp_code(const std::shared_ptr<expression_node> &root, const std::string & lib);
};

