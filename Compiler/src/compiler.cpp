#include "compiler.h"

#include <memory>

compiler::compiler(const std::map<std::string, token_type> &token_type_list,
                   const std::set<char> &special_symbols, const std::map<std::string, uint16_t> &functions_arity)
    : _lxr(token_type_list, special_symbols), _prs(token_type_list, functions_arity) {}

void compiler::compile(const std::string &input, const std::string &output, const std::string &cpp_compiler) {
    _lxr.open(input);
    std::thread lexer_worker(&compiler::lexer_thread, this);
    std::shared_ptr<statement_node> root = std::make_shared<statement_node>();
    std::thread parser_worker(&compiler::parser_thread, this, std::ref(root));
    lexer_worker.join();
    parser_worker.join();
    check_exception();
    translator::generate_cpp_code(root, LIB);
    run(output, cpp_compiler);
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
