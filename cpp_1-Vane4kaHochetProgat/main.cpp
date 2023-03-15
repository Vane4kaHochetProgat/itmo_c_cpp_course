//
// Created by vane4ka on 21.04.2022.
//

#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "phonebook.h"
#include "quicksort.h"
#include "return_codes.h"

template<class T>
int process(std::istream &in, const std::string_view &output_file_name) {
    std::string mode;
    int size;

    if (!(in >> mode)) {
        std::cerr << "Input file doesn't define a mode\n";
        return ERROR_INVALID_DATA;
    }

    if (!(in >> size)) {
        std::cerr << "Sort size is invalid\n";
        return ERROR_INVALID_DATA;
    }
    if (size <= 0) {
        std::cerr << "Sort size is invalid\n";
        return ERROR_INVALID_DATA;
    }

    std::vector<T> data(size);

    for (T &e : data) {
        if (!(in >> e)) {
            std::cerr << "Invalid params\n";
            return ERROR_INVALID_DATA;
        }
    }

    int average_index = 0;

    if (mode == "ascending") {
        quicksort<T, false>(data, 0, size - 1, average_index);
    } else if (mode == "descending") {
        quicksort<T, true>(data, 0, size - 1, average_index);
    } else {
        std::cerr << "Sort mode " << mode << " isn't implemented for quicksort\n";
        return ERROR_NOT_IMPLEMENTED;
    }

    std::ofstream fout(output_file_name.data());
    if (!fout.is_open() || !fout.good()) {
        std::cerr << "Can't write to output file: " << output_file_name << "\n";
        return ERROR_UNKNOWN;
    }
    for (const T &e : data) {
        if (!(fout << e)) {
            std::cerr << "Invalid params\n";
            return ERROR_UNKNOWN;
        }
        fout << "\n";
    }
    return ERROR_SUCCESS;
}

int main(int argc, char *argv[]) {
    std::vector<std::string_view> args(argv + 1, argv + argc);

    if (args.size() != 2) {
        std::cerr << "Wrong number of args\n";
        return ERROR_INVALID_PARAMETER;
    }

    std::ifstream fin(args[0].data());
    if (!fin.is_open()) {
        std::cerr << "Input file doesn't exist: " << args[0] << "\n";
        return ERROR_FILE_NOT_FOUND;
    }

    std::string type;
    if (!(fin >> type)) {
        std::cerr << "Input file doesn't define a type\n";
        return ERROR_INVALID_DATA;
    }

    if (type == "phonebook") {
        return process<Phonebook>(fin, args[1]);
    }
    if (type == "int") {
        return process<int>(fin, args[1]);
    }
    if (type == "float") {
        return process<float>(fin, args[1]);
    }
    std::cerr << "Type " << type << " isn't implemented for quicksort\n";
    return ERROR_NOT_IMPLEMENTED;
}