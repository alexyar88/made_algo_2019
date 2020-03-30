/*
Солдаты.

В одной военной части решили построить в одну шеренгу по росту.
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.

Вариант 7_1.  Требуемая скорость выполнения команды - O(log n) в среднем. В реализации используйте декартово дерево.
*/

#include <iostream>


struct TreapNode {

    explicit TreapNode(int _value, int _priority) {
        value = _value;
        priority = _priority;
    };

    TreapNode *left = nullptr;
    TreapNode *right = nullptr;
    int value = 0;
    int priority = 0;

    // Глубина поддерева нам нужна, чтобы выичслять позицию солдата за log время
    int subtree_size = 1;
};


class Treap {
public:
    ~Treap();

    void Insert(int value, int priority);

    void Remove(int value);

    int GetPositionByValue(int value) const;

    int GetValueByPosition(int position) const;

private:
    TreapNode *root = nullptr;

    void delete_subtree(TreapNode *node);

    std::pair<TreapNode *, TreapNode *> split(TreapNode *node, int value);

    TreapNode *merge(TreapNode *left, TreapNode *right);

    void update_subtree_size(TreapNode *node);

    int get_subtree_size(TreapNode *node) const;

    TreapNode *get_node_by_position(TreapNode *node, int position) const;

    int get_position_by_value(TreapNode *node, int value, int position) const;

};

// Рекурсивно удаляем узел и все подузлы
void Treap::delete_subtree(TreapNode *node) {

    if (!node) {
        return;
    }

    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}


std::pair<TreapNode *, TreapNode *> Treap::split(TreapNode *node, int value) {
    if (!node) {
        return std::make_pair(nullptr, nullptr);
    }

    if (node->value < value) {
        const auto right_pair = split(node->right, value);
        node->right = right_pair.first;
        update_subtree_size(node); // Обновляем глубину подерева
        return std::make_pair(node, right_pair.second);
    } else {
        const auto left_pair = split(node->left, value);
        node->left = left_pair.second;
        update_subtree_size(node); // Обновляем глубину подерева
        return std::make_pair(left_pair.first, node);
    }
}


TreapNode *Treap::merge(TreapNode *left, TreapNode *right) {
    if (!left) {
        return right;
    }
    if (!right) {
        return left;
    }

    if (left->priority > right->priority) {
        left->right = merge(left->right, right);
        update_subtree_size(left); // Обновляем глубину подерева
        return left;
    } else {
        right->left = merge(left, right->left);
        update_subtree_size(right); // Обновляем глубину подерева
        return right;
    }
}

// Возвращает глубину поддерева или 0, если нода - null. Чтобы не делать проверку на null
int Treap::get_subtree_size(TreapNode *node) const {
    if (!node) {
        return 0;
    } else {
        return node->subtree_size;
    }
}

// Для обновления глубины поддерева после split/merge
void Treap::update_subtree_size(TreapNode *node) {
    node->subtree_size = get_subtree_size(node->left) + get_subtree_size(node->right) + 1;
}

// Рекурсивно ищем узел по заданной позиции. Нужно, чтобы потом его удалить. !!! Позиции идут справа налево
TreapNode *Treap::get_node_by_position(TreapNode *node, int position) const {
    int current_position = get_subtree_size(node->right);
    if (current_position == position) {
        return node;
    } else if (current_position > position) {
        return get_node_by_position(node->right, position);
    } else {
        return get_node_by_position(node->left, position - current_position - 1);
    }
}

// Рекурсивно ищем позицию узла по значению. !!! Позиция считается от большего к меньшему, справа налево.
int Treap::get_position_by_value(TreapNode *node, int value, int position) const {

    if (node->value == value) {
        // Выход из рекурсии. Возвращаем накопленную позицию + то, что справа
        return position + get_subtree_size(node->right);
    } else if (node->value > value) {
        // Если мы правее значения, рекурсивно идем влево и накапливаем позицию того, что сейчас справа
        return get_position_by_value(node->left, value, position + get_subtree_size(node->right) + 1);
    } else {
        // Если левее - вправо
        return get_position_by_value(node->right, value, position);
    }

}


Treap::~Treap() {
    delete_subtree(root);
}

// Вставляем элемент, как объясняли в лекции
void Treap::Insert(int value, int priority) {

    if (!root) {
        root = new TreapNode(value, priority);
        return;
    }

    TreapNode *new_node = new TreapNode(value, priority);
    auto split_pair = split(root, value);
    TreapNode *tree_first = merge(split_pair.first, new_node);
    root = merge(tree_first, split_pair.second);
}

// Удаляем элемент, как объясняли в лекции
void Treap::Remove(int value) {
    auto split_pair1 = split(root, value + 1);
    auto split_pair2 = split(split_pair1.first, value);
    delete_subtree(split_pair2.second); // Удаляем отделившийся после сплитов элемент
    root = merge(split_pair2.first, split_pair1.second);
}

// Публичная обертка для рекурсивной функции
int Treap::GetPositionByValue(int value) const {
    return get_position_by_value(root, value, 0);
}

// Публичная обертка для рекурсивной функции. Возвращает значение, по которому удаляем элемент
int Treap::GetValueByPosition(int position) const {
    TreapNode *node = get_node_by_position(root, position);
    return node->value;
}


int main() {

    int command_count = 0;
    int command_value = 0;
    int command = 0;

    Treap *treap = new Treap();

    std::cin >> command_count;
    for (int i = 0; i < command_count; ++i) {
        std::cin >> command >> command_value;
        if (command == 1) {
            treap->Insert(command_value, rand());
            std::cout << treap->GetPositionByValue(command_value) << std::endl;
        } else if (command == 2) {
            int position_value = treap->GetValueByPosition(command_value);
            treap->Remove(position_value);
        }
    }

    delete treap;
    return 0;
}