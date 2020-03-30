// Задача 9. Алгоритм сжатия данных Хаффмана
// Реализуйте алгоритм построения оптимального префиксного кода Хаффмана.
// При помощи алгоритма реализуйте две функции для создания архива из одного файла и извлечения файла из архива.

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "Huffman.h"


class BitsWriter {
public:
    BitsWriter(IOutputStream &stream_) : stream(stream_) {};

    void WriteBit(bool bit);

    void WriteByte(unsigned char byte);

    void FlushBuffer();

private:
    IOutputStream &stream;
    unsigned char accumulator = 0;
    unsigned char bits_count = 0;
};

// Пишем биты в поток, как на лекции
void BitsWriter::WriteBit(bool bit) {
    accumulator |= bit << bits_count;
    ++bits_count;
    if (bits_count == 8) {
        stream.Write(accumulator);
        accumulator = 0;
        bits_count = 0;
    }
}

// Пишем байты в поток, как на лекции
void BitsWriter::WriteByte(unsigned char byte) {
    if (bits_count == 0) {
        stream.Write(byte);
    } else {
        accumulator |= byte << bits_count;
        stream.Write(accumulator);
        accumulator = byte >> (8 - bits_count);
    }
}

// Дописываем в поток остатки бит и число оставшихся бит
void BitsWriter::FlushBuffer() {
    if (bits_count != 0) {
        stream.Write(accumulator);
    }
    stream.Write(bits_count);
}

class BitsReader {
public:
    // Читаем поток до конца и кладем в очередь
    BitsReader(IInputStream &stream_) : stream(stream_) {
        unsigned char byte;
        while (stream.Read(byte)) {
            symbols.push_back(byte);
        }
    }

    void ReadByte(unsigned char &byte);

    bool ReadBit(bool &bit);

private:
    unsigned char buffer = 0;
    unsigned char bits_count = 0;
    std::deque<unsigned char> symbols;
    unsigned char reminder = 0;
    IInputStream &stream;

};

// Читаем из очереди по биту
bool BitsReader::ReadBit(bool &bit) {
    if ((symbols.empty()) && bits_count == 0) {
        return false;
    }

    if (bits_count == 0 && symbols.size() > 2) {
        buffer = symbols.front();
        symbols.pop_front();
    }
    // Все, что нужно, уже считали. Просто удаляем последний элемен
    if (bits_count == 0 && symbols.size() == 1) {
        symbols.pop_front();
    }

    bit = buffer & (1 << (bits_count - reminder));
    bits_count = (bits_count + 1) % 8;

    // Следим, не последние ли 2 байта читаем.
    // Если последние, то предпоследний байт - информация.
    // А последний - число нужных бит в предпоследнем.
    if (bits_count == 0 && symbols.size() == 2) {
        buffer = symbols.front();
        symbols.pop_front();
        if (symbols.front() > 0) {
            reminder = 8 - symbols.front();
            bits_count = reminder;
            symbols.pop_front();
        }
    }
    return true;
}

// Читаем из очереди по байту
void BitsReader::ReadByte(unsigned char &byte) {
    if (bits_count == 0) {
        byte = symbols.front();
        symbols.pop_front();
    } else {
        for (int i = 0; i < 8; ++i) {
            bool bit;
            ReadBit(bit);
            byte |= bit << i;
        }
    }
}


struct Node {

    Node *left = nullptr;
    Node *right = nullptr;
    unsigned char symbol = 0;
    unsigned int count = 0;

    // Констукторы на все случаи жизни :)
    Node(unsigned int count_, Node *left_, Node *right_) {
        count = count_;
        left = left_;
        right = right_;
    }

    Node(unsigned int symbol_, unsigned int count_) {
        symbol = symbol_;
        count = count_;
    }

    Node(unsigned int symbol_) {
        symbol = symbol_;
    }

    Node() {}

    // Если это - лист, то там обязательно будет символ
    bool is_char() const {
        return !left && !right;
    }
};

// Чтобы куча могла сравнивать ноды дерева
struct NodeComparator {
    bool operator()(const Node *node1, const Node *node2) {
        return node1->count > node2->count;
    }
};

// Чтобы удалять дерево, когда оно уже не нужно
void DeleteTree(Node *node) {
    if (!node) {
        return;
    }
    DeleteTree(node->left);
    DeleteTree(node->right);
    delete node;
}

// ---------- Encoder ----------

class HuffmanEncoder {
public:

    void EncodeBytes(IInputStream &original, IOutputStream &compressed);

private:

    Node *build_tree(const std::vector<unsigned char> bytes);

    void fill_dictionary(const Node *node, std::vector<bool> &code_bits,
                         std::unordered_map<unsigned char, std::vector<bool>> &dictionary);

    void write_tree(BitsWriter &bits_writer, const Node *node);
};

// Жадно строим дерево Хаффмана. Кладем 2 элемента в кучу и достаем 1, пока не закончатся.
Node *HuffmanEncoder::build_tree(const std::vector<unsigned char> bytes) {
    unsigned int char_counts[256] = {};

    for (unsigned char byte : bytes) {
        ++char_counts[byte];
    }

    std::vector<Node *> nodes;
    for (unsigned int i = 0; i < 256; ++i) {
        if (char_counts[i] > 0) {
            nodes.push_back(new Node(static_cast<unsigned char>(i), char_counts[i]));
        }
    }

    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> heap(nodes.begin(), nodes.end());

    while (heap.size() > 1) {
        Node *node1 = heap.top();
        heap.pop();
        Node *node2 = heap.top();
        heap.pop();

        int parent_count = node1->count + node2->count;
        Node *parent = new Node(parent_count, node1, node2);

        heap.push(parent);
    }

    Node *root = heap.top();
    heap.pop();
    return root;
}

// Рекурсивно заполняем словарь (символ -> его код) c помощью прохода по дереву
void HuffmanEncoder::fill_dictionary(const Node *node, std::vector<bool> &code_bits,
                                     std::unordered_map<unsigned char, std::vector<bool>> &dictionary) {

    if (node->left) {
        code_bits.push_back(false);
        fill_dictionary(node->left, code_bits, dictionary);
    }
    if (node->right) {
        code_bits.push_back(true);
        fill_dictionary(node->right, code_bits, dictionary);
    }

    if (node->is_char()) {
        dictionary[node->symbol] = code_bits;
    }

    if (!code_bits.empty()) {
        code_bits.pop_back();
    }
}


// Обходим дерево Хаффмана и пишем в поток, чтобы потом восстановить.
// Если нода - лист, пишем 1 и потом символ. Если нет, то 0
void HuffmanEncoder::write_tree(BitsWriter &bits_writer, const Node *node) {
    if (!node) {
        return;
    }
    if (node->is_char()) {
        bits_writer.WriteBit(true);
        bits_writer.WriteByte(node->symbol);
    } else {
        bits_writer.WriteBit(false);
    }
    write_tree(bits_writer, node->left);
    write_tree(bits_writer, node->right);
}

// Строим дерево. Строим словарь. Пишем дерево в поток и удаляем. Пишем кодированные биты в поток.
void HuffmanEncoder::EncodeBytes(IInputStream &original, IOutputStream &compressed) {
    unsigned char byte;
    std::vector<unsigned char> original_data;
    while (original.Read(byte)) {
        original_data.push_back(byte);
    }

    BitsWriter writer(compressed);

    Node *root_node = build_tree(original_data);
    std::unordered_map<unsigned char, std::vector<bool>> dictionary;

    std::vector<bool> code_bits;
    fill_dictionary(root_node, code_bits, dictionary);

    write_tree(writer, root_node);
    DeleteTree(root_node);

    for (unsigned char symbol : original_data) {
        std::vector<bool> bits = dictionary[symbol];
        for (bool bit : bits) {
            writer.WriteBit(bit);
        }
    }
    writer.FlushBuffer();
}

// ---------- Decoder ----------

class HuffmanDecoder {
public:
    void DecodeBytes(IInputStream &compressed, IOutputStream &original);

private:
    void build_tree(Node *node, BitsReader &bits_reader);

};

// Рекурсивно восстанавливаем дерево Хаффмана из бит в потоке.
// Встретили 1 - считали в текущую ноду следующий байт.
// Встретили 0 - сделали для ноды дочерние и пошли в них
void HuffmanDecoder::build_tree(Node *node, BitsReader &bits_reader) {
    bool bit;
    unsigned char byte;
    bits_reader.ReadBit(bit);

    if (!bit) {
        node->left = new Node();
        build_tree(node->left, bits_reader);
        node->right = new Node();
        build_tree(node->right, bits_reader);
    } else {
        bits_reader.ReadByte(byte);
        node->symbol = byte;
    }
}

// Восстанавливаем дерево. Проходимся по дереву и раскодируем сообщение. Пишем его в поток.
void HuffmanDecoder::DecodeBytes(IInputStream &compressed, IOutputStream &original) {
    BitsReader reader(compressed);
    Node *root_node = new Node();
    build_tree(root_node, reader);

    Node *node = root_node;
    bool bit;
    while (reader.ReadBit(bit)) {
        if (bit) {
            node = node->right;
        } else {
            node = node->left;
        }
        if (node->is_char()) {
            original.Write(node->symbol);
            node = root_node;
        }
    }
    DeleteTree(root_node);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    HuffmanEncoder encoder;
    encoder.EncodeBytes(original, compressed);
}


void Decode(IInputStream &compressed, IOutputStream &original) {
    HuffmanDecoder decoder;
    decoder.DecodeBytes(compressed, original);
}

int main() {

    IInputStream data1("aaaa bbb cc");
    IOutputStream result1;
    Encode(data1, result1);


    IInputStream data2(result1.buffer);
    IOutputStream result2;
    Decode(data2, result2);

    return 0;
}