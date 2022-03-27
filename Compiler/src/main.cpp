#include <iostream>
#include <cstring>
#include <map>

#include "compiler.h"

int main(int argc, char * argv[]) {
    std::string cpp_compiler = "g++";
    std::string output_file;
    if (argc < 2) {
        std::cerr << "no input file\n"
                     "use --help for help manual";
        return 0;
    } else if (std::strcmp(argv[1], "--help") == 0) {
        std::cout << "help manual\n"
                     "Usage: cpm.exe file [options]...\n"
                     "Options:\n"
                     "\t--help          Display this information.\n"
                     "\t-cmp=<src> Use your <src>. Compiler must support version from c++17. Default value <src> is g++\n"
                     "\t-o <file>       Place the output into <file>";
        return 0;
    } else {
        for (int i = 2; i < argc; ++i) {
            if (argv[i][1] == 'c') {
                cpp_compiler = std::string((argv[i] + 4), std::strlen(argv[i]));
            } else if (argv[i][1] == 'o') {
                output_file = std::string(argv[++i]);
            } else {
                std::cout << std::string("unexpected option ") + argv[i];
                return 0;
            }
        }
    }
    if (output_file.empty()) {
        output_file = "output";
    }
    try {
        std::map<std::string, token_type> token_type_list = {{"number", token_type("number", "[+-]?([0-9]*[.]?[0-9]+|[0-9]+[.]?[0-9]*)")},
                                                             {"string", token_type("string", R"("[^"]*")")},
                                                             {"variable", token_type("variable", "^[a-zA-Z_$][a-zA-Z_$0-9]*$")},
                                                             {"function", token_type("function", "print|input")},
                                                             {"assign", token_type("assign", "[=]")},
                                                             {"operator", token_type("operator", "[+-]")},
                                                             {"lbracket", token_type("lbracket", "[(]")},
                                                             {"rbracket", token_type("rbracket", "[)]")},
                                                             {"modifier", token_type("modifier", "var|val")},
                                                             {"semicolon", token_type("semicolon", "[;]")}};
        std::set<char> special_symbols = {'=', '+', '-', ';', '(', ')', ' ', '\n', '\t'};
        std::map<std::string, uint16_t> functions_arity = {{"print", 1}, {"input", 0}};
        compiler cmp(token_type_list, special_symbols, functions_arity);
        cmp.compile(argv[1], output_file, cpp_compiler);
    } catch (compile_exception &e) {
        std::cerr << e.what();
    }
    return 0;
}
