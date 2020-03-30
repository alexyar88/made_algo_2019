/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 2. С помощью z-функции.
*/

#include <iostream>
#include <vector>

std::vector<int> ZFunction(std::string &text) {
    int text_len = text.length();
    std::vector<int> z_values(text_len);
    int left = 0;
    int right = 0;

    for (int i = 1; i < text_len; ++i) {
        if (i <= right) {
            z_values[i] = std::min(right - i + 1, z_values[i - left]);
        }
        while ((z_values[i] + i < text_len) && (text[z_values[i]] == text[z_values[i] + i])) {
            ++z_values[i];
        }
        if (i + z_values[i] - 1 > right) {
            left = i;
            right = z_values[i] + i - 1;
        }
    }

    return z_values;
}

std::vector<int> KMP(std::string &text, std::string &pattern) {

    std::vector<int> result;

    int text_len = text.length();
    int pattern_len = pattern.length();



    // Считаем z-функцию для паттерна.
    std::vector<int> z_pattern = ZFunction(pattern);

    // Будет хранить текущее значение z-функции для строки
    int z_val = 0;

    // Т.к. z-функция для паттерна уже найдена, задаем индексы left и right с учетом z-функции для паттерна
    int left = 0;
    int right = pattern_len;

    // Находим значение z-функции для каждого символа из строки
    // Итерируемся, будто сконкатенировали паттерн, разделитель и строку
    // Начинаем итерироваться с 1-го символа строки.
    for (int i = pattern_len; i < text_len + pattern_len + 1; ++i) {

        if (i <= right) {
            z_val = std::min(right - i + 1, z_pattern[i - left]);
        }
        while ((z_val < pattern_len) && (pattern[z_val] == text[z_val + i - (pattern_len + 1)])) {
            ++z_val;
        }
        if (z_val + i - 1 > right) {
            left = i;
            right = z_val + i - 1;
        }

        if (z_val == pattern_len) {
            result.push_back(i - (pattern_len + 1));
        }
    }

    return result;
}

int main() {
    std::string pattern;
    std::string text;
    std::getline(std::cin, pattern);
    std::getline(std::cin, text);
    for (const int val : KMP(text, pattern)) {
        std::cout << val << ' ';
    }
    return 0;
}
