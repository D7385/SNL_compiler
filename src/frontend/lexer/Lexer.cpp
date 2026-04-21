#include "frontend/lexer/Lexer.h"
#include <iostream>

LexerResult Lexer::getResult(std::istream& reader) {
    errors.clear();
    LexerResult result;
    std::vector<Token> list;
    this->reader = &reader;
    getMeFirst = -1;
    line = 1;
    column = 0;

    Token* token = getToken();
    while (token != nullptr) {
        list.push_back(*token);
        delete token;
        token = getToken();
    }

    result.setTokenList(list);
    result.setErrors(errors);
    return result;
}

Token* Lexer::getToken() {
    State state = State::Normal;
    std::string sb;
    int ch = getChar();

    while (ch != -1) {
        sb += static_cast<char>(ch);

        switch (state) {
            case State::Normal:
                if (isAlpha(ch)) {
                    state = State::InId;
                } else if (isDigit(ch)) {
                    state = State::InNum;
                } else if (isBlank(ch)) {
                    sb.pop_back();
                    state = State::Normal;
                } else if (ch == '+') {
                    return new Token(line, column, TokenType::PLUS, sb);
                } else if (ch == '-') {
                    return new Token(line, column, TokenType::MINUS, sb);
                } else if (ch == '*') {
                    return new Token(line, column, TokenType::TIMES, sb);
                } else if (ch == '/') {
                    return new Token(line, column, TokenType::OVER, sb);
                } else if (ch == '(') {
                    return new Token(line, column, TokenType::LPAREN, sb);
                } else if (ch == ')') {
                    return new Token(line, column, TokenType::RPAREN, sb);
                } else if (ch == '[') {
                    return new Token(line, column, TokenType::LMIDPAREN, sb);
                } else if (ch == ']') {
                    return new Token(line, column, TokenType::RMIDPAREN, sb);
                } else if (ch == ';') {
                    return new Token(line, column, TokenType::SEMI, sb);
                } else if (ch == ',') {
                    return new Token(line, column, TokenType::COMMA, sb);
                } else if (ch == '=') {
                    return new Token(line, column, TokenType::EQ, sb);
                } else if (ch == '<') {
                    return new Token(line, column, TokenType::LT, sb);
                } else if (ch == ':') {
                    state = State::InAssign;
                } else if (ch == '{') {
                    sb.pop_back();
                    state = State::InComment;
                } else if (ch == '.') {
                    state = State::InDot;
                } else if (ch == '\'') {
                    sb.pop_back();
                    state = State::InChar;
                } else {
                    state = State::Error;
                }
                break;

            case State::InId:
                if (isAlpha(ch) || isDigit(ch)) {
                    state = State::InId;
                } else {
                    unGetChar(static_cast<int>(sb.back()));
                    sb.pop_back();
                    Token* token = new Token(line, column, TokenType::ID, sb);
                    token->checkKeyWords();
                    return token;
                }
                break;

            case State::InNum:
                if (!isDigit(ch)) {
                    unGetChar(static_cast<int>(sb.back()));
                    sb.pop_back();
                    return new Token(line, column, TokenType::INTC, sb);
                }
                break;

            case State::InAssign:
                if (ch == '=') {
                    return new Token(line, column, TokenType::ASSIGN, sb);
                } else {
                    state = State::Error;
                }
                break;

            case State::InComment:
                sb.pop_back();
                while (ch != -1 && ch != '}') {
                    ch = getChar();
                }
                state = State::Normal;
                if (ch != '}') {
                    state = State::Error;
                } else {
                    // 跳过 '}' 字符
                    ch = getChar();
                }
                break;

            case State::InDot:
                if (isAlpha(ch)) {
                    unGetChar(ch);
                    sb.pop_back();
                    return new Token(line, column, TokenType::DOT, sb);
                }
                if (ch == '.') {
                    state = State::InRange;
                    break;
                }
                while (isBlank(ch)) {
                    ch = getChar();
                }
                if (ch == -1) {
                    return new Token(line, column, TokenType::EOF_TOKEN, ".");
                }
                unGetChar(ch);
                state = State::Error;
                break;

            case State::InRange:
                if (isDigit(ch)) {
                    unGetChar(ch);
                    sb.pop_back();
                    return new Token(line, column, TokenType::UNDERRANGE, sb);
                }
                state = State::Error;
                break;

            case State::InChar:
                if (isAlpha(ch) || isDigit(ch)) {
                    ch = getChar();
                    if (ch == '\'') {
                        return new Token(line, column, TokenType::CHARACTER, sb);
                    }
                }
                state = State::Error;
                break;

            case State::Error:
                errors.push_back("[Error] Unrecognized token. near " + 
                               std::to_string(line) + ":" + std::to_string(column));
                return new Token();

            default:
                state = State::Error;
        }

        ch = getChar();
    }

    if (state == State::InDot) {
        return new Token(line, column, TokenType::EOF_TOKEN, ".");
    }

    if (state != State::Normal) {
        errors.push_back("[错误]在 " + std::to_string(line) + "行 " + 
                       std::to_string(column) + "列");
    }

    return nullptr;
}

int Lexer::getChar() {
    int ch;
    if (getMeFirst != -1) {
        ch = getMeFirst;
        getMeFirst = -1;
    } else {
        ch = reader->get();
        if (ch == EOF) {
            ch = -1;
        }
    }

    if (ch == '\n') {
        column = 0;
        line++;
    } else if (ch != -1) {
        column++;
    }

    if (ch == '\r') {
        column--;
    }

    return ch;
}

void Lexer::unGetChar(int ch) {
    getMeFirst = ch;
    column--;
}

bool Lexer::isAlpha(int ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool Lexer::isDigit(int ch) {
    return (ch >= '0' && ch <= '9');
}

bool Lexer::isBlank(int ch) {
    return (static_cast<char>(ch) == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

std::string Lexer::showChar(int ch) {
    if (ch == '\n') {
        return "\\n";
    } else if (ch == '\r') {
        return "\\r";
    } else {
        return std::string(1, static_cast<char>(ch));
    }
}
