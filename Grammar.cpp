
#include "Grammar.h"

bool Grammar::match(string str, int type) {
    if (ptr == tokens.end()) return false;
    if ((str == ptr->first && type == ptr->second) || (type == 0 && ptr->second == 0) ||
        (type == 6 && ptr->second == 6) || (type == 7 && ptr->second == 7)) {
        of << "[语]文法：" << hash[type] << ",读头:" << hash[type] << ",匹配成功" << endl;
        ptr++;
        of << "[词]下一个单词:" << toString(*ptr) << endl;
        return true;
    }
    return false;
}

bool Grammar::P() {
    of << "[语]<程序> -> <变量说明部分>;<语句部分>" << endl;
    of << "[语]<变量说明部分> -> string<标识符列表>" << endl;
    if (!match("string", TYPE)) return false;
    string name = ptr->first;
    if (!I("string")) return false;
    if (!match(";", STOP)) return false;
//    puts("a");
    if (!D()) return false;
    return true;
}

bool Grammar::I(string type) {
    of << "[语]<标识符列表> -> <标识符><标识符列表prime>" << endl;
    string name = ptr->first;
    if (match("", 0)) {
        if (!identifierTable.add(name, type)) {
            return false;
        }
        of << "[翻]填写标识符" + name + "类型为string" << endl;
        if (Ip(type)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool Grammar::Ip(string type) {
    if (ptr->second != STOP) return false;
    if (ptr->first == ",") {
        of << "[语]选择<标识符列表prime> -> ,<标识符><标识符列表prime>" << endl;
        match(",", STOP);
        string name = ptr->first;
        if (!match("", 0)) return false;
        if (!identifierTable.add(name, type)) return false;
        of << "[翻]填写标识符" + name + "类型为string" << endl;
        if (!Ip(type)) return false;
        return true;
    }
    of << "[语]选择<标识符列表prime> -> 空串" << endl;
    return true;
}

bool Grammar::D() {
    of << "[语]<语句部分> -> <语句>;<语句部分prime>" << endl;
    if (!S()) return false;
    if (!match(";", STOP)) return false;
//    puts("b");
    if (!Dp()) return false;
    return true;
}

bool Grammar::Dp() {
    if (ptr->second == 0 || ptr->second == KEY_WORD) {
        of << "[语]选择<语句部分prime> -> <语句>;<语句部分prime>" << endl;
        if (!S()) return false;
        if (!match(";", STOP)) return false;
//        puts("c");
        if (!Dp()) return false;
        return true;
    } else if (ptr ->second==-1 || (ptr->first == "end" && ptr->second == BRACKET)) {
        of << "[语]选择<语句部分prime> -> 空串" << endl;
        return true;
    }
    return false;
}

bool Grammar::S() {
    if (ptr->second == 0) {
        of << "[语]选择<语句> -> <赋值语句>" << endl;
        if (A()) return true;
        return false;
    } else if (ptr->first == "if" && ptr->second == KEY_WORD) {
        of << "[语]选择<语句> -> <条件语句>" << endl;
        if (C()) return true;
        return false;
    } else if (ptr->first == "do" && ptr->second == KEY_WORD) {
        of << "[语]选择<语句> -> <循环语句>" << endl;
        if (L()) return true;
        return false;
    }
    return false;
}

bool Grammar::A() {
    of << "[语]<赋值语句> -> <标识符>=<表达式>" << endl;
    string name = ptr->first;
    if (!match("", 0)) return false;
    of << "[翻]获取赋值语句标识符" + name << endl;
    if (!match("=", OPERATOR)) return false;
    Identifier E1 = E();

    if (E1.value == "null") {
        of << "错误!标识符" + E1.name + "缺少初始值" << endl;
        return false;
    }
    if (!identifierTable.updateValue(name, E1.value))
        return false;
    middleCodeTable.add("=", E1.name, "null", name);
    of << "[翻]更新标识符" + name + "值为" << E1.value << endl;
    of << "[翻]输出赋值语句四元式(=," + E1.name + ",null," + name + ")" << endl;
    return true;
}

bool Grammar::C() {
    of << "[语]<条件语句> -> if(<条件>)<嵌套语句>else<嵌套语句>" << endl;
    if (!match("if", KEY_WORD)) return false;
    if (!match("(", BRACKET)) return false;
    Identifier K1 = K();
    if (K1.name == "null") return false;
    int trueExit = middleCodeTable.NXQ() + 2;
    of << "[翻]产生if语句真出口跳转四元式" << endl;
    middleCodeTable.add("jnz", K1.name, "null", to_string(trueExit));
    int falseExit = middleCodeTable.NXQ();
    of << "[翻]产生if语句假出口跳转四元式" << endl;
    middleCodeTable.add("j", "null", "null", "0");
    if (!match(")", BRACKET)) return false;
    if (!N()) return false;
    of << "[翻]回填if语句假出口地址" << endl;
    middleCodeTable.backpatch(falseExit, to_string(middleCodeTable.NXQ()));
    int Exit = middleCodeTable.NXQ();
    of << "[翻]产生if语句出口跳转四元式" << endl;
    middleCodeTable.add("j", "null", "null", "0");
    if (!match("else", KEY_WORD)) return false;
    if (!N()) return false;
    of << "[翻]回填if语句出口地址" << endl;
    middleCodeTable.backpatch(Exit, to_string(middleCodeTable.NXQ()));
    return true;
}

bool Grammar::L() {
    of << "[语]<循环语句> -> do<嵌套语句>while(<条件>)" << endl;
    if (!match("do", KEY_WORD)) return false;
    int entrance = middleCodeTable.NXQ();
    if (!N()) return false;
    if (!match("while", KEY_WORD)) return false;
    if (!match("(", BRACKET)) return false;
    Identifier K1 = K();
    if (!match(")", BRACKET)) return false;
    middleCodeTable.add("jnz", K1.name, "null", to_string(entrance));
    return true;
}

Grammar::Identifier Grammar::E() {
    of << "[语]<表达式> -> <项><表达式prime>" << endl;
    Identifier T1 = T();
    if (T1.name == "null") {
        return {"null"};
    }
    Identifier Ep1 = Ep(T1);
    if (Ep1.name == "null") {
        return {"null"};
    }
    return Ep1;
}

Grammar::Identifier Grammar::Ep(Identifier T1) {
    if (ptr->first == "+" && ptr->second == OPERATOR) {
        of << "[语]选择<表达式prime> -> <连接运算符><项><表达式prime>" << endl;
        if (!match("+", OPERATOR)) return {"null"};
        Identifier T2 = T();
        if (T2.name == "null") {
            return {"null"};
        }
        Identifier t = tempVarTable.tempVar();
        t.type = T1.type;
        t.value = T1.value + T2.value;
        if (!tempVarTable.updateValue(t.name, t.value))
            return {"null"};
        middleCodeTable.add("+", T1.name, T2.name, t.name);
        of << "[翻]创建加法运算临时变量" << endl;
        of << "[翻]产生加法运算四元式(+," + T1.name + "," + T2.name + "," + t.name + ")" << endl;
        Identifier Ep2 = Ep(t);
        if (Ep2.name == "null") {
            return {"null"};
        }
        return Ep2;
    } else if ((ptr->first == ")" && ptr->second == BRACKET) ||
               (ptr->first != "=" && ptr->first != "+" && ptr->first != "*" && ptr->second == OPERATOR) ||
               (ptr->first == ";" && ptr->second == STOP) ||
               ((ptr->first == "else" || ptr->first == "while") && ptr->second == KEY_WORD)) {
        of << "[语]选择<表达式prime> -> 空串" << endl;
        return T1;
    }
    return {"null"};
}

Grammar::Identifier Grammar::T() {
    of << "[语]<项> -> <因子><项prime>" << endl;
    Identifier F1 = F();
    if (F1.name == "null") return {"null"};
    Identifier Tp1 = Tp(F1);
    if (Tp1.name == "null") return {"null"};
    return Tp1;
}

Grammar::Identifier Grammar::Tp(Identifier F1) {
    if (ptr->first == "*" && ptr->second == OPERATOR) {
        of << "[语]选择<项prime> -> <重复运算符><数字><项prime>" << endl;
        if (!match("*", OPERATOR)) return {"null"};
        string s = ptr->first;
        if (!match("", NUMBER)) return {"null"};
        int m = stoi(s.c_str());
        Identifier t = tempVarTable.tempVar();
        t.type = F1.type;
        for (int i = 0; i < m; i++) {
            t.value += F1.value;
        }
        if (!tempVarTable.updateValue(t.name, t.value)) {
            return {"null"};
        }
        middleCodeTable.add("*", F1.name, s, t.name);
        of << "[翻]创建乘法运算临时变量" << endl;
        of << "[翻]产生乘法运算四元式(*," + F1.name + "," + s + "," + t.name + ")" << endl;
        Identifier Tp2 = Tp(t);
        if (Tp2.name == "null") {
            return {"null"};
        }
        return Tp2;
    } else if ((ptr->first == ")" && ptr->second == BRACKET) ||
               (ptr->first != "=" && ptr->first != "*" && ptr->second == OPERATOR) ||
               (ptr->first == ";" && ptr->second == STOP) ||
               ((ptr->first == "else" || ptr->first == "while") && ptr->second == KEY_WORD)) {
        of << "[语]选择<项prime> -> 空串" << endl;
        return F1;
    }
    return {"null"};
}

Grammar::Identifier Grammar::F() {
    if (ptr->second == 0) {
        of << "[语]选择<因子> -> <标识符>" << endl;
        string name = ptr->first;
        if (!match("", 0)) return {"null"};
        Identifier q = identifierTable.getIdentifier(name);
        if (q.value == "null") {
            of << "错误！标识符" + name + "缺少初始值！" << endl;
            return {"null"};
        }
        of << "[翻]获取表达式中标识符值" << endl;
        return identifierTable.getIdentifier(name);
    } else if (ptr->second == STRING) {
        of << "[语]选择<因子> -> <字符串>" << endl;
        string name = ptr->first;
        if (!match("", STRING)) return {"null"};
        of << "[翻]创建字符串的临时变量" << endl;
        Identifier t = Identifier(name, "string", name);
        return t;
    } else if (ptr->first == "(" && ptr->second == BRACKET) {
        of << "[语]选择<因子> -> (表达式)" << endl;
        if (!match("(", BRACKET)) return {"null"};
        Identifier E1 = E();
        if (E1.name == "null") return {"null"};
        if (!match(")", BRACKET)) return {"null"};
        return E1;
    }
    return {"null"};
}

Grammar::Identifier Grammar::K() {
    of << "[语]<条件> -> <表达式><关系运算符><表达式>" << endl;
    Identifier E1 = E();
    if (E1.name == "null") return {"null"};
    string op = ptr->first;
    if (ptr->first != "+" && ptr->first != "=" && ptr->first != "*" && ptr->second == OPERATOR) {
        of<<"[语]文法：逻辑运算符,读头:逻辑运算符,匹配成功"<<endl;
        ptr++;
        of << "[词]下一个单词:" << toString(*ptr) << endl;
        Identifier E2 = E();
        if (E2.name == "null") {
            return {"null"};
        }
        Identifier t = tempVarTable.tempVar();
        t.type = "bool";
        tempVarTable.updateValue(t.name, "false");
        if (op == "<" && E1.value < E2.value) tempVarTable.updateValue(t.name, "true");
        if (op == ">" && E1.value > E2.value) tempVarTable.updateValue(t.name, "true");
        if (op == "<>" && E1.value != E2.value) tempVarTable.updateValue(t.name, "true");
        if (op == ">=" && E1.value >= E2.value) tempVarTable.updateValue(t.name, "true");
        if (op == "<=" && E1.value <= E2.value) tempVarTable.updateValue(t.name, "true");
        if (op == "==" && E1.value == E2.value) tempVarTable.updateValue(t.name, "true");
        of << "[翻]产生" + op + "运算四元式(" + op + "," + E1.name + "," + E2.name + "," + t.name + ")" << endl;
        middleCodeTable.add(op, E1.name, E2.name, t.name);
        return t;
    }
    return {"null"};
}

bool Grammar::G() {
    of << "[语]<复合语句> -> start<语句部分>end" << endl;
    if (match("start", BRACKET)) {
        if (D()) {
            if (match("end", BRACKET)) return true;
            return false;
        }
        return false;
    }
    return false;
}

bool Grammar::N() {
    if (ptr->second == 0 || ptr->second == KEY_WORD) {
        of << "[语]选择<嵌套语句> -> 语句" << endl;
        if (S()) {
            return true;
        } else {
            return false;
        }
    } else if (ptr->first == "start" && ptr->second == BRACKET) {
        of << "[语]选择<嵌套语句> -> <复合语句>" << endl;
        if (G()) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void Grammar::start() {
    if(!P()) of<<"error"<<endl;
    else of<<"结束"<<endl;
    of.close();

}
