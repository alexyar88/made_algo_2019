/*
2_2. Быстрое сложение

Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
((1+2)+3) -> 1+2 + 3+3 = 9
((1+3)+2) -> 1+3 + 4+2 = 10
((2+3)+1) -> 2+3 + 5+1 = 11
Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.
Формат входных данных. Вначале вводится n - количество чисел. Затем вводится n строк - значения чисел (значение каждого числа не превосходит 10^9, сумма всех чисел не превосходит 2*10^9).
Формат выходных данных. Натуральное число - минимальное время.
*/

#include <iostream>
#include <cassert>

class PriorityQueue {
public:
    ~PriorityQueue() {
        delete[] buffer;
    }

    bool Empty() const;

    void Push(int value);

    int Top() const;

    int Pop();


private:
    int *buffer = nullptr;
    int size = 0;
    int max_size = 0;

    void SiftUp(int index);

    void SiftDown(int index);

    int Parent(int child_index) const;

    int ChildLeft(int parent_index) const;

    int ChildRight(int parent_index) const;

    void Swap(int index1, int index2);

    void IncreaseBuffer();


};

int PriorityQueue::Parent(int child_index) const {
    assert(child_index > 0);
    return (child_index - 1) / 2;
}

int PriorityQueue::ChildLeft(int parent_index) const {
    return 2 * parent_index + 1;
}

int PriorityQueue::ChildRight(int parent_index) const {
    return 2 * parent_index + 2;
}

void PriorityQueue::Swap(int index1, int index2) {
    assert((index1 < size) && (index2 < size));
    int temp = buffer[index1];
    buffer[index1] = buffer[index2];
    buffer[index2] = temp;
}

void PriorityQueue::SiftUp(int index) {
    while ((index > 0) && buffer[Parent(index)] > buffer[index]) {
        Swap(Parent(index), index);
        index = Parent(index);
    }
}

void PriorityQueue::SiftDown(int index) {
    while (true) {
        int min_index = index;
        int child_left_index = ChildLeft(index);
        if ((child_left_index < size) && buffer[child_left_index] < buffer[min_index]) {
            min_index = child_left_index;
        }

        int child_right_index = ChildRight(index);
        if ((child_right_index < size) && buffer[child_right_index] < buffer[min_index]) {
            min_index = child_right_index;
        }

        if (min_index != index) {
            Swap(index, min_index);
            index = min_index;
        } else {
            break;
        }
    }
}

void PriorityQueue::IncreaseBuffer() {
    if (buffer == nullptr) { // Если буфера нет, создаем
        buffer = new int[1];
        max_size = 1;
    } else { // Если есть, увеличиваем в 2 раза
        int new_max_size = max_size * 2;
        int *new_buffer = new int[new_max_size];

        for (int i = 0; i < max_size; ++i) {
            new_buffer[i] = buffer[i];
        }

        delete[] buffer;
        buffer = new_buffer;
        max_size = new_max_size;
    }
}

bool PriorityQueue::Empty() const {
    return size == 0;
}

int PriorityQueue::Top() const {
    assert((!Empty() && buffer != nullptr));
    return buffer[0];
}

void PriorityQueue::Push(int value) {
    if (size == max_size) {
        IncreaseBuffer();
    }
    buffer[size] = value;
    ++size;
    SiftUp(size - 1);
}

int PriorityQueue::Pop() {
    assert((!Empty() && buffer != nullptr));
    int result = buffer[0];
    Swap(0, size - 1);
    --size;
    SiftDown(0);
    return result;
}

int main() {

    int number_count = 0;
    int value = 0;
    std::cin >> number_count;

    PriorityQueue *queue = new PriorityQueue();

    for (int i = 0; i < number_count; ++i) {
        std::cin >> value;
        queue->Push(value);
    }
    // Если взять 4 элемента по 5*10**8 то обычный int переполняется, хотя условия задачи выполняется.
    // Поэтому, берем int64_t
    int64_t result = 0;
    for (int i = 0; i < number_count - 1; ++i) {
        int sum = queue->Pop() + queue->Pop();
        queue->Push(sum);
        result += sum;
    }

    delete queue;

    std::cout << result;

    return 0;
}