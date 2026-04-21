#ifndef PARSERESULT_H
#define PARSERESULT_H

#include <vector>
#include <string>
#include "SyntaxTree.h"

class ParseResult {
private:
    bool success;
    SyntaxTree* tree;
    std::vector<std::string> errors;

public:
    ParseResult();
    ~ParseResult();

    bool isSuccess() const;
    void setSuccess(bool success);

    SyntaxTree* getTree() const;
    void setTree(SyntaxTree* tree);

    const std::vector<std::string>& getErrors() const;
    void setErrors(const std::vector<std::string>& errors);
};

#endif // PARSERESULT_H
