#include <iostream>
#include <cstring>
//#include "sort.h"

const unsigned int PART_SIZE = 511;

void SortDigit(unsigned int *arr, const unsigned int size, const unsigned int digit_num) {




}

const unsigned int MaxValue(unsigned int *arr, const unsigned int size) {
    unsigned int max_value = 0;
    for (unsigned int *end = (arr + size); arr != end; arr++) {
        if (*arr > max_value) {
            max_value = *arr;
        }
    }
    return max_value;
}

void Sort(unsigned int *arr, unsigned int size) {
    const unsigned int max_element = MaxValue(arr, size);
    unsigned int *el;
    unsigned int *el2;
    unsigned int *end;
    unsigned int i;

    unsigned int count_arr[PART_SIZE] = {};

    for (unsigned int exp = 1; max_element / exp > 0; exp = exp * 511) {
        for (el = arr, end = (arr + size); el != end; el++) {
            count_arr[(*el / exp) % 511]++;
        }
        for (el = count_arr + 1, el2 = count_arr, end = count_arr + 255; el != end; el++, el2++) {
            *el += *el2;
        }

        unsigned int *result_arr = new unsigned int[size];
        unsigned int count_arr_index;

        for (el = arr + size - 1, end = arr - 1; el != end; el--) {
            count_arr_index = (*el / exp) % 511;
            result_arr[count_arr[count_arr_index] - 1] = *el;
            count_arr[count_arr_index]--;
        }



        for (el = arr, end = arr + size, el2 = result_arr; el != end; el++, el2++) {
            *el = *el2;
        }
        delete[] result_arr;
    }
}






int main() {
//    std::cout << "Hello, World!" << std::endl;
    unsigned int arr[5] = {41, 42, 35, 66, 27};
    Sort(arr, 5);
    unsigned int max_value = MaxValue(arr, 5);
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << std::endl;
    }
//    std::cout << max_value << std::endl;
    return 0;
}