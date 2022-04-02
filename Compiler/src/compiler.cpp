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
#ifdef _WIN32
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::string cmdArgs = cpp_compiler + std::string(" temporary_cpp_code.cpp -std=c++17 -o ") + output;
    if (!CreateProcess(cpp_compiler.c_str(), cmdArgs.data(), 0, 0, 0, 0, 0, 0, &si, &pi)) {
        DeleteFile("temporary_cpp_code.cpp");
        throw compile_exception("Failed to start c++ compiler");
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if (!DeleteFile("temporary_cpp_code.cpp")) {
        std::cout << "Failed to delete temporary file\n";
    }
#elif __unix__
    pid_t pid;
    int status;
    fflush(NULL);
    pid = fork();
    switch (pid) {
        case -1:
            throw compile_exception("Failed to start c++ compiler");
            break;
        case 0:
            execl(cpp_compiler.c_str(), cpp_compiler.c_str(), "temporary_cpp_code.cpp", "-o", output.c_str(), "-std=c++17", NULL);
            break;
        default:
            if (waitpid(pid, &status, 0) != -1) {
                std::cout << "C++  exited with status " << status << "\n";
            }
            if (!std::remove("temporary_cpp_code.cpp")) {
                std::cout << "Failed to delete temporary file\n";
            }
            break;
    }
#else
    throw compile_exception("Your platform is unsupported. Now available Unix and Windows");
#endif
}

void compiler::lexer_thread() {
    try {
        auto tokens = _lxr.next_command();
        while (!tokens.empty()) {
            _pipe.push(tokens);
            tokens = _lxr.next_command();
        }
        _pipe.push({});
    } catch (compile_exception &e) {
        _errors.push(std::current_exception());
        _pipe.push({});
    }
}

void compiler::parser_thread(std::shared_ptr<statement_node> &root) {
    try {
        auto tokens = _pipe.pop();
        while (!tokens.empty()) {
            root->add_node(_prs.parse(tokens));
            tokens = _pipe.pop();
        }
    } catch (compile_exception &e) {
        _errors.push(std::current_exception());
    }
}

void compiler::check_exception() {
    if (!_errors.queue.empty()) {
        auto e = _errors.queue.front();
        _errors.queue.pop();
        std::rethrow_exception(e);
    }
}
