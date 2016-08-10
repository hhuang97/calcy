#ifndef EVAL_H
#define EVAL_H

#include <map>
#include <string>
#include <vector>
#include <memory>
#include "ast.h"
#include "type.h"

using namespace std;

void initializeBuiltins();

Type* eval(AST *ast);

#endif
