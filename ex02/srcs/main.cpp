#include "PMergeMe.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <vector>

static bool parsePositiveInt(const std::string& token, int& value) {
    char* end = NULL;
    long parsed = std::strtol(token.c_str(), &end, 10);

    if (end == token.c_str() || *end != '\0' || parsed <= 0 || parsed > 2147483647L) {
        return false;
    }
    value = static_cast<int>(parsed);
    return true;
}

int main(int argc, char** argv) {
    std::vector<int> values;
    std::list<int> valuesList;

    if (argc < 2) {
        std::cerr << "Error not enough arguments" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        int value = 0;
        if (!parsePositiveInt(argv[i], value)) {
            std::cerr << "Error during the conversion of arguments to numbers" << std::endl;
            return 1;
        }
        values.push_back(value);
        valuesList.push_back(value);
    }

    PmergeMe merger(values, valuesList);
    merger.sort();
    return 0;
}
