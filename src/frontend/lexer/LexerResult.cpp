#include "frontend/lexer/LexerResult.h"

const std::vector<Token>& LexerResult::getTokenList() const {
    return tokenList;
}

void LexerResult::setTokenList(const std::vector<Token>& tokens) {
    tokenList = tokens;
}

const std::vector<std::string>& LexerResult::getErrors() const {
    return errors;
}

void LexerResult::setErrors(const std::vector<std::string>& errors) {
    this->errors = errors;
}
