//
// Created by alexzhang on 22-11-14.
//

#ifndef COMPILER_AUTOMATIC_H
#define COMPILER_AUTOMATIC_H

#include <algorithm>
#include <cstring>
#include "Types.h"

class Automatic {
private:
    struct Node {
        char ch;
        int type;
        Node *nxt[128];

        Node() {
            for (auto &i: nxt) i = nullptr;
            type = 0;
        }

        Node(char ch) {
            for (auto &i: nxt) i = nullptr;
            type = 0;
            this->ch = ch;
        }
    };

    Node *root;
public:
    Automatic() {
        root = new Node();
    }

    ~Automatic() {
        clear();
        delete root;
    }

    void clear();

    void clear(Node *&now);

    void insert(const char *str, int type);

    std::pair<std::string, int> getNext(std::string::iterator &start, std::string::iterator end, int &line);
};


#endif //COMPILER_AUTOMATIC_H
