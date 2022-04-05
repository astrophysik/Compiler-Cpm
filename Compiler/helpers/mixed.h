#include <algorithm>
#include <iostream>
#include <variant>
#include <cassert>

// Как я понимаю для разработки использовался как раз этот класс
// и уже после он был скопирован в lib.h, смотри комментарий там.
class mixed {
public:
    std::variant<int, double, std::string> storage;

    mixed() = default;

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

    friend mixed operator*(int lhs, const mixed &rhs);

    friend mixed operator+(const mixed &lhs, const mixed &rhs);

    friend mixed operator-(const mixed &lhs, const mixed &rhs);
};

bool is_integer(const std::string &str) {
    size_t pos = 0;
    try {
        std::stoi(str, &pos);
        return pos == str.size();
    } catch (std::invalid_argument &e) {
        return false;
    }
}

bool is_double(const std::string &str) {
    size_t pos = 0;
    try {
        std::stod(str, &pos);
        return pos == str.size();
    } catch (std::invalid_argument &e) {
        return false;
    }
}

mixed operator+(const std::string &str, int num) {
    return str + std::to_string(num);
}

mixed operator+(int num, const std::string &str) {
    return std::to_string(num) + str;
}

mixed operator+(const std::string &str, double num) {
    return str + std::to_string(num);
}

mixed operator+(double num, const std::string &str) {
    return std::to_string(num) + str;
}

mixed operator-(const std::string &str, int num) {
    if (is_integer(str)) {
        return std::stoi(str) - num;
    } else if (is_double(str)) {
        return std::stod(str) - num;
    } else {
        return -num;
    }
}

mixed operator-(const std::string &str, double num) {
    if (is_integer(str)) {
        return std::stoi(str) - num;
    } else if (is_double(str)) {
        return std::stod(str) - num;
    } else {
        return -num;
    }
}

mixed operator-(int num, const std::string &str) {
    return -1 * (str - num);
}

mixed operator-(double num, const std::string &str) {
    return -1 * (str - num);
}

std::string operator-(const std::string &lhs, const std::string &rhs) {
    return "";
}

std::string operator*(int lhs, const std::string &rhs) {
    return rhs;
}

mixed operator*(int lhs, const mixed &rhs) {
    return std::visit([&lhs](auto &&arg){
        return mixed(lhs * arg);
    }, rhs.storage);
}

mixed operator+(const mixed &lhs, const mixed &rhs) {
    return std::visit([](auto && lhs, auto &&rhs) {
        return mixed(lhs + rhs);
    }, lhs.storage, rhs.storage);
}

mixed operator-(const mixed &lhs, const mixed &rhs) {
    return std::visit([](auto &&lhs, auto &&rhs) {
        return mixed(lhs - rhs);
    }, lhs.storage, rhs.storage);
}

void print(const mixed &var) {
    std::visit([](auto &&arg){
        std::cout << arg << "\n";
    }, var.storage);
}

template<typename T>
void print(T t) {
    std::cout << t << "\n";
}

mixed input() {
    std::string str;
    std::cin >> str;
    if (is_integer(str)) {
        return std::stoi(str);
    } else if (is_double(str)) {
        return std::stod(str);
    } else {
        return str;
    }
}

mixed input(const std::string &output) {
    std::cout << output;
    return input();
}

#define var mixed
#define val const mixed
