#include "frontend/lexer/Token.h"

Token::Token() : line(0), column(0), type(TokenType::EMPTY), value("") {
}

Token::Token(const std::string& value) : line(0), column(0), type(TokenType::EMPTY), value(value) {
}

Token::Token(TokenType type) : line(0), column(0), type(type), value(TokenTypeUtil::getStr(type)) {
}

Token::Token(int line, int column, TokenType type, const std::string& value) 
    : line(line), column(column), type(type), value(value) {
}

void Token::checkKeyWords() {
    if (type == TokenType::ID) {
        if (value == TokenTypeUtil::getStr(TokenType::PROGRAM)) {
            type = TokenType::PROGRAM;
        } else if (value == TokenTypeUtil::getStr(TokenType::PROCEDURE)) {
            type = TokenType::PROCEDURE;
        } else if (value == TokenTypeUtil::getStr(TokenType::TYPE)) {
            type = TokenType::TYPE;
        } else if (value == TokenTypeUtil::getStr(TokenType::VAR)) {
            type = TokenType::VAR;
        } else if (value == TokenTypeUtil::getStr(TokenType::IF)) {
            type = TokenType::IF;
        } else if (value == TokenTypeUtil::getStr(TokenType::THEN)) {
            type = TokenType::THEN;
        } else if (value == TokenTypeUtil::getStr(TokenType::ELSE)) {
            type = TokenType::ELSE;
        } else if (value == TokenTypeUtil::getStr(TokenType::FI)) {
            type = TokenType::FI;
        } else if (value == TokenTypeUtil::getStr(TokenType::WHILE)) {
            type = TokenType::WHILE;
        } else if (value == TokenTypeUtil::getStr(TokenType::DO)) {
            type = TokenType::DO;
        } else if (value == TokenTypeUtil::getStr(TokenType::ENDWH)) {
            type = TokenType::ENDWH;
        } else if (value == TokenTypeUtil::getStr(TokenType::BEGIN)) {
            type = TokenType::BEGIN;
        } else if (value == TokenTypeUtil::getStr(TokenType::END)) {
            type = TokenType::END;
        } else if (value == TokenTypeUtil::getStr(TokenType::READ)) {
            type = TokenType::READ;
        } else if (value == TokenTypeUtil::getStr(TokenType::WRITE)) {
            type = TokenType::WRITE;
        } else if (value == TokenTypeUtil::getStr(TokenType::ARRAY)) {
            type = TokenType::ARRAY;
        } else if (value == TokenTypeUtil::getStr(TokenType::OF)) {
            type = TokenType::OF;
        } else if (value == TokenTypeUtil::getStr(TokenType::RECORD)) {
            type = TokenType::RECORD;
        } else if (value == TokenTypeUtil::getStr(TokenType::RETURN)) {
            type = TokenType::RETURN;
        } else if (value == TokenTypeUtil::getStr(TokenType::INTEGER)) {
            type = TokenType::INTEGER;
        } else if (value == TokenTypeUtil::getStr(TokenType::CHAR)) {
            type = TokenType::CHAR;
        }
    }
}

std::string Token::toString() const {
    return "`" + value + "|" + TokenTypeUtil::getStr(type) + "|" + 
           std::to_string(line) + ":" + std::to_string(column) + "`";
}
