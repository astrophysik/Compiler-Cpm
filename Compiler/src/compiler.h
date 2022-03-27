#pragma once

#include <utility>
#include <thread>

#include "parser.h"
#include "translator.h"
#include "../helpers/lib.h"
#include "../helpers/pipe.h"

class compiler {
    lexer _lxr;
    parser _prs;
    pipe<std::vector<token>> _pipe;
    pipe<std::exception_ptr> _errors;

    inline static std::string delete_file_command();

    static void run(const std::string & output, const std::string & cpp_compiler);

    void lexer_thread() {
        try {
            auto tokens = _lxr.next_command();
            while (!tokens.empty()) {
                _pipe.push(tokens);
                tokens = _lxr.next_command();
            }
            _pipe.push({});
        } catch (compile_exception & e) {
            _errors.push(std::current_exception());
        }
    }

    void parser_thread(std::shared_ptr<statement_node> & root) {
        try {
            auto tokens = _pipe.pop();
            while (!tokens.empty()) {
                root->add_node(_prs.parse(tokens));
                tokens = _pipe.pop();
            }
        } catch (compile_exception & e) {
            _errors.push(std::current_exception());
        }
    }

    void check_exception() {
        if (!_errors.queue.empty()) {
            auto e = _errors.queue.front();
            _errors.queue.pop();
            std::rethrow_exception(e);
        }
    }

public:
    compiler(const std::map<std::string, token_type>& token_type_list,
             const std::set<char>& special_symbols, const std::map<std::string, uint16_t>& functions_arity);

    void compile(const std::string & input, const std::string & output, const std::string & cpp_compiler);
};
