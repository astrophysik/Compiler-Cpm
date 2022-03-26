#pragma once

#include <utility>

#include "parser.h"
#include "../helpers/lib.h"

//todo добавить многопоточность?
class compiler {
    lexer lxr;
    parser prs;

    std::string command_translate(expression_node * node);

    static void write_header(std::ofstream &ofstream);

    void write_source(std::ofstream &ofstream, const std::shared_ptr<expression_node>& root);

    static void write_tail(std::ofstream &ofstream);

    void generate_cpp_code(const std::shared_ptr<expression_node>& root);

    inline static std::string delete_file_command();

    void run(const std::string & output, const std::string & cpp_compiler);

public:
    compiler(const std::map<std::string, token_type>& token_type_list,
             const std::set<char>& special_symbols, const std::map<std::string, uint16_t>& functions_arity);

    void compile(const std::string & input, const std::string & output, const std::string & cpp_compiler);
};
