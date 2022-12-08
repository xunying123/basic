/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include<ctype.h>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

int processLine(std::string line, Program &program, EvalState &state);

bool check(Expression *xx, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            int k = processLine(input, program, state);
            if (k == -1) return 0;
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

int processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    // scanner.addWordCharacters("-");
    scanner.setInput(line);
    std::string token = scanner.nextToken();
    if (token[0] < '0' || token[0] > '9') {
        if (scanner.hasMoreTokens()) {
            if (token == "LET") {
                try {
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
                    //  scanner1.addWordCharacters("-");
                    scanner1.setInput(www);
                    Expression *exp = parseExp(scanner1);
                    if (!check(exp, state)) {
                        delete exp;
                        error("VARIABLE NOT DEFINED");
                    }
                    int value = exp->eval(state);
                    delete exp;
                    state.setValue(a, value);
                    return 0;
                } catch (std::string msg) {
                    std::cout << msg << std::endl;
                }

            }
            if (token == "PRINT") {
                Expression *exp = parseExp(scanner);
                if (!check(exp, state)) {
                    delete exp;
                    error("VARIABLE NOT DEFINED");
                }
                try {
                    int value = exp->eval(state);
                    if (value > 9999999) {
                        delete exp;
                        error("DIVIDE BY ZERO");
                    }
                    delete exp;
                    std::cout << value << std::endl;
                    return 0;
                }
                catch (std::string msg) {
                    delete exp;
                    std::cout << msg << std::endl;
                }
            }
            if (token == "INPUT") {
                std::string in1, in2;
                std::string a = scanner.nextToken();
                std::cout << " ? ";
                bool sign;
                while(true) {
                    sign=1;
                    getline(std::cin, in1);
                    if (in1[0] == '-') {
                        sign = 0;
                        in2 = "";
                        for (int i = 1; i < in1.length(); i++) {
                            in2 += in1[i];
                        }
                    } else in2 = in1;
                    TokenScanner scanner;
                    scanner.ignoreWhitespace();
                    scanner.scanNumbers();
                    scanner.setInput(in2);
                    std::string token = scanner.nextToken();
                    if((scanner.getTokenType(token)==NUMBER)&&(!scanner.hasMoreTokens())) {
                        if(in2[1]=='.') {
                            std::cout<<"INVALID NUMBER"<<std::endl;
                            std::cout<<" ? ";
                            continue;
                        }
                        break;
                    }else {
                        std::cout<<"INVALID NUMBER"<<std::endl;
                        std::cout<<" ? ";
                        continue;
                    }
                }
                int value = stringToInteger(in2);
                if (sign == 0) value = 0 - value;
                state.setValue(a, value);

            }
        } else {
            if (token == "CLEAR") {
                program.clear();
                state.Clear();
                return 0;
            }
            if (token == "QUIT") {
                program.clear();
                state.Clear();
                exit(0);
                return -1;
            }
            if (token == "LIST") {
                program.Print();
            }
            if (token == "RUN") {
                auto itea = program.order.begin();
                (itea->second).execute(state, program);
            }
        }
    } else {
        if (scanner.hasMoreTokens()) {
            int k = stringToInteger(token);
            program.addSourceLine(k, line);
        } else {
            int k = stringToInteger(line);
            program.removeSourceLine(k);
        }
    }
}

bool check(Expression *xx, EvalState &state) {
    if (xx->getType() == CONSTANT) return true;
    if (xx->getType() == IDENTIFIER) {
        int kk = xx->eval(state);
        if (kk == -99999999) return false;
        else return true;
    }
    if (xx->getType() == COMPOUND) {
        int kk = xx->eval(state);
        if (kk < -99999) return false;
        else return true;
    }
}

