
#include "eval.h"
#include <iostream>
#include <cmath>

using namespace std;

static map<string,Type* (*)(vector<Type *>)> functions;

static map<string,Type *> variables;

bool validArgs(int numArgs, vector<CalcyType> types, vector<Type *> args) {
    if (numArgs >= 0 && args.size() != numArgs) {
        return false;
    }
    for (size_t i = 0; i < args.size(); i++) {
        bool matches = false;
        for (size_t t = 0; t < types.size(); t++) {
            if (args.at(i)->type == types.at(t)) {
                matches = true;
            }
        }
        if (matches == false)
            return false;
    }
    return true;
}

static Type *calcyAdd(vector<Type *> args) {
    bool doubles = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i)->type != CALCY_INT && args.at(i)->type != CALCY_DOUBLE) {
            cout << "Incorrect argument type given to +" << endl;
            return new TypeNothing();
        }
        if (args.at(i)->type == CALCY_DOUBLE)
            doubles = true;
    }
    if (doubles) {
        double sum = 0;
        for (auto i = args.begin(); i != args.end(); i++) {
            if ((*i)->type == CALCY_INT) {
                TypeInt *n = dynamic_cast<TypeInt *>(*i);
                sum += n->value;
            }
            if ((*i)->type == CALCY_DOUBLE) {
                TypeDouble *n = dynamic_cast<TypeDouble *>(*i);
                sum += n->value;
            }
        }
        return new TypeDouble(sum);
    } else {
        int sum = 0;
        for (auto i = args.begin(); i != args.end(); i++) {
            TypeInt *n = dynamic_cast<TypeInt *>(*i);
            sum += n->value;
        }
        return new TypeInt(sum);
    }
}

static Type *calcySub(vector<Type *> args) {
    bool doubles = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i)->type != CALCY_INT && args.at(i)->type != CALCY_DOUBLE) {
            cout << "Incorrect argument type given to +" << endl;
            return new TypeNothing();
        }
        if (args.at(i)->type == CALCY_DOUBLE)
            doubles = true;
    }
    if (doubles) {
        double diff = 0;
        for (auto i = args.begin(); i != args.end(); i++) {
            if ((*i)->type == CALCY_INT) {
                TypeInt *n = dynamic_cast<TypeInt *>(*i);
                diff -= n->value;
                if (i == args.begin())
                    diff *= -1;
            }
            if ((*i)->type == CALCY_DOUBLE) {
                TypeDouble *n = dynamic_cast<TypeDouble *>(*i);
                diff -= n->value;
                if (i == args.begin())
                    diff *= -1;
            }
        }
        return new TypeDouble(diff);
    } else {
        int diff = 0;
        for (auto i = args.begin(); i != args.end(); i++) {
            TypeInt *n = dynamic_cast<TypeInt *>(*i);
            diff -= n->value;
            if (i == args.begin())
                diff *= -1;
        }
        return new TypeInt(diff);
    }
}

static Type *calcyMult(vector<Type *> args) {
    bool doubles = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i)->type != CALCY_INT && args.at(i)->type != CALCY_DOUBLE) {
            cout << "Incorrect argument type given to +" << endl;
            return new TypeNothing();
        }
        if (args.at(i)->type == CALCY_DOUBLE)
            doubles = true;
    }
    if (doubles) {
        double prod = 1;
        for (auto i = args.begin(); i != args.end(); i++) {
            if ((*i)->type == CALCY_INT) {
                TypeInt *n = dynamic_cast<TypeInt *>(*i);
                prod *= n->value;
            }
            if ((*i)->type == CALCY_DOUBLE) {
                TypeDouble *n = dynamic_cast<TypeDouble *>(*i);
                prod *= n->value;
            }
        }
        return new TypeDouble(prod);
    } else {
        int prod = 0;
        for (auto i = args.begin(); i != args.end(); i++) {
            TypeInt *n = dynamic_cast<TypeInt *>(*i);
            prod *= n->value;
        }
        return new TypeInt(prod);
    }
}

static Type *calcyDiv(vector<Type *> args) {
    bool doubles = false;
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i)->type != CALCY_INT && args.at(i)->type != CALCY_DOUBLE) {
            cout << "Incorrect argument type given to +" << endl;
            return new TypeNothing();
        }
        if (args.at(i)->type == CALCY_DOUBLE)
            doubles = true;
    }
    if (doubles) {
        double quot = 1;
        for (auto i = args.begin(); i != args.end(); i++) {
            if ((*i)->type == CALCY_INT) {
                TypeInt *n = dynamic_cast<TypeInt *>(*i);
                quot /= n->value;
                if (i == args.begin())
                    quot = 1 / quot;
            }
            if ((*i)->type == CALCY_DOUBLE) {
                TypeDouble *n = dynamic_cast<TypeDouble *>(*i);
                quot /= n->value;
                if (i == args.begin())
                    quot = 1 / quot;
            }
        }
        return new TypeDouble(quot);
    } else {
        int quot = 1;
        for (auto i = args.begin(); i != args.end(); i++) {
            TypeInt *n = dynamic_cast<TypeInt *>(*i);
            quot /= n->value;
            if (i == args.begin())
                quot = 1 / quot;
        }
        return new TypeInt(quot);
    }
}

static Type *calcyMod(vector<Type *> args) {
    if (args.size() != 2) {
        cout << "Invalid number of args to %" << endl;
        return new TypeNothing();
    }
    for (size_t i = 0; i < args.size(); i++) {
        if (args.at(i)->type != CALCY_INT) {
            cout << "Incorrect argument type given to +" << endl;
            return new TypeNothing();
        }
    }
    int a = dynamic_cast<TypeInt *>(args.at(0))->value;
    int b = dynamic_cast<TypeInt *>(args.at(1))->value;
    return new TypeInt(a % b);
}

