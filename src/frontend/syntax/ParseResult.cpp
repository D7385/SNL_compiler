#include "frontend/syntax/ParseResult.h"

ParseResult::ParseResult() : success(false), tree(nullptr) {
}

ParseResult::~ParseResult() {
    delete tree;
}

bool ParseResult::isSuccess() const {
    return success;
}

void ParseResult::setSuccess(bool success) {
    this->success = success;
}

SyntaxTree* ParseResult::getTree() const {
    return tree;
}

void ParseResult::setTree(SyntaxTree* tree) {
    this->tree = tree;
}

const std::vector<std::string>& ParseResult::getErrors() const {
    return errors;
}

void ParseResult::setErrors(const std::vector<std::string>& errors) {
    this->errors = errors;
}
