/*
Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.
Формат ввода

В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 2 - pop front
a = 3 - push back
Если дана команда pop front, то число b - ожидаемое значение. Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.
Формат вывода

Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
*/

#include <iostream>
#include <cassert>

class Queue {
public:
    ~Queue() {
        delete[] buffer;
    }

    bool Empty() const;

    void Push(int value);

    void Pop();

    int Front() const;

private:
    int *buffer = nullptr;
    int buffer_size = 0;
    int head = 0;
    int tail = 0;

    void IncreaseBuffer();

    void CreateBuffer();
};

bool Queue::Empty() const {
    return head == tail;
}

int Queue::Front() const {
    assert(!Empty());
    return buffer[head % buffer_size];
}

void Queue::Push(int value) {
    if ((tail == 0) && (head == 0) && (buffer == nullptr)) {
        // Если нет буфера, создаем новый
        CreateBuffer();
    } else if ((tail > head) && (tail % buffer_size == head % buffer_size)) {
        // Если не хватает буфера, расширяем его
        IncreaseBuffer();
    }

    buffer[tail % buffer_size] = value;
    tail++;
}

void Queue::Pop() {
    assert(!Empty());
    head++;
}

void Queue::CreateBuffer() {
    buffer_size = 1;
    buffer = new int[1];
}

void Queue::IncreaseBuffer() {
    // Создаем массив х2 размера и копируем туда данные
    int new_buffer_size = buffer_size * 2;
    int *new_buffer = new int[new_buffer_size];

    for (int i = 0; i < buffer_size; ++i) {
        new_buffer[i] = buffer[(i + head) % buffer_size];
    }
    delete[] buffer;
    buffer = new_buffer;

    tail = buffer_size;
    head = 0;
    buffer_size = new_buffer_size;
}


int main() {

    int command_count = 0;
    std::cin >> command_count;

    Queue *queue = new Queue();

    for (int i = 0; i < command_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 2:
                if (queue->Empty()) {
                    if (value != -1) {
                        std::cout << "NO";
                        return 0;
                    }
                } else if (queue->Front() != value) {
                    std::cout << "NO";
                    return 0;
                } else {
                    queue->Pop();
                }
                break;

            case 3: {
                queue->Push(value);
                break;
            }
            default:
                break;
        }

    }

    delete queue;

    std::cout << "YES";

    return 0;
}