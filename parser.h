#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <sstream>
#include "type.h"
#include "ast.h"

vector<string> tokenize(string s);
AST *createAst(vector<string> tokens);
AST* parse(string line);


#endif
