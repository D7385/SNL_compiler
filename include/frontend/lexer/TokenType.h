#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <string>
enum class TokenType {
    EOF_TOKEN,
    ERROR,
    EMPTY,

    /*保留字*/
    PROGRAM,
    PROCEDURE,
    TYPE,
    VAR,
    IF,
    THEN,
    ELSE,
    FI,
    WHILE,
    DO,
    ENDWH,
    BEGIN,
    END,
    READ,
    WRITE,
    ARRAY,
    OF,
    RECORD,
    RETURN,
    /*类型*/
    INTEGER,
    CHAR,

    ID,
    INTC,
    CHARACTER,

    /*特殊符号*/
    ASSIGN,
    EQ,
    LT,
    PLUS,
    MINUS,
    TIMES,
    OVER,
    LPAREN,
    RPAREN,
    LMIDPAREN,
    RMIDPAREN,
    UNDERRANGE,
    SEMI,
    COMMA,
    DOT
};

class TokenTypeUtil {
public:
    static const std::string& getStr(TokenType type) {
        static const std::string strings[] = {
            ".",           // EOF_TOKEN
            "error",       // ERROR
            "",            // EMPTY
            "program",     // PROGRAM
            "procedure",   // PROCEDURE
            "type",        // TYPE
            "var",         // VAR
            "if",          // IF
            "then",        // THEN
            "else",        // ELSE
            "fi",          // FI
            "while",       // WHILE
            "do",          // DO
            "endwh",       // ENDWH
            "begin",       // BEGIN
            "end",         // END
            "read",        // READ
            "write",       // WRITE
            "array",       // ARRAY
            "of",          // OF
            "record",      // RECORD
            "return",      // RETURN
            "integer",     // INTEGER
            "char",        // CHAR
            "id",          // ID
            "intc",        // INTC
            "character",   // CHARACTER
            ":=",          // ASSIGN
            "=",           // EQ
            "<",           // LT
            "+",           // PLUS
            "-",           // MINUS
            "*",           // TIMES
            "/",           // OVER
            "(",           // LPAREN
            ")",           // RPAREN
            "[",           // LMIDPAREN
            "]",           // RMIDPAREN
            "..",          // UNDERRANGE
            ";",           // SEMI
            ",",           // COMMA
            "."            // DOT
        };
        return strings[static_cast<int>(type)];
    }
};

#endif // TOKENTYPE_H

