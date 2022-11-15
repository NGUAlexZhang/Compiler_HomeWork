//
// Created by alexzhang on 22-11-14.
//

#ifndef COMPILER_GRAMMAR_H
#define COMPILER_GRAMMAR_H

#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include "Types.h"

using namespace std;

class Grammar {
private:
    ofstream of;

    struct FourTuple {
        std::string op1, op2, op3, op4;

        FourTuple(std::string op1, std::string op2, std::string op3, std::string op4) {
            this->op1 = std::move(op1), this->op2 = std::move(op2), this->op3 = std::move(op3), this->op4 = std::move(
                    op4);
        }

        std::string to_string() const {
            return "(" + this->op1 + "," + this->op2 + "," + this->op3 + "," + this->op4 + ")";
        }
    };

    struct Identifier {
        string name;
        string type;
        string value;

        Identifier(string name) {
            this->name = std::move(name);
            this->type = "null";
            this->value = "null";
        }

        Identifier(string name, string type) {
            this->name = std::move(name);
            this->type = std::move(type);
            this->value = "null";
        }

        Identifier(string name, string type, string value) {
            this->name = std::move(name);
            this->type = std::move(type);
            this->value = std::move(value);
        }

        string identifierOut() const {
            return "name;" + this->name + ",type:" + this->type + ",value:" + this->value;
        }
    };

    struct IdentifierTable {
        vector<Identifier> table;

        IdentifierTable() = default;

        int getIdx(const string &name) {
            for (int i = 0; i < table.size(); i++) {
                if (table[i].name == name)
                    return i;
            }
            return -1;
        }

        Identifier getIdentifier(const string &name) {
            int idx = getIdx(name);
            if (idx == -1) {
                cout << "错误！此标识符未被声明！请声明！" << endl;
                return {"null"};
            }
            return table[idx];
        }

        bool add(const string &name, string type) {
            if (getIdx(name) != -1) {
                cout << "错误！此标识符已被声明！请勿重复声明！" << endl;
                return false;
            }
            table.emplace_back(name, type);
            return true;
        }

        bool updateValue(const string &name, string value) {
            int idx = getIdx(name);
            if (idx == -1) {
                cout << "错误！此标识符未被声明！请声明！" << endl;
                return false;
            }
            table[idx].value = std::move(value);
            return true;
        }

        void print() {
            cout << "test" << endl;
            ofstream ofs("debug.txt", ios::app);
            ofs << "--------标识符表--------" << endl;
            for (const auto &i: table) {
                ofs << i.identifierOut() << endl;
            }
            ofs << endl;
            ofs.close();
        }
    };

    struct MiddleCodeTable {
        vector<FourTuple> table;

        int NXQ() const {
            return table.size();
        }

        bool backpatch(int index, string op4) {
            table[index].op4 = op4;
            return true;
        }

        MiddleCodeTable() {}

        void add(string op1, string op2, string op3, string op4) {
            table.emplace_back(op1, op2, op3, op4);
        }

        void print() const {
            ofstream ofs("debug.txt", ios::app);
            ofs << "--------四元式表--------" << endl;
            for (int i = 0; i < table.size(); i++) {
                ofs << "(" + to_string(i) + ")" + table[i].to_string() << endl;
            }
            ofs << endl;
            for (int i = 0; i < table.size(); i++) {
                ofs << "(" + to_string(i) + ")" + table[i].to_string() << endl;
            }
            ofs << endl;
            ofs.close();
        }
    };

    struct TempVarTable {
        vector<Identifier> table;

        TempVarTable() {}

        Identifier tempVar() {
            int index = table.size();
            string s = "T" + to_string(index + 1);
            Identifier t = Identifier(s, "string", "");
            table.push_back(t);
            return t;
        }

        int getIdx(string name) {
            for (int i = 0; i < table.size(); i++) {
                if (table[i].name == name)
                    return i;
            }
            return -1;
        }

        bool updateValue(string name, string value) {
            int idx = getIdx(name);
            if (idx == -1) {
                cout << "错误！此标识符未被声明！请声明！" << endl;
                return false;
            }
            table[idx].value = value;
            return true;
        }

        void pirnt() {
            ofstream ofs("debug.txt", ios::app);
            ofs << "--------临时变量表--------" << endl;
            for (auto i: table) {
                ofs << "name:" << i.name << ",type:" << i.type << ",value:" << i.value << endl;
            }
            ofs << endl;
        }
    };

    std::vector<std::pair<std::string, int>> tokens;
    std::vector<std::pair<std::string, int>>::iterator ptr;
    std::map<int, std::string> hash;
    std::map<std::string, std::string> var;
    std::vector<FourTuple> fourTuples;
    IdentifierTable identifierTable;
    MiddleCodeTable middleCodeTable;
    TempVarTable tempVarTable;
public:
    std::string toString(const std::pair<std::string, int> &token) {
        return hash[token.second] + ", " + token.first;
    }

    explicit Grammar(std::vector<std::pair<std::string, int>> tokens) {
        this->tokens = tokens;
        for (auto it: tokens) {
            cout << it.first << "   " << it.second << endl;
        }
        ptr = this->tokens.begin();
        hash[-1] = "#";
        hash[0] = "标识符", hash[1] = "关键词", hash[2] = "类型", hash[3] = "运算符",
        hash[4] = "括号", hash[5] = "分隔符", hash[6] = "字符串", hash[7] = "数字";
        of.open("debug.txt", std::ios::out);
    }

    ~Grammar() {
        of.close();
    }

    bool match(string str, int type);

    bool P();

    bool I(string type);

    bool Ip(string type);

    bool S();

    bool Dp();

    bool A();

    bool D();

    Identifier E();

    Identifier Ep(Identifier T1);

    Identifier T();

    Identifier F();

    Identifier Tp(Identifier F1);

    Identifier K();

    bool G();

    bool N();

    bool C();

    bool L();

    void start();

    void printVars() {
        identifierTable.print();
    }

    void printTemps() {
        if (of.is_open()) of.close();
        tempVarTable.pirnt();
    }

    void printMiddles() {
        if (of.is_open()) of.close();
        middleCodeTable.print();
    }
};


#endif //COMPILER_GRAMMAR_H
