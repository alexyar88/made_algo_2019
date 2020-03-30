/*
 Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
 Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
 Напишите нерекурсивный алгоритм.
 Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
 Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.

 Вариант 4_4:
 Реализуйте стратегию выбора опорного элемента “случайный элемент”.
 Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
 * */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

// Начало и конец - включаются в диапазон: [start, end]
int GetRandomIntInRange(int start, int end) {
    std::srand(time(nullptr)); //
    return (std::rand() % (end + 1 - start)) + start;
}

void Swap(int *arr, int index1, int index2) {
    int temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}

int Partition(int *arr, int left, int right) {
    int p_element = GetRandomIntInRange(left, right);
    Swap(arr, p_element, left);


    int i = right;

    for (int j = right; j > 0; j--) {
        if ((arr[j] > arr[left])) {
            Swap(arr, i, j);
            --i;
        }
    }
    Swap(arr, i, left);
    return i;
}

// Возвращает позицию элемента в массиве, как если бы он был отсортирован
int FindKPosition(int *arr, int position, int left, int right) {
    // Если что-то пойдет не так, выкинем ошибку на assert.
    int result_position = -1;

    while (left <= right) {
        int partition_index = Partition(arr, left, right);

        if (arr[partition_index] == arr[position]) {
            result_position = partition_index;
            break;
        } else if (arr[partition_index] > arr[position]) {
            right = partition_index - 1;
        } else {
            left = partition_index + 1;
        }
    }

    assert(result_position >= 0);
    return result_position;
}

int main() {

    int array_size = 0;
    int position = 0;
    std::cin >> array_size >> position;

    int *arr = new int[array_size];
    for (int i = 0; i < array_size; ++i) {
        std::cin >> arr[i];
    }
    int value_position_in_sorted_array = FindKPosition(arr, position, 0, array_size - 1);
    std::cout << arr[value_position_in_sorted_array];

    delete[] arr;

    return 0;
}