#include <fstream>
#include <iostream>
#include <vector>
#include "LexicalAnalyser.h"
#include "Grammar.h"
std::string getCode(char *file) {
    std::ifstream is(file);
    if (!is) {
        puts("No this file");
        exit(-1);
    }
    std::string ans = "";
    char ch;
    while (is >> ch) {
        ans += ch;
    }
    is.close();
    return ans;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        puts("Please input the file name");
        return -1;
    }
    if (argc == 2) {
        auto code = getCode(argv[1]);
        LexicalAnalyser la(code);
        auto twoTuples = la.getTokens();
        twoTuples.emplace_back("#",-1);
        Grammar gm(twoTuples);
        gm.start();
        gm.printVars();
        gm.printMiddles();
        gm.printTemps();
    }
    return 0;
}