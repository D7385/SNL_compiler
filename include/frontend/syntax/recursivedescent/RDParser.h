#ifndef RDPARSER_H
#define RDPARSER_H

#include <vector>
#include "../SyntaxParser.h"

class RDParser : public SyntaxParser {
private:
    std::vector<Token> list;
    std::vector<std::string> errors;
    size_t index;

public:
    RDParser();
    ~RDParser() override;

    ParseResult* parse(const std::vector<Token>& tokens) override;

private:
    // 语法分析方法
    TreeNode* program();
    TreeNode* programHead();
    TreeNode* declarePart();
    TreeNode* programBody();
    TreeNode* programName();
    TreeNode* typeDecPart();
    TreeNode* varDecPart();
    TreeNode* varDec();
    TreeNode* varDecList();
    TreeNode* varIdList();
    TreeNode* varDecMore();
    TreeNode* varIdMore();
    TreeNode* procDecpart();
    TreeNode* typeDec();
    TreeNode* typeDecList();
    TreeNode* typeId();
    TreeNode* typeDef();
    TreeNode* typeDecMore();
    TreeNode* procDec();
    TreeNode* procDecPart();
    TreeNode* baseType();
    TreeNode* structureType();
    TreeNode* procName();
    TreeNode* paramList();
    TreeNode* procBody();
    TreeNode* arrayType();
    TreeNode* recType();
    TreeNode* paramDecList();
    TreeNode* low();
    TreeNode* top();
    TreeNode* filedDecList();
    TreeNode* param();
    TreeNode* paramMore();
    TreeNode* idList();
    TreeNode* filedDecMore();
    TreeNode* formList();
    TreeNode* idMore();
    TreeNode* fidMore();
    TreeNode* stmList();
    TreeNode* stm();
    TreeNode* stmMore();
    TreeNode* conditionalStm();
    TreeNode* loopStm();
    TreeNode* inputStm();
    TreeNode* outputStm();
    TreeNode* returnStm();
    TreeNode* assCall();
    TreeNode* relExp();
    TreeNode* invar();
    TreeNode* exp();
    TreeNode* assignmentRest();
    TreeNode* callStmRest();
    TreeNode* otherRelE();
    TreeNode* term();
    TreeNode* otherTerm();
    TreeNode* variMore();
    TreeNode* actParamList();
    TreeNode* cmpOp();
    TreeNode* factor();
    TreeNode* otherFactor();
    TreeNode* addOp();
    TreeNode* filedVar();
    TreeNode* actParamMore();
    TreeNode* variable();
    TreeNode* multiOp();
    TreeNode* filedVarMore();

    // 辅助方法
    TreeNode* createNode(const std::string& value = "");
    TreeNode* match(TokenType type);
    Token* getToken();
    Token* peekToken();
    void error(const std::vector<TokenType>& expected);
};

#endif // RDPARSER_H
