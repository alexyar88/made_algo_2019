/*
Задача 8. Хеш-таблица (10 баллов)
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

1_2. Для разрешения коллизий используйте двойное хеширование.

*/

#include <iostream>
#include <string>
#include <vector>

class HashTable {
public:
    explicit HashTable(int initial_size);

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

    ~HashTable();

private:
    struct HashTableElement {
        HashTableElement(std::string key_) : key(std::move(key_)) {}

        bool is_deleted = false;
        std::string key = nullptr;
    };

    std::vector<HashTableElement *> table;
    int element_count = 0;
    int table_size = 0;


    int hash(const std::string &str, int num_iter) const;

    void rehash();

};

// Конструктор
HashTable::HashTable(int initial_size) {
    table = std::vector<HashTableElement *>(initial_size, nullptr);
    table_size = initial_size;
}

// Декструктор
HashTable::~HashTable() {
    for (HashTableElement *element : table) {
        delete element;
    }
}


// Хэш-функция
int HashTable::hash(const std::string &str, int num_iter) const {
    int hash1 = 0; // вычисляем методом Горнера
    int hash2 = 0; // просто складываем все символы строки

    // Берем константу из метода Горнера нечетной, тогда она взвимопроста к размеру таблицы, который всегда четный
    int a = 2 * table_size + 1;

    for (char str_char : str) {
        hash1 = hash1 * a + str_char;
        hash2 += str_char;
    }

    // Делаем h2 нечетным, чтобы h2 был взаимнопростым к размеру таблицы, которая всегда четная
    hash2 = hash2 * 2 + 1;

    int hash_value = (hash1 + hash2 * num_iter) % table_size;
    if (hash_value < 0) { // Если вдруг было переполнение int и хэш получился отрицательный
        hash_value = hash_value + table_size;
    }
    return hash_value;
}

// Перехеширование
void HashTable::rehash() {
    table_size = table_size * 2; // Увеличиваем размер буфера в 2 раза
    element_count = 0; // Обнуляем счетчик элементов
    std::vector<HashTableElement *> new_table(table_size);

    // Переносим неудаленные элементы в новую таблицу
    for (HashTableElement *element : table) {
        if (element) {
            // Если элемент помечен, как удаленный, освобождаем память
            if (element->is_deleted) {
                delete element;
            } else { // Если нет - то переносим в новый буфер
                int iteration = 0;
                while (true) {
                    int new_hash = hash(element->key, iteration);
                    if (!new_table[new_hash]) {
                        new_table[new_hash] = element;
                        ++element_count;
                        break;
                    }
                    ++iteration;
                }
            }
        }
    }

    table = new_table;
}

// Добавляем ключ
bool HashTable::Add(const std::string &key) {

    // Если коэфициент заполнения таблицы >= 3/4, перехэшируем и потом вставляем новый элемент
    if (element_count * 4 >= table_size * 3) {
        rehash();
    }

    int iteration = 0;

    // Переменная нужна, чтобы при вставке элемента за 1 проход по циклу одновременно
    // искать позицию для вставки и проверять наличие элемента в таблице
    int insert_position = -1;

    while (true) {
        int hash_value = hash(key, iteration);

        // Если нашли дырку, значит такого ключа в таблице нет. Будем вставлять новый ключ или сюда,
        // или, если нашли раньше удаленный элемент, то вместо него, чтобы сэкономить место
        if (!table[hash_value]) {
            if (insert_position < 0) {
                insert_position = hash_value;
            }
            break;
        } else {
            // Если есть удаленный элемент, запомним место для вставки, но продолжим поиск, т.к.
            // такой ключ уже может быть в таблице
            if (table[hash_value]->is_deleted) {
                if (insert_position < 0) {
                    insert_position = hash_value;
                }
            } else if (table[hash_value]->key == key) { // Если ключ уже в таблице, выходим
                return false;
            }
        }
        iteration++;
    }

    if (!table[insert_position]) { // Вставляем в пустое место
        table[insert_position] = new HashTableElement(key);
        ++element_count;
    } else { // Вставляем вместо удаленного элемента
        table[insert_position]->key = key;
        table[insert_position]->is_deleted = false;
    }
    return true;
}

// Проверяем наличие ключа
bool HashTable::Has(const std::string &key) const {
    int iteration = 0;
    while (true) {
        int hash_value = hash(key, iteration);

        if (!table[hash_value]) {
            return false;
        }

        if (!table[hash_value]->is_deleted && table[hash_value]->key == key) {
            return true;
        }
        iteration++;
    }
}

// Удаляем ключ
bool HashTable::Remove(const std::string &key) {
    int iteration = 0;
    while (true) {
        int hash_value = hash(key, iteration);

        if (!table[hash_value]) {
            return false;
        }

        // Находим ключ и помечаем его как удаленный
        if (!table[hash_value]->is_deleted && table[hash_value]->key == key) {
            table[hash_value]->is_deleted = true;
            return true;
        }
        iteration++;
    }
}

int main() {

    HashTable table(8);
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                break;
        }
    }
    return 0;
}