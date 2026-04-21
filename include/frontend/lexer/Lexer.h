#ifndef LEXER_H
#define LEXER_H

#include <istream>
#include <vector>
#include <string>
#include "frontend/lexer/Token.h"
#include "frontend/lexer/LexerResult.h"

class Lexer {
enum class State {
    Normal,
    InId,
    InNum,
    InComment,
    InChar,
    Error,
    InAssign,
    InRange,
    InDot
};

private:
    int getMeFirst = -1;
    int line = 1;
    int column = 0;
    std::vector<std::string> errors;
    std::istream* reader;

public:
    LexerResult getResult(std::istream& reader);

private:
    Token* getToken();
    int getChar();
    void unGetChar(int ch);
    bool isAlpha(int ch);
    bool isDigit(int ch);
    bool isBlank(int ch);
    std::string showChar(int ch);
};

#endif // LEXER_H
