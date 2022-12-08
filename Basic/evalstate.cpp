/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */


#include "evalstate.hpp"
#include "Utils/error.hpp"

//using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
    /* Empty */
}

EvalState::~EvalState() {
    /* Empty */
}

void EvalState::setValue(std::string var, int value) {
    if((var=="LET")||(var=="REM")||(var=="PRINT")||(var=="INPUT")||(var=="END")||(var=="GOTO")||(var=="IF")||(var=="THEN")||(var=="RUN")||(var=="LIST")||(var=="CLEAR")||(var=="QUIT")||(var=="HELP")){
        error ("SYNTAX ERROR");
    }
    if(isDefined(var)) symbolTable[var] = value;
    else symbolTable.emplace(var,value);
}

int EvalState::getValue(std::string var) {
    if(isDefined(var)) return symbolTable[var];
    else return 0;
}

bool EvalState::isDefined(std::string var) {
    return symbolTable.find(var)!=symbolTable.end();
}

void EvalState::Clear() {
    symbolTable.clear();
}