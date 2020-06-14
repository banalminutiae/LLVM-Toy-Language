#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <vector>
//common global variable between lexer and parse
//REFACTOR: this is awful
static std::string identifier_str; //if -4
static double num_val; //if -5;

int gettok();

#endif
