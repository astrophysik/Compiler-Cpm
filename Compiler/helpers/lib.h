#pragma once

inline std::string LIB = "#include <algorithm>\n"
                         "#include <iostream>\n"
                         "#include <variant>\n"
                         "\n"
                         "\n"
                         "bool is_number(const std::string &str) {\n"
                         "    return std::all_of(str.begin(), str.end(), [](char ch) { return ch >= '0' && ch <= '9'; });\n"
                         "}\n"
                         "\n"
                         "bool is_float(const std::string &str) {\n"
                         "    return std::all_of(str.begin(), str.end(), [](char ch) { return (ch >= '0' && ch <= '9') || ch == '.'; });\n"
                         "}\n"
                         "\n"
                         "std::string operator+(const std::string &str, int num) {\n"
                         "    return str + std::to_string(num);\n"
                         "}\n"
                         "\n"
                         "std::string operator+(int num, const std::string &str) {\n"
                         "    return std::to_string(num) + str;\n"
                         "}\n"
                         "\n"
                         "std::string operator+(const std::string &str, double num) {\n"
                         "    return str + std::to_string(num);\n"
                         "}\n"
                         "\n"
                         "std::string operator+(double num, const std::string &str) {\n"
                         "    return std::to_string(num) + str;\n"
                         "}\n"
                         "\n"
                         "double operator-(const std::string &lhs, double rhs) {\n"
                         "    if (is_number(lhs)) {\n"
                         "        return std::stoi(lhs) - rhs;\n"
                         "    } else if (is_float(lhs)) {\n"
                         "        return std::stod(lhs) - rhs;\n"
                         "    } else {\n"
                         "        return -rhs;\n"
                         "    }\n"
                         "}\n"
                         "\n"
                         "double operator-(double num, const std::string &str) {\n"
                         "    return -1 * (str - num);\n"
                         "}\n"
                         "\n"
                         "class mixed {\n"
                         "\n"
                         "public:\n"
                         "    std::variant<std::string, double> storage;\n"
                         "\n"
                         "    mixed(std::string str) : storage(str) {}\n"
                         "\n"
                         "    mixed(double num) : storage(num) {}\n"
                         "\n"
                         "    mixed &operator=(std::string str) {\n"
                         "        storage = str;\n"
                         "        return *this;\n"
                         "    }\n"
                         "\n"
                         "    mixed &operator=(float num) {\n"
                         "        storage = num;\n"
                         "        return *this;\n"
                         "    }\n"
                         "\n"
                         "    mixed &operator=(const mixed &other) = default;\n"
                         "\n"
                         "    template<typename T>\n"
                         "    friend mixed operator+(const mixed &lhs, T rhs) {\n"
                         "        if (std::holds_alternative<std::string>(lhs.storage)) {\n"
                         "            return std::get<std::string>(lhs.storage) + rhs;\n"
                         "        } else {\n"
                         "            return std::get<double>(lhs.storage) + rhs;\n"
                         "        }\n"
                         "    }\n"
                         "\n"
                         "    template<typename T>\n"
                         "    friend mixed operator+(T lhs, const mixed &rhs) {\n"
                         "        if (std::is_same_v<std::string, T> && std::holds_alternative<std::string>(rhs.storage)) {\n"
                         "            return lhs + std::get<std::string>(rhs.storage);\n"
                         "        }\n"
                         "        return rhs + lhs;\n"
                         "    }\n"
                         "\n"
                         "    friend mixed operator+(const mixed &lhs, const mixed &rhs) {\n"
                         "        if (std::holds_alternative<std::string>(rhs.storage)) {\n"
                         "            return lhs + std::get<std::string>(rhs.storage);\n"
                         "        } else {\n"
                         "            return lhs + std::get<double>(rhs.storage);\n"
                         "        }\n"
                         "    }\n"
                         "\n"
                         "    template<typename T>\n"
                         "    friend mixed operator-(const mixed &lhs, T rhs) {\n"
                         "        if constexpr (!std::is_same_v<std::string, T>) {\n"
                         "            if (std::holds_alternative<std::string>(lhs.storage)) {\n"
                         "                return std::get<std::string>(lhs.storage) - rhs;\n"
                         "            } else {\n"
                         "                return std::get<double>(lhs.storage) - rhs;\n"
                         "            }\n"
                         "        } else {\n"
                         "            return std::get<double>(lhs.storage) - rhs;\n"
                         "        }\n"
                         "    }\n"
                         "\n"
                         "    template<typename T>\n"
                         "    friend mixed operator-(T lhs, const mixed &rhs) {\n"
                         "        return -1 * (rhs - lhs);\n"
                         "    }\n"
                         "\n"
                         "    friend mixed operator-(const mixed &lhs, const mixed &rhs) {\n"
                         "        if (std::holds_alternative<std::string>(rhs.storage)) {\n"
                         "            return lhs - std::get<std::string>(rhs.storage);\n"
                         "        } else {\n"
                         "            return lhs - std::get<double>(rhs.storage);\n"
                         "        }\n"
                         "    }\n"
                         "};\n"
                         "\n"
                         "void print(const mixed &var) {\n"
                         "    if (std::holds_alternative<double>(var.storage)) {\n"
                         "        std::cout << std::get<double>(var.storage) << \"\\n\";\n"
                         "    } else {\n"
                         "        std::cout << std::get<std::string>(var.storage) << \"\\n\";\n"
                         "    }\n"
                         "}\n"
                         "\n"
                         "template<typename T>\n"
                         "void print(T t) {\n"
                         "    std::cout << t << \"\\n\";\n"
                         "}\n"
                         "\n"
                         "std::string input() {\n"
                         "    std::string str;\n"
                         "    std::cin >> str;\n"
                         "    return str;\n"
                         "}\n"
                         "\n"
                         "std::string input(const std::string &output) {\n"
                         "    std::cout << output;\n"
                         "    return input();\n"
                         "}\n"
                         "\n"
                         "#define var mixed\n"
                         "#define val const mixed\n";