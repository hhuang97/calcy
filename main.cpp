#include <iostream>
#include "type.h"
#include "ast.h"
#include "parser.h"
#include "eval.h"

using namespace std;

int main() {
    initializeBuiltins();
    while (true) {
        string line;
        getline(cin, line);
        unique_ptr<AST> ast(parse(line));
        Type *res = eval(ast.get());
        cout << res->toString() << endl;
    }

    return 0;
}
