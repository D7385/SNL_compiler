#ifndef LEXERRESULT_H
#define LEXERRESULT_H

#include <vector>
#include <string>
#include "frontend/lexer/Token.h"

class LexerResult {
private:
    std::vector<Token> tokenList;
    std::vector<std::string> errors;

public:
    const std::vector<Token>& getTokenList() const;
    void setTokenList(const std::vector<Token>& tokens);

    const std::vector<std::string>& getErrors() const;
    void setErrors(const std::vector<std::string>& errors);
};

#endif // LEXERRESULT_H