static Type *calcyIf(vector<Type *> args) {
    if (args.size() != 3) {
        cout << "Invalid number of args to if" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type != CALCY_BOOL) {
        cout << "Conditional not of type boolean" << endl;
        return new TypeNothing();
    } else {
        bool cond = dynamic_cast<TypeBool *>(args.at(0))->value;
        if (cond) {
            return args.at(1);
        } else {
            return args.at(2);
        }
    }
}

static Type *calcyLt(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_INT);
    types.push_back(CALCY_DOUBLE);
    types.push_back(CALCY_CHAR);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to <" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type == CALCY_INT) {
        int a = dynamic_cast<TypeInt *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
    }
    if (args.at(0)->type == CALCY_DOUBLE) {
        double a = dynamic_cast<TypeDouble *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
    }
    if (args.at(0)->type == CALCY_CHAR) {
        char a = dynamic_cast<TypeChar *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a < b);
        }
    }
}

static Type *calcyLte(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_INT);
    types.push_back(CALCY_DOUBLE);
    types.push_back(CALCY_CHAR);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to <=" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type == CALCY_INT) {
        int a = dynamic_cast<TypeInt *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
    }
    if (args.at(0)->type == CALCY_DOUBLE) {
        double a = dynamic_cast<TypeDouble *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
    }
    if (args.at(0)->type == CALCY_CHAR) {
        char a = dynamic_cast<TypeChar *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a <= b);
        }
    }
}

static Type *calcyEq(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_INT);
    types.push_back(CALCY_DOUBLE);
    types.push_back(CALCY_CHAR);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to =" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type == CALCY_INT) {
        int a = dynamic_cast<TypeInt *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
    }
    if (args.at(0)->type == CALCY_DOUBLE) {
        double a = dynamic_cast<TypeDouble *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
    }
    if (args.at(0)->type == CALCY_CHAR) {
        char a = dynamic_cast<TypeChar *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a == b);
        }
    }
}

static Type *calcyGte(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_INT);
    types.push_back(CALCY_DOUBLE);
    types.push_back(CALCY_CHAR);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to >=" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type == CALCY_INT) {
        int a = dynamic_cast<TypeInt *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
    }
    if (args.at(0)->type == CALCY_DOUBLE) {
        double a = dynamic_cast<TypeDouble *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
    }
    if (args.at(0)->type == CALCY_CHAR) {
        char a = dynamic_cast<TypeChar *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a >= b);
        }
    }
}

static Type *calcyGt(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_INT);
    types.push_back(CALCY_DOUBLE);
    types.push_back(CALCY_CHAR);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to >" << endl;
        return new TypeNothing();
    }
    if (args.at(0)->type == CALCY_INT) {
        int a = dynamic_cast<TypeInt *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
    }
    if (args.at(0)->type == CALCY_DOUBLE) {
        double a = dynamic_cast<TypeDouble *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
    }
    if (args.at(0)->type == CALCY_CHAR) {
        char a = dynamic_cast<TypeChar *>(args.at(0))->value;
        if (args.at(1)->type == CALCY_INT) {
            int b = dynamic_cast<TypeInt *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_DOUBLE) {
            double b = dynamic_cast<TypeDouble *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
        if (args.at(1)->type == CALCY_CHAR) {
            char b = dynamic_cast<TypeChar *>(args.at(1))->value;
            return new TypeBool(a > b);
        }
    }
}

static Type *calcyAnd(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_BOOL);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to <" << endl;
        return new TypeNothing();
    }
    bool a = dynamic_cast<TypeBool *>(args.at(0))->value;
    bool b = dynamic_cast<TypeBool *>(args.at(1))->value;
    return new TypeBool(a && b);
}

static Type *calcyOr(vector<Type *> args) {
    vector<CalcyType> types;
    types.push_back(CALCY_BOOL);
    if (!validArgs(2, types, args)) {
        cout << "Args invalid to <" << endl;
        return new TypeNothing();
    }
    bool a = dynamic_cast<TypeBool *>(args.at(0))->value;
    bool b = dynamic_cast<TypeBool *>(args.at(1))->value;
    return new TypeBool(a || b);
}


// TODO calcyExp

void initializeBuiltins() {
    functions["+"] = calcyAdd;
    functions["-"] = calcySub;
    functions["*"] = calcyMult;
    functions["/"] = calcyDiv;
    functions["%"] = calcyMod;
    functions["if"] = calcyIf;
    functions[">"] = calcyGt;
    functions[">="] = calcyGte;
    functions["<"] = calcyLt;
    functions["<="] = calcyLte;
    functions["="] = calcyEq;
    functions["and"] = calcyAnd;
    functions["or"] = calcyOr;
    // functions["define"] = calcyDefine;
    variables["x"] = new TypeInt(3);
    // functions["^"] = calcyExp;
}

Type *eval(AST *ast) {
    if (ast->token.get()->type != CALCY_ID) {
        return ast->token.get();
    }

    if (variables.count(ast->token->toString()) > 0)
        return variables.at(ast->token->toString());

    vector<Type *> args;
    for (size_t i = 0; i < ast->children.size(); i++)
        args.emplace_back(eval(ast->children.at(i).get()));
    Type *result = (*functions[ast->token->toString()])(args);
    return result;
}
