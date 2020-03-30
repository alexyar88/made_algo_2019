/*
13. «Пятнашки»
Написать алгоритм для решения игры в “пятнашки”
 * */

#include <iostream>
#include <vector>
#include <assert.h>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <unordered_set>

struct Position {
    std::vector<char> chips;
    char zero_place;
    int distance = INT16_MAX;

    bool IsFinish() const;

    std::vector<Position> Siblings() const;

    int Heuristic() const; // Эвристика: число клеток не на своем месте

    bool operator==(const Position &other) const {
        for (size_t i = 0; i < chips.size(); ++i) {
            if (other.chips[i] != chips[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Position &other) const {
        return !operator==(other);
    }

};

namespace std {
    template<>
    struct hash<Position> {
        size_t operator()(const Position &pos) const {
            size_t value = 0;
            for (int chip : pos.chips) {
                value = 37 * value + static_cast<size_t>(chip);
            }
            return value;
        }
    };
}

const Position FINISH_POSITION = Position{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, 15};

bool Position::IsFinish() const {
    return operator==(FINISH_POSITION);
}


std::vector<Position> Position::Siblings() const {
    std::vector<Position> result;
    // Ноль вниз, фишка вверх
    if (zero_place < 12) {
        Position sibling = Position{this->chips, this->zero_place};
//        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 4]);
        sibling.zero_place += 4;
        result.emplace_back(sibling);
    }
    // Ноль вверх, фишка вниз
    if (zero_place > 3) {
        Position sibling = Position{this->chips, this->zero_place};
//        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 4]);
        sibling.zero_place -= 4;
        result.emplace_back(sibling);
    }
    // Ноль влево, фишка вправо
    if (zero_place % 4 != 0) {
        Position sibling = Position{this->chips, this->zero_place};
//        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place - 1]);
        sibling.zero_place -= 1;
        result.emplace_back(sibling);
    }
    // Ноль вправо, фишка влево
    if (zero_place % 4 != 3) {
        Position sibling = Position{this->chips, this->zero_place};
//        Position sibling = *this;
        std::swap(sibling.chips[zero_place], sibling.chips[zero_place + 1]);
        sibling.zero_place += 1;
        result.emplace_back(sibling);
    }
    return result;
}

// Эвристика: число клеток не на своем месте
int Position::Heuristic() const {

    int not_in_right_place_count = 16;
    for (int i = 0; i < 16; ++i) {
        if (chips[i] == FINISH_POSITION.chips[i]) {
            not_in_right_place_count--;
        }
    }

    return not_in_right_place_count + distance;
}

char GetMoveSymbol(const Position &from, const Position &to) {
    char zero_diff = to.zero_place - from.zero_place;
    switch (zero_diff) {
        case 1:
            return 'L';
        case -1:
            return 'R';
        case 4:
            return 'U';
        case -4:
            return 'D';
        default:
            assert(false);
            return 0; // чтобы IDE не ругалось
    }
}

bool BFS_opt(Position &start, std::unordered_map<Position, Position> &parents) {

    // Компаратор для очереди с приоритетами
    struct CompareHeuristic {
        bool operator()(const Position &p1, const Position &p2) const {
            return p1.Heuristic() < p2.Heuristic();
        }
    };

//    parents[start] = FINISH_POSITION;

    std::unordered_set<Position> visited;

    start.distance = 0;

    std::set<Position, CompareHeuristic> position_set;
    position_set.insert(start);
    visited.insert(start);

    while (!position_set.empty()) {
        Position current = *position_set.begin();
        position_set.erase(position_set.begin());
        for (Position sibling : current.Siblings()) {


            if (visited.count(sibling) == 0) {
                position_set.insert(sibling);
                visited.insert(sibling);
            }
            if (sibling.distance > current.distance + 1) {
                sibling.distance = current.distance + 1;
                parents[sibling] = current;

                position_set.erase(sibling);
                position_set.insert(sibling);

                if (sibling.IsFinish()) {
                    return true;
                }
            } else {
                if (sibling.IsFinish()) {
                    parents[sibling] = current;
                    return true;
                }
            }


        }
    }
    return false;
}

std::vector<char> GetPath(const Position &start, const std::unordered_map<Position, Position> &parents) {
    std::vector<char> result;
    Position current = FINISH_POSITION;
    while (current != start) {
        const Position parent = parents.at(current);
        result.push_back(GetMoveSymbol(parent, current));
        current = parent;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

// Чтобы отвечать сразу, если решение не существует
bool IsSolutionExists(const Position &start) {
    int inversions = 0;
    int zero_row = 0;
    for (int i = 0; i < 16; ++i) {
        if (start.chips[i] == 0) {
            zero_row = i / 4 + 1;
        } else {
            for (int j = 0; j < i; ++j) {
                if (start.chips[i] < start.chips[j]) {
                    inversions++;
                }
            }
        }
    }
    return (inversions + zero_row) % 2 == 0;
}

std::pair<bool, std::vector<char>> SolvePuzzle(Position &start) {

    if (!IsSolutionExists(start)) {
        return std::make_pair(false, std::vector<char>());
    }

    std::unordered_map<Position, Position> parents;
    if (!BFS_opt(start, parents)) {
        return std::make_pair(false, std::vector<char>());
    } else {
        return std::make_pair(true, GetPath(start, parents));
    }
}

int main() {

//    std::vector<char> chips(16);
//    int zero_position;
//    int chip;
//    for (char i = 0; i < 16; ++i) {
//
//        std::cin >> chip;
//        chips[i] = static_cast<char>(chip);
//        if (chip == 0) {
//            zero_position = i;
//        }
//    }
//    Position start{chips, static_cast<char>(zero_position)};

//    Position start = {{1, 2, 4, 5, 3, 6, 7, 8, 9, 10, 11, 12, 13, 0, 14, 15}, 13};
    Position start = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 15}, 12};

    const auto result = SolvePuzzle(start);
    if (result.first) {
        std::cout << result.second.size() << std::endl;
        for (const auto move : result.second) {
            std::cout << move;
        }
    } else {
        std::cout << -1;
    }

    return 0;
}

