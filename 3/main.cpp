/*
3_2. Сортировка почти упорядоченной последовательности.
Дана последовательность целых чисел a1...an и натуральное число k, такое что для любых i, j: если j >= i + k, то a[i] <= a[j].
Требуется отсортировать последовательность. Последовательность может быть очень длинной.
Время работы O(n * log(k)). Память O(k). Использовать слияние.
*/

#include <iostream>

void Merge(const int *arr, int *buffer, int first1, int last1, int first2, int last2) {
    int buffer_index = 0;
    while ((first1 <= last1) && (first2 <= last2)) {
        if (arr[first1] < arr[first2]) {
            buffer[buffer_index] = arr[first1];
            ++first1;
        } else {
            buffer[buffer_index] = arr[first2];
            ++first2;
        }
        ++buffer_index;
    }

    if ((first1 > last1) && (first2 > last2)) {
        return;
    } else if (first1 > last1) {
        while (first2 <= last2) {
            buffer[buffer_index] = arr[first2];
            ++buffer_index;
            ++first2;
        }
    } else if (first2 > last2) {
        while (first1 <= last1) {
            buffer[buffer_index] = arr[first1];
            ++buffer_index;
            ++first1;
        }
    }
}

void MergeSort(int *arr, int first, int last) {
    if (first >= last) {
        return;
    }

    int center = (last + first) / 2;

    MergeSort(arr, first, center);
    MergeSort(arr, center + 1, last);

    int buffer_size = last - first + 1;
    int *buffer = new int[buffer_size];
    Merge(arr, buffer, first, center, center + 1, last);
    for (int i = 0; i < buffer_size; ++i) {
        arr[i + first] = buffer[i];
    }
    delete[] buffer;
}

// Сортируем первый кусок. Потом сортируем следующий и мержим с предыдущим.
void AboutSoredSort(int *arr, int array_size, int part_size) {

    // Если вдруг К в задаче >= размера входных данных, просто сортируем все
    if (part_size <= array_size) {
        MergeSort(arr, 0, array_size - 1);
        return;
    }

    // округляем в большую сторону число кусков
    int parts_count = (array_size + part_size - 1) / part_size;

    // Сортируем первый кусок, чтобы было с чем мержить
    MergeSort(arr, 0, part_size - 1);

    // Сортируем следующий кусок и мержим с предыдущим
    for (int part_num = 1; part_num <= parts_count; ++part_num) {

        // Размер последнего куска может быть меньше, чем К, учитываем это
        if (part_num * part_size > array_size) {
            part_size = array_size - (part_num - 1) * part_size;
        }

        MergeSort(arr, part_num * part_size, (part_num + 1) * part_size - 1);

        int *buffer = new int[part_size];

        Merge(arr, buffer,
              (part_num - 1) * part_size,
              part_num * part_size - 1,
              part_num * part_size,
              (part_num + 1) * part_size - 1);

        for (int i = 0; i < part_size; ++i) {
            arr[i + part_num * part_size] = buffer[i];
        }

        delete[] buffer;
    }

}


int main() {
    int array_size = 0;
    int part_size = 0;
    std::cin >> array_size >> part_size;

    // Заполняем массив входных данных
    int *input_array = new int[array_size];
    for (int i = 0; i < array_size; ++i) {
        std::cin >> input_array[i];
    }

    AboutSoredSort(input_array, array_size, part_size);

    for (int i = 0; i < array_size; ++i) {
        std::cout << input_array[i] << " ";
    }
    delete[] input_array;

    return 0;
}