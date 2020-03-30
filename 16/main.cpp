#include <iostream>
#include <map>
#include <memory>

struct Node {
    std::map<char, std::shared_ptr<Node>> go;
    bool is_terminal = false;
};

class Trie {
public:

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    void Print() const;

private:
    std::shared_ptr<Node> root = std::make_shared<Node>();

    static void print(const std::shared_ptr<Node> &node, const std::string &current);
};

bool Trie::Has(const std::string &key) const {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            return false;
        }
        current = next->second;
    }

    return current->is_terminal;
}


bool Trie::Add(const std::string &key) {
    std::shared_ptr<Node> current = root;
    for (char symbol : key) {
        auto next = current->go.find(symbol);
        if (next == current->go.end()) {
            std::shared_ptr<Node> node_new = std::make_shared<Node>();
            current->go[symbol] = node_new;
            current = node_new;
        } else {
            current = next->second;
        }
    }
    if (current->is_terminal) {
        return false;
    }
    current->is_terminal = true;
    return true;
}

void Trie::print(const std::shared_ptr<Node> &node, const std::string &current) {
    if (node->is_terminal) {
        std::cout << current << std::endl;
    }
    for (const auto go : node->go) {
        print(go.second, current + go.first);
    }
}

void Trie::Print() const {
    print(root, "");
}


int main() {
    Trie trie;
    trie.Add("hi");
    trie.Add("his");
    trie.Add("abacaba");
    trie.Add("a");
    trie.Add("aba");
    trie.Print();
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}
