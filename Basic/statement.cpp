/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"

/* Implementation of the Statement class */

int stringToInt(std::string str);


Statement::Statement(std::string s) {
    line = s;
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    //  scanner.addWordCharacters("-");
    scanner.setInput(s);
    std::string token = scanner.nextToken();
    token = scanner.nextToken();
    op = token;
}

void Statement::execute(EvalState &state, Program &program) {
    if (op == "INPUT") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        scanner.setInput(line);
        std::string mm = scanner.nextToken();
        std::string in1, in2;
        std::string token= scanner.nextToken();
        std::string a = scanner.nextToken();
        std::cout << " ? ";
        bool sign;
        while (true) {
            sign = 1;
            getline(std::cin, in1);
            if (in1[0] == '-') {
                sign = 0;
                in2 = "";
                for (int i = 1; i < in1.length(); i++) {
                    in2 += in1[i];
                }
            } else in2 = in1;
            TokenScanner scanner11;
            scanner11.ignoreWhitespace();
            scanner11.scanNumbers();
            scanner11.setInput(in2);
            std::string token = scanner11.nextToken();
            if ((scanner11.getTokenType(token) == NUMBER) && (!scanner11.hasMoreTokens())) {
                if (in2[1] == '.') {
                    std::cout << "INVALID NUMBER" << std::endl;
                    std::cout << " ? ";
                    continue;
                }
                break;
            } else {
                std::cout << "INVALID NUMBER" << std::endl;
                std::cout << " ? ";
                continue;
            }
        }
        int value = stringToInteger(in2);
        if (sign == 0) value = 0 - value;
        state.setValue(a, value);
        int m= stringToInteger(mm);
        int k = program.getNextLineNumber(m);
        if (k == -1) return;
        program.order[k].execute(state, program);
    }
    if (op == "REM") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        //   scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        int k = program.getNextLineNumber(m);
        if (k == -1) return;
        program.order[k].execute(state, program);
    }

    if (op == "LET") {
        try {
            TokenScanner scanner;
            scanner.ignoreWhitespace();
            scanner.scanNumbers();
            //    scanner.addWordCharacters("-");
            scanner.setInput(line);
            std::string token = scanner.nextToken();
            int m = stringToInteger(token);
            token = scanner.nextToken();
            std::string a = scanner.nextToken();
            token = scanner.nextToken();
            token = scanner.nextToken();
            std::string www = token;
            www += " ";
            while (scanner.hasMoreTokens()) {
                token = scanner.nextToken();
                www = www + token + " ";
            }
            TokenScanner scanner1;
            scanner1.ignoreWhitespace();
            scanner1.scanNumbers();
            //    scanner1.addWordCharacters("-");
            scanner1.setInput(www);
            Expression *exp = parseExp(scanner1);
            int value = exp->eval(state);
            delete exp;
            state.setValue(a, value);
            int k = program.getNextLineNumber(m);
            if (k == -1) return;
            program.order[k].execute(state, program);
        }
        catch (std::string msg) {
            std::cout << msg << std::endl;
        }
    }
    if (op == "PRINT") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        //    scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int m = stringToInteger(token);
        token = scanner.nextToken();
        token = scanner.nextToken();
        std::string ss=token;
        ss+=" ";
        while(scanner.hasMoreTokens()) {
            token = scanner.nextToken();
            ss=ss+token+" ";
        }
        TokenScanner scanner11;
        scanner11.ignoreWhitespace();
        scanner11.scanNumbers();
        //    scanner.addWordCharacters("-");
        scanner11.setInput(ss);
        Expression *exp1 = parseExp(scanner11);
        int value = exp1->eval(state);
        delete exp1;
        if (value == -99999999) {
            error("VARIABLE NOT DEFINED");
        }
        std::cout << value << std::endl;
        int k = program.getNextLineNumber(m);
        if (k == -1) return;
        program.order[k].execute(state, program);
    }
    if (op == "IF") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        //   scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        int mm = stringToInteger(token);
        token = scanner.nextToken();
        std::string a = scanner.nextToken();
        a += " ";
        token = scanner.nextToken();
        while (token != "<" && token != "=" && token != ">") {
            a = a + token + " ";
            token = scanner.nextToken();
        }
        char oop = token[0];
        token = scanner.nextToken();
        TokenScanner scanner1, scanner2;
        scanner1.ignoreWhitespace();
        scanner1.scanNumbers();
        //    scanner1.addWordCharacters("-");
        scanner1.setInput(a);
        Expression *exp1 = parseExp(scanner1);
        int value1 = exp1->eval(state);
        delete exp1;
        scanner2.ignoreWhitespace();
        scanner2.scanNumbers();
        // scanner2.addWordCharacters("-");
        a = "";
        while (token != "THEN") {
            a = a + token + " ";
            token = scanner.nextToken();
        }
         int value2;
        scanner2.setInput(a);
        if(scanner2.hasMoreTokens()) {
            Expression *exp2 = parseExp(scanner2);
            value2 = exp2->eval(state);
            delete exp2;
        }
        else value2= stringToInteger(a);
        if (((oop == '=') && (value1 == value2)) || ((oop == '<') && (value1 < value2)) ||
            ((oop == '>') && (value1 > value2))) {
            token = scanner.nextToken();
            int m = stringToInteger(token);
            program.order[m].execute(state, program);
        } else {
            int k = program.getNextLineNumber(mm);
            if (k == -1) return;
            program.order[k].execute(state, program);
        }
    }
    if (op == "GOTO") {
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.scanNumbers();
        //    scanner.addWordCharacters("-");
        scanner.setInput(line);
        std::string token = scanner.nextToken();
        token = scanner.nextToken();
        token = scanner.nextToken();
        int m = stringToInteger(token);
        auto itea=program.order.find(m);
        if(itea==program.order.end()) {
            error("LINE NUMBER ERROR");
        }
        program.order[m].execute(state, program);
    }
    if (op == "END") {
        return;
    }
}

Statement::Statement() {
    op = "";
    line = "";
}

