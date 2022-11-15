//
// Created by alexzhang on 22-11-13.
//
#ifndef COMPILER_LEXICALANALYSER_H
#define COMPILER_LEXICALANALYSER_H

#include <string>
#include <vector>
#include "Types.h"
#include "Automatic.h"
#include <vector>
#include <iostream>
class LexicalAnalyser {
private:
    std::string code;
    Automatic ac;
    int line;
    std::vector<const char *> types{"string"},
            operators{"<", ">", "=", "<=", ">=", "==", "+", "*"},
            brackets{"(", ")", "start", "end"}, keyWords{"if", "else", "do", "while"}, stops{",", ";"};
public:
    explicit LexicalAnalyser(std::string code) {
        this->code = code;
        line = 1;
        for (auto it: types) {
            ac.insert(it, TYPE);
        }
        for (auto it: operators) {
            ac.insert(it, OPERATOR);
        }
        for (auto it: brackets) {
            ac.insert(it, BRACKET);
        }
        for (auto it: keyWords) {
            ac.insert(it, KEY_WORD);
        }
        for (auto it: stops) {
            ac.insert(it, STOP);
        }
    }

    std::vector<std::pair<std::string, int>> getTokens();
};


#endif //COMPILER_LEXICALANALYSER_H
