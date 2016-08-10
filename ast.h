#ifndef AST_H
#define AST_H

#include "type.h"
#include <vector>
#include <memory>

using std::vector;

class AST {
public:
    AST() {}
    AST(Type *t) : token(t) {}
    std::unique_ptr<Type> token;
    vector<std::unique_ptr<AST>> children;
};


#endif//AST_H
