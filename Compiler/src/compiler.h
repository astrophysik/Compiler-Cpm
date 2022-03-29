#pragma once

#include <thread>
#include <utility>
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#endif


#include "../helpers/lib.h"
#include "../helpers/pipe.h"
#include "parser.h"
#include "translator.h"

class compiler {
    lexer _lxr;
    parser _prs;
    pipe<std::vector<token>> _pipe;
    pipe<std::exception_ptr> _errors;

    static void run(const std::string &output, const std::string &cpp_compiler);

    void lexer_thread();

    void parser_thread(std::shared_ptr<statement_node> &root);

    void check_exception();

public:
    compiler(const std::map<std::string, token_type> &token_type_list,
             const std::set<char> &special_symbols, const std::map<std::string, uint16_t> &functions_arity);

    void compile(const std::string &input, const std::string &output, const std::string &cpp_compiler);
};
