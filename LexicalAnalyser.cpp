//
// Created by alexzhang on 22-11-13.
//

#include "LexicalAnalyser.h"

std::vector<std::pair<std::string, int>> LexicalAnalyser::getTokens() {
    std::vector<std::pair<std::string, int>> ans;
    auto it = code.begin();
    while (it != code.end()) {
        auto nxt = ac.getNext(it, code.end(), line);
        if (nxt.second!=-1) {
            ans.push_back(nxt);
        }
    }
    std::cout<<ans.size()<<std::endl;
    return ans;
}
