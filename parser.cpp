
#include "parser.h"

using namespace std;

vector<string> tokenize(string s) {
    vector<string> tokens;
    if (s.at(0) != '(' || s.at(s.size() - 1) != ')')
        cout << "tokenize received a improperly formatted string" << endl;
    size_t i = 1;
    while (i < s.size() - 1) {
        while (s.at(i) == ' ')
            i++;
        if (s.at(i) == '(') {
            size_t j = i;
            int pcount = 1;
            while (pcount > 0) {
                i++;
                if (s.at(i) == '(')
                    pcount++;
                if (s.at(i) == ')')
                    pcount--;
            }
            i++;
            tokens.push_back(s.substr(j, (i - j)));
        }
        else {
            size_t j = i;
            while (j < s.size() && s.at(j) != ' ' && s.at(j) != '(' && s.at(j) != ')')
                j++;
            tokens.push_back(s.substr(i, (j-i)));
            i = j;
        }
    }
    return tokens;
}


Type *makeType(string s) {
    regex integer("[0-9]+");
    regex real("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?");
    regex boolean("\\#(t|f)");
    regex string("\"[^\"]*\"");
    regex character("\\\\[a-zA-Z0-9]");
    regex identifier("[a-zA-Z\\+-/\\*%@\\$#&!^|><=]+[a-zA-Z0-9\\+-/\\*%@\\$#&!^|<>=]*");
    if (regex_match(s, integer)) {
        cout << "Found integer " << s << endl;
        int x = 0;
        stringstream(s) >> x;
        return new TypeInt(x);
    }
    else if (regex_match(s, real)) {
        cout << "Found double " << s << endl;
        double x = 0;
        stringstream(s) >> x;
        return new TypeDouble(x);
    }
    else if (regex_match(s, boolean)) {
        cout << "Found boolean " << s << endl;
        return new TypeBool(s=="#t"?true:false);
    }
    else if (regex_match(s, string)) {
        cout << "Found string " << s << endl;
        return new TypeString(s);
    }
    else if (regex_match(s, character)) {
        cout << "Found character " << s << endl;
        return new TypeChar(s.at(1));
    }
    else if (regex_match(s, identifier)) {
        cout << "Found identifier " << s << endl;
        return new TypeId(s);
    }
    cout << "I don't know what that is: " << s << endl;
    return NULL;
}

AST *createAst(vector<string> tokens) {
    AST *ast = new AST();
    ast->token = unique_ptr<Type>(makeType(tokens.at(0)));
    size_t i = 1;
    while (i < tokens.size()) {
        if (tokens.at(i).at(0) == '(')
            ast->children.emplace_back(createAst(tokenize(tokens.at(i))));
        else {
            ast->children.emplace_back(new AST(makeType(tokens.at(i))));
        }
        i++;
    }

    return ast;
}

AST* parse(string line) {
    auto tokens = tokenize(line);

    AST *ast = createAst(tokens);

    return ast;
}
