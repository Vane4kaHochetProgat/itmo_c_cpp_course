//
// Created by vane4ka on 21.04.2022.
//

#ifndef CPP_1_VANE4KAHOCHETPROGAT_QUICKSORT_H
#define CPP_1_VANE4KAHOCHETPROGAT_QUICKSORT_H

#include <random>
#include <utility>
#include <vector>

template<typename T, bool descending>
struct Comparator;

template<typename T>
struct Comparator<T, false> {
    bool operator()(const T &left, const T &right) { return left < right; }
};

template<typename T>
struct Comparator<T, true> {
    bool operator()(const T &left, const T &right) { return right < left; }
};

int random_index(int low, int high) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(low, high);
    return dist(rng);
}

template<typename T, bool descending>
void quicksort(std::vector<T> &data, int low, int high, int &i) {
    Comparator<T, descending> comp;
    if (low < high) {
        std::swap(data[random_index(low, high)], data[high]);
        i = (low - 1);

        for (int j = low; j < high; j++) {
            if (comp(data[j], data[high])) {
                i++;
                std::swap(data[i], data[j]);
            }
        }
        std::swap(data[i + 1], data[high]);
        //		std::cout << i << "\n";
        //		std::cout << i - low << " " << high - i - 2 << "\n";
        quicksort<T, descending>(data, low, i, i);
        quicksort<T, descending>(data, i + 2, high, i);
    }
}

#endif      // CPP_1_VANE4KAHOCHETPROGAT_QUICKSORT_H