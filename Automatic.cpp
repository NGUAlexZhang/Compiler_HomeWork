//
// Created by alexzhang on 22-11-14.
//

#include "Automatic.h"
#include <iostream>

void Automatic::insert(const char *str, int type) {
    int len = strlen(str);
    Node *ptr = root;
    for (int i = 0; i < len; i++) {
        char ch = str[i];
        if (ptr->nxt[ch] == nullptr) {
            ptr->nxt[ch] = new Node(ch);
        }
        ptr = ptr->nxt[ch];
    }
    ptr->type = type;
}

void Automatic::clear() {
    for (auto &nxt: root->nxt) {
        clear(nxt);
    }
}

void Automatic::clear(Automatic::Node *&now) {
    if (now == nullptr) return;
    for (auto &nxt: now->nxt) {
        clear(nxt);
    }
    delete now;
    now = nullptr;
}

std::pair<std::string, int> Automatic::getNext(std::string::iterator &start, std::string::iterator end, int &line) {
    int tmpLine = line;
    std::string ans = "";
    auto &now = start;
    auto ptr = root;
    bool fail = false;
    while (now != end && *now == ' ' || *now == '\t' || *now == '\n' || *now == '\r') {
        if (*now == '\r' || *now == '\n') line++;
        now++;
    }
    if (now == end) return std::pair<std::string, int>("", -1);
    if (*now == '\"') {
        now++;
        while (now != end && *now != '\"') {
            ans += *now;
            now++;
        }
        if (*now == '\"') now++;
        else {
            printf("lack of \" in line %d", tmpLine);
            exit(-1);
        }
        return std::pair<std::string, int>(ans, STRING);
    }
    if (*now <= '9' && *now >= '0') {
        while (now != end && *now <= '9' && *now >= '0') {
            if (now != end && *now == ' ' || *now == '\t' || *now == '\n' || *now == '\r') {
                if (*now == '\r' || *now == '\n') line++;
                now++;
                continue;
            }
            ans += *now;
            now++;
        }
        return std::pair<std::string, int>(ans, NUMBER);
    }
    if (!((*now <= 'z' && *now >= 'a') || (*now <= '9' && *now >= '0'))) {
        if (root->nxt[*now] == nullptr) {
            printf("unknown character %c in line %d", *now, line);
            exit(-1);
        }
        while (now != end) {
            if (*now == ' ' || *now == '\t' || *now == '\n' || *now == '\r') {
                if (*now == '\r' || *now == '\n') line++;
                now++;
                continue;
            }
            if (ptr->nxt[*now] == nullptr) break;
            ans += *now;
            ptr = ptr->nxt[*now];
            now++;
        }
        return std::pair<std::string, int>(ans, ptr->type);
    }
    while (now != end) {
        if (*now == ' ' || *now == '\t' || *now == '\n' || *now == '\r') {
            if (*now == '\r' || *now == '\n') line++;
            now++;
            continue;
        }
        if (!((*now <= 'z' && *now >= 'a') || (*now <= '9' && *now >= '0'))) {
            break;
        }
        if (fail) {
            ans += *now;
            now++;
            continue;
        }
        if (ptr->nxt[*now] == nullptr) {
            fail = true;
        } else {
            ptr = ptr->nxt[*now];
        }
        ans += *now;
        now++;
        if (ptr->type != 0) break;
    }
    return std::pair<std::string, int>(ans, ptr->type);
}
