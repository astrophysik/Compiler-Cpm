#include <gtest/gtest.h>
#include <queue>

#include "src/parser.h"
#include "src/lexer.h"

const std::map<std::string, token_type> token_type_list = {{"number", token_type("number", "[+-]?([0-9]*[.]?[0-9]+|[0-9]+[.]?[0-9]*)")},
                                                     {"string", token_type("string", R"("[^"]*")")},
                                                     {"variable", token_type("variable", "^[a-zA-Z_$][a-zA-Z_$0-9]*$")},
                                                     {"function", token_type("function", "print|input")},
                                                     {"ctype", token_type("ctype", "(int|char|void|double|float)\\s*[*]*")},
                                                     {"assign", token_type("assign", "[=]")},
                                                     {"operator", token_type("operator", "[+-]")},
                                                     {"lbracket", token_type("lbracket", "[(]")},
                                                     {"rbracket", token_type("rbracket", "[)]")},
                                                     {"modifier", token_type("modifier", "var|val")},
                                                     {"comma", token_type("comma", "[,]")},
                                                     {"semicolon", token_type("semicolon", "[;]")}};
const std::set<char> special_symbols = {'=', '+', '-', ';', '(', ')', ',', ' ', '\n', '\t'};
const std::map<std::string, uint16_t> functions_arity = {{"print", 1}, {"input", 0}};
const std::map<std::string, bool> function_value = {{"print", 0}, {"input", 1}};
const std::string tmp_file = "tmp_file.cpm";


std::shared_ptr<statement_node> parse_to_ast(const std::string & src) {
    std::queue<std::vector<token>> queue;

    std::ofstream file(tmp_file);
    file << src << std::endl;
    file.close();

    lexer simple_lexer(token_type_list, special_symbols);
    simple_lexer.open(tmp_file);
    auto tokens = simple_lexer.next_command();
    while (!tokens.empty()) {
        queue.push(std::move(tokens));
        tokens = simple_lexer.next_command();
    }

    parser simple_parser(token_type_list, functions_arity, function_value);
    std::shared_ptr<statement_node> root = std::make_shared<statement_node>();
    while (!queue.empty()) {
        tokens = queue.front();
        queue.pop();
        root->add_node(simple_parser.parse_statement(std::move(tokens)));
    }

    simple_lexer.close();
    return root;
}

bool is_err(const std::string & src) {
    std::shared_ptr<statement_node> root;
    bool error = false;
    try {
        root = parse_to_ast(src);
    } catch (compile_exception &e) {
        std::cout << e.what() << "\n";
        error = true;
    }
    return error;
}

TEST(SimpleAstTests, ast_base_test) {
    std::shared_ptr<statement_node> root;
    std::string src;

    src = "1;";
    root = parse_to_ast(src);
    ASSERT_EQ(root->expressions.size(), 1);
    auto *n = dynamic_cast<number_node *>(root->expressions.begin()->get());
    ASSERT_TRUE(n);
    ASSERT_EQ(n->number.value, "1");

    src = "42.42;";
    root = parse_to_ast(src);
    ASSERT_EQ(root->expressions.size(), 1);
    auto *d = dynamic_cast<number_node *>(root->expressions.begin()->get());
    ASSERT_TRUE(d);
    ASSERT_EQ(d->number.value, "42.42");
}

TEST(SimpleAstTests, ast_operation_test) {
    std::shared_ptr<statement_node> root;
    std::string src;

    src = "1 + 1;";
    root = parse_to_ast(src);
    ASSERT_EQ(root->expressions.size(), 1);
    auto *bop = dynamic_cast<binary_operation_node *>(root->expressions.begin()->get());
    ASSERT_TRUE(bop);
    auto *lhs = dynamic_cast<number_node *>(bop->left.get());
    auto *rhs = dynamic_cast<number_node *>(bop->left.get());
    ASSERT_TRUE(lhs && rhs);
    ASSERT_EQ(lhs->number.value, "1");
    ASSERT_EQ(bop->op.value, "+");
    ASSERT_EQ(rhs->number.value, "1");

    src = "1 - (9 - 8);";
    root = parse_to_ast(src);
    ASSERT_EQ(root->expressions.size(), 1);
    auto *hop = dynamic_cast<binary_operation_node *>(root->expressions.begin()->get());
    ASSERT_TRUE(bop);
    auto *a = dynamic_cast<number_node *>(hop->left.get());
    auto *b = dynamic_cast<binary_operation_node *>(hop->right.get());
    auto *c = dynamic_cast<number_node *>(b->left.get());
    auto *d = dynamic_cast<number_node *>(b->right.get());
    ASSERT_TRUE(a && b && c && d);
    ASSERT_EQ(a->number.value, "1");
    ASSERT_EQ(c->number.value, "9");
    ASSERT_EQ(d->number.value, "8");
}

TEST(SimpleAstTests, ast_variable_test) {
    std::shared_ptr<statement_node> root;
    std::string src;

    src = "a;";
    ASSERT_TRUE(is_err(src));

    src = "val a = ;";
    ASSERT_TRUE(is_err(src));

    src = "a;";
    ASSERT_TRUE(is_err(src));

    src = "val a = +;";
    ASSERT_TRUE(is_err(src));

    src = "val a = 1;";
    ASSERT_FALSE(is_err(src));

    src = "var a = 1;";
    ASSERT_FALSE(is_err(src));

    src = "val a = 1;\n"
          "a = 10;";
    ASSERT_TRUE(is_err(src));

    src = "var a = 1;\n"
          "a = 42;";
    ASSERT_FALSE(is_err(src));

    src = "var a = 1;"
          "a = 42;"
          "var b = a;"
          "b = \"4242\";";
    ASSERT_FALSE(is_err(src));
}

TEST(SimpleAstTests, ast_function_test) {
    std::shared_ptr<statement_node> root;
    std::string src;

    src = "print(\"Hello, World!\");";
    root = parse_to_ast(src);

    auto *f = dynamic_cast<function_call *>(root->expressions.begin()->get());
    ASSERT_TRUE(f);
    auto *arg = dynamic_cast<string_node *>(f->args.begin()->get());
    ASSERT_TRUE(arg);

    ASSERT_EQ(f->function.value, "print");
    ASSERT_EQ(arg->string.value, "\"Hello, World!\"");


    src = "input(\"Hello, Me\");";
    root = parse_to_ast(src);
    f = dynamic_cast<function_call *>(root->expressions.begin()->get());
    ASSERT_TRUE(f);
    arg = dynamic_cast<string_node *>(f->args.begin()->get());
    ASSERT_TRUE(arg);

    ASSERT_EQ(f->function.value, "input");
    ASSERT_EQ(arg->string.value, "\"Hello, Me\"");
}

TEST(SimpleAstTests, ast_errors_test) {
    std::string src;

    src = "1";
    ASSERT_TRUE(is_err(src));

    src = "a;";
    ASSERT_TRUE(is_err(src));

    src = "val a = 1 * 1;";
    ASSERT_TRUE(is_err(src));

    src = "val a = print(1);";
    ASSERT_TRUE(is_err(src));

    src = "print();";
    ASSERT_TRUE(is_err(src));
}

TEST(SimpleAstTests, ast_code_test) {
    std::string src;

    src = "val a = input(\"What is your name?\n\");"
          "print(\"Hello, \" + a);";
    ASSERT_FALSE(is_err(src));

    src = "val a = input(\"first : \");"
          "val b = input(\"second : \");"
          "val c = input(\"third : \");"
          "print(\"perimetr is : \");"
          "print(a + b + c);";
    ASSERT_FALSE(is_err(src));

    src = "val me_ = input(\"What is your name?\");"
          "val friend4242_my_best_11 = input(\"What is your friend's name?\");"
          "print(me_ + \" and \" + friend4242_my_best_11 + \" are friends!\");";
    ASSERT_FALSE(is_err(src));
}

TEST(SimpleAstTests, ast_ffi_test) {
    std::shared_ptr<statement_node> root;
    std::string src;

    src = "dsf f();";
    ASSERT_TRUE(is_err(src));

    src = "f();";
    ASSERT_TRUE(is_err(src));

    src = "int f(,);";
    ASSERT_TRUE(is_err(src));

    src = "int f(error);";
    ASSERT_TRUE(is_err(src));

    src = "void f();";
    root = parse_to_ast(src);

    auto *decl = dynamic_cast<ffi_func_decl *>(root->expressions.begin()->get());
    ASSERT_TRUE(decl);
    auto *return_type = decl->return_type.get();
    auto *func_name = decl->func_name.get();
    auto args = decl->args;
    ASSERT_EQ(return_type->ctype.value, "void");
    ASSERT_EQ(func_name->variable.value, "f");
    ASSERT_TRUE(args.empty());

    src = "double func_name111___(int c, char _gGggaa123);";
    root = parse_to_ast(src);

    decl = dynamic_cast<ffi_func_decl *>(root->expressions.begin()->get());
    ASSERT_TRUE(decl);
    return_type = decl->return_type.get();
    func_name = decl->func_name.get();
    args = decl->args;
    ASSERT_EQ(return_type->ctype.value, "double");
    ASSERT_EQ(func_name->variable.value, "func_name111___");
    ASSERT_EQ(args.size(), 2);
    ASSERT_EQ(args.begin()->get()->arg_type->ctype.value, "int");
    ASSERT_EQ(args.begin()->get()->arg_name->variable.value, "c");
    ASSERT_EQ((++args.begin())->get()->arg_type->ctype.value, "char");
    ASSERT_EQ((++args.begin())->get()->arg_name->variable.value, "_gGggaa123");

    src = "int memcpy(void ** dst, void              "
          "***      src,                 int    \tn);";
    root = parse_to_ast(src);

    decl = dynamic_cast<ffi_func_decl *>(root->expressions.begin()->get());
    ASSERT_TRUE(decl);
    return_type = decl->return_type.get();
    func_name = decl->func_name.get();
    args = decl->args;
    ASSERT_EQ(return_type->ctype.value, "int");
    ASSERT_EQ(func_name->variable.value, "memcpy");
    ASSERT_EQ(args.size(), 3);
    ASSERT_EQ(args.begin()->get()->arg_type->ctype.value, "void**");
    ASSERT_EQ(args.begin()->get()->arg_name->variable.value, "dst");
    ASSERT_EQ((++args.begin())->get()->arg_type->ctype.value, "void***");
    ASSERT_EQ((++args.begin())->get()->arg_name->variable.value, "src");
    ASSERT_EQ((++++args.begin())->get()->arg_type->ctype.value, "int");
    ASSERT_EQ((++++args.begin())->get()->arg_name->variable.value, "n");
}
