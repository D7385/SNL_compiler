#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "frontend/lexer/TokenType.h"

class Token {
public:
    int line;       // 行
    int column;     // 列
    TokenType type; // 类型
    std::string value;   // 含义

    Token();
    Token(const std::string& value);
    Token(TokenType type);
    Token(int line, int column, TokenType type, const std::string& value);

    /**
     * 如果是保留字则将类型改为相应的类型
     */
    void checkKeyWords();

    std::string toString() const;
};

#endif // TOKEN_H
