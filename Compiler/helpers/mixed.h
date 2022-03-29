#pragma once

#include <algorithm>
#include <iostream>
#include <variant>


bool is_number(const std::string &str) {
    size_t pos = 0;
    try {
        std::stod(str, &pos);
        return pos == str.size();
    } catch (std::invalid_argument &e) {
        return false;
    }
}

std::string operator+(const std::string &str, int num) {
    return str + std::to_string(num);
}

std::string operator+(int num, const std::string &str) {
    return std::to_string(num) + str;
}

std::string operator+(const std::string &str, double num) {
    return str + std::to_string(num);
}

std::string operator+(double num, const std::string &str) {
    return std::to_string(num) + str;
}

double operator-(const std::string &lhs, double rhs) {
    if (is_number(lhs)) {
        return std::stod(lhs) - rhs;
    } else {
        return -rhs;
    }
}

double operator-(double num, const std::string &str) {
    return -1 * (str - num);
}

class mixed {

public:
    std::variant<int, double, std::string> storage;

    mixed(std::string str) : storage(str) {}

    mixed(int num) : storage(num) {}

    mixed(double num) : storage(num) {}

    mixed &operator=(std::string str) {
        storage = str;
        return *this;
    }

    mixed &operator=(float num) {
        storage = num;
        return *this;
    }

    mixed &operator=(int num) {
        storage = num;
        return *this;
    }

    mixed &operator=(const mixed &other) = default;

    template<typename T>
    friend mixed operator+(const mixed &lhs, T rhs) {
        if (std::holds_alternative<std::string>(lhs.storage)) {
            return std::get<std::string>(lhs.storage) + rhs;
        } else if (std::holds_alternative<int>(lhs.storage)) {
            return std::get<int>(lhs.storage) + rhs;
        } else {
            return std::get<double>(lhs.storage) + rhs;
        }
    }

    template<typename T>
    friend mixed operator+(T lhs, const mixed &rhs) {
        if (std::is_same_v<std::string, T> && std::holds_alternative<std::string>(rhs.storage)) {
            return lhs + std::get<std::string>(rhs.storage);
        }
        return rhs + lhs;
    }

    friend mixed operator+(const mixed &lhs, const mixed &rhs) {
        if (std::holds_alternative<std::string>(rhs.storage)) {
            return lhs + std::get<std::string>(rhs.storage);
        } else if (std::holds_alternative<int>(rhs.storage)) {
            return lhs + std::get<int>(rhs.storage);
        } else {
            return lhs + std::get<double>(rhs.storage);
        }
    }

    template<typename T>
    friend mixed operator-(const mixed &lhs, T rhs) {
        if constexpr (!std::is_same_v<std::string, T>) {
            if (std::holds_alternative<std::string>(lhs.storage)) {
                return std::get<std::string>(lhs.storage) - rhs;
            } else if (std::holds_alternative<int>(lhs.storage)) {
                return std::get<int>(lhs.storage) - rhs;
            } else {
                return std::get<double>(lhs.storage) - rhs;
            }
        } else {
            if (std::holds_alternative<int>(lhs.storage)) {
                return std::get<int>(lhs.storage) - rhs;
            } else {
                return std::get<double>(lhs.storage) - rhs;
            }
        }
    }

    template<typename T>
    friend mixed operator-(T lhs, const mixed &rhs) {
        return -1 * (rhs - lhs);
    }

    friend mixed operator-(const mixed &lhs, const mixed &rhs) {
        if (std::holds_alternative<std::string>(rhs.storage)) {
            return lhs - std::get<std::string>(rhs.storage);
        } else if (std::holds_alternative<int>(rhs.storage)) {
            return lhs - std::get<int>(rhs.storage);
        } else {
            return lhs - std::get<double>(rhs.storage);
        }
    }
};

void print(const mixed &var) {
    if (std::holds_alternative<double>(var.storage)) {
        std::cout << std::get<double>(var.storage) << "\n";
    } else if (std::holds_alternative<int>(var.storage)) {
        std::cout << std::get<int>(var.storage) << "\n";
    }else {
        std::cout << std::get<std::string>(var.storage) << "\n";
    }
}

template<typename T>
void print(T t) {
    std::cout << t << "\n";
}

mixed input() {
    std::string str;
    std::cin >> str;
    if (is_number(str)) {
        return std::stod(str);
    }
    return str;
}

mixed input(const std::string &output) {
    std::cout << output;
    return input();
}

#define var mixed
#define val const mixed
