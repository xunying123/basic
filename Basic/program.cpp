/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    sentences.clear();
    order.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    int i=0;
    auto itea=order.begin();
    for(;itea->first>lineNumber;itea++) {
        i++;
    }
    if(itea->first==lineNumber) {
        sentences[i]=line;
        Statement k(line);
        order[lineNumber]=k;
    }else {
        sentences.insert(sentences.begin()+i,line);
        Statement k(line);
        order[lineNumber]=k;
    }
}

void Program::removeSourceLine(int lineNumber) {
    auto itea1=order.find(lineNumber);
        if(itea1==order.end()) {
            //std::cout<<"LINE NUMBER ERROR"<<std::endl;
            return;
        }
        else {
            int i=0;
            auto itea=order.begin();
            for(;itea->first<lineNumber;itea++) {
                i++;
            }
            if(itea->first==lineNumber) {
                order.erase(lineNumber);
                sentences.erase(sentences.begin()+i);
            }
            else {
                return ;
            }
        }

}

std::string Program::getSourceLine(int lineNumber) {
    auto itea=order.find(lineNumber);
    if(itea!=order.end()) {
        return (itea->second).line;
    }
    else {
        std::string k;
        return k;
    }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    auto itea=order.find(lineNumber);
    if(itea==order.end()) {
        error("SYNTAX ERROR");
    }
    else {
        std::string lin=order[lineNumber].line;
        Statement k(lin);
        (*stmt)=k;
    }
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
    auto itea=order.find(lineNumber);
    if(itea==order.end()) {
        return NULL;
    }
    else {
        return &order[lineNumber];
    }
}

int Program::getFirstLineNumber() {
    if(order.empty()) return -1;
    else return order.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
   auto itea=order.find(lineNumber);
   auto itea1=order.end();
   itea1--;
   if(itea==itea1) {
       return -1;
   }
   else {
       itea++;
       return itea->first;
   }
}

void Program::Print() {
    auto itea=order.begin();
    while(itea!=order.end()) {
        std::cout<<(itea->second).line<<std::endl;
        itea++;
    }
}
//more func to add
//todo


