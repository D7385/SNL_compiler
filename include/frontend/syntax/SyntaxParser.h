#ifndef SYNTAXPARSER_H
#define SYNTAXPARSER_H

#include <vector>
#include "../lexer/Token.h"
#include "ParseResult.h"

class SyntaxParser {
public:
    virtual ~SyntaxParser() {}
    virtual ParseResult* parse(const std::vector<Token>& tokens) = 0;
};

#endif // SYNTAXPARSER_H
