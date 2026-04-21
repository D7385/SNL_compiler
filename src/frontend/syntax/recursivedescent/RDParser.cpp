#include "frontend/syntax/recursivedescent/RDParser.h"
#include <iostream>

RDParser::RDParser() : index(0) {
}

RDParser::~RDParser() {
}

ParseResult* RDParser::parse(const std::vector<Token>& tokens) {
    errors.clear();
    ParseResult* result = new ParseResult();
    list = tokens;
    index = 0;

    if (list.empty()) {
        errors.push_back("No token to read.");
        result->setErrors(errors);
        return result;
    }

    TreeNode* root = program();
    result->setTree(new SyntaxTree(root));

    if (getToken() != nullptr) {
        errors.push_back("Source code too long.");
    }

    result->setErrors(errors);
    result->setSuccess(errors.empty());
    return result;
}

TreeNode* RDParser::createNode(const std::string& value) {
    return new TreeNode(value);
}

TreeNode* RDParser::match(TokenType type) {
    Token* token = getToken();
    if (token != nullptr && token->type == type) {
        return createNode(token->value);
    } else {
        std::vector<TokenType> expected = {type};
        error(expected);
        return createNode();
    }
}

Token* RDParser::getToken() {
    if (index < list.size()) {
        return &list[index++];
    }
    return nullptr;
}

Token* RDParser::peekToken() {
    if (index < list.size()) {
        return &list[index];
    }
    return nullptr;
}

void RDParser::error(const std::vector<TokenType>& expected) {
    Token* token = peekToken();
    std::string errorMsg = "Syntax error: expected ";
    for (size_t i = 0; i < expected.size(); i++) {
        if (i > 0) errorMsg += ", ";
        errorMsg += TokenTypeUtil::getStr(expected[i]);
    }
    if (token != nullptr) {
        errorMsg += " but found " + token->value + " at line " + 
                   std::to_string(token->line) + ", column " + std::to_string(token->column);
    } else {
        errorMsg += " but reached end of file";
    }
    errors.push_back(errorMsg);
}

// 语法分析方法实现
TreeNode* RDParser::program() {
    TreeNode* root = createNode("Program");
    std::vector<TreeNode*> children = {
        programHead(),
        declarePart(),
        programBody(),
        match(TokenType::EOF_TOKEN)
    };
    root->setChildren(children);
    return root;
}

TreeNode* RDParser::programHead() {
    TreeNode* pHead = createNode("ProgramHead");
    std::vector<TreeNode*> children = {
        match(TokenType::PROGRAM),
        programName()
    };
    pHead->setChildren(children);
    return pHead;
}

TreeNode* RDParser::declarePart() {
    TreeNode* node = createNode("DeclarePart");
    std::vector<TreeNode*> children = {
        typeDecPart(),
        varDecPart(),
        procDecpart()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::programBody() {
    TreeNode* node = createNode("ProgramBody");
    std::vector<TreeNode*> children = {
        match(TokenType::BEGIN),
        stmList(),
        match(TokenType::END)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::programName() {
    TreeNode* node = createNode("ProgramName");
    std::vector<TreeNode*> children = {
        match(TokenType::ID)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::typeDecPart() {
    TreeNode* node = createNode("TypeDecPart");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::VAR:
            case TokenType::PROCEDURE:
            case TokenType::BEGIN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::TYPE:
                node->setChildren({typeDec()});
                break;
            default:
                error({TokenType::VAR, TokenType::PROCEDURE, TokenType::BEGIN, TokenType::TYPE});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::varDecPart() {
    TreeNode* node = createNode("VarDecPart");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::PROCEDURE:
            case TokenType::BEGIN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::VAR:
                node->setChildren({varDec()});
                break;
            default:
                error({TokenType::PROCEDURE, TokenType::BEGIN, TokenType::VAR});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::varDec() {
    TreeNode* node = createNode("VarDec");
    std::vector<TreeNode*> children = {
        match(TokenType::VAR),
        varDecList()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::varDecList() {
    TreeNode* node = createNode("VarDecList");
    std::vector<TreeNode*> children = {
        typeDef(),
        varIdList(),
        match(TokenType::SEMI),
        varDecMore()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::varIdList() {
    TreeNode* node = createNode("varIdList");
    std::vector<TreeNode*> children = {
        match(TokenType::ID),
        varIdMore()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::varDecMore() {
    TreeNode* node = createNode("varDecMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::PROCEDURE:
            case TokenType::BEGIN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::INTEGER:
            case TokenType::CHAR:
            case TokenType::ARRAY:
            case TokenType::RECORD:
            case TokenType::ID:
                node->setChildren({varDecList()});
                break;
            default:
                error({TokenType::PROCEDURE, TokenType::BEGIN, TokenType::INTEGER, 
                      TokenType::CHAR, TokenType::ARRAY, TokenType::RECORD, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::varIdMore() {
    TreeNode* node = createNode("VarIdMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::SEMI:
                node->setChildren({this->createNode()});
                break;
            case TokenType::COMMA:
                node->setChildren({match(TokenType::COMMA), varIdList()});
                break;
            default:
                error({TokenType::SEMI, TokenType::COMMA});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::procDecpart() {
    TreeNode* node = createNode("ProcDecpart");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::BEGIN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::PROCEDURE:
                node->setChildren({procDec()});
                break;
            default:
                error({TokenType::BEGIN, TokenType::PROCEDURE});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::typeDec() {
    TreeNode* node = createNode("TypeDec");
    std::vector<TreeNode*> children = {
        match(TokenType::TYPE),
        typeDecList()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::typeDecList() {
    TreeNode* node = createNode("TypeDecList");
    std::vector<TreeNode*> children = {
        typeId(),
        match(TokenType::EQ),
        typeDef(),
        match(TokenType::SEMI),
        typeDecMore()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::typeId() {
    TreeNode* node = createNode("TypeID");
    std::vector<TreeNode*> children = {
        match(TokenType::ID)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::typeDef() {
    TreeNode* node = createNode("TypeDef");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::INTEGER:
            case TokenType::CHAR:
                node->setChildren({baseType()});
                break;
            case TokenType::ARRAY:
            case TokenType::RECORD:
                node->setChildren({structureType()});
                break;
            case TokenType::ID:
                node->setChildren({match(TokenType::ID)});
                break;
            default:
                error({TokenType::INTEGER, TokenType::CHAR, TokenType::ARRAY, 
                      TokenType::RECORD, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::typeDecMore() {
    TreeNode* node = createNode("typeDefMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::VAR:
            case TokenType::PROCEDURE:
            case TokenType::BEGIN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::ID:
                node->setChildren({typeDecList()});
                break;
            default:
                error({TokenType::VAR, TokenType::PROCEDURE, TokenType::BEGIN, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::procDec() {
    TreeNode* node = createNode("ProcDec");
    std::vector<TreeNode*> children = {
        match(TokenType::PROCEDURE),
        procName(),
        match(TokenType::LPAREN),
        paramList(),
        match(TokenType::RPAREN),
        match(TokenType::SEMI),
        procDecPart(),
        procBody(),
        procDecpart()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::procDecPart() {
    TreeNode* node = createNode("ProcDecPart");
    node->setChildren({declarePart()});
    return node;
}

TreeNode* RDParser::baseType() {
    TreeNode* node = createNode("BaseType");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::INTEGER:
                node->setChildren({match(TokenType::INTEGER)});
                break;
            case TokenType::CHAR:
                node->setChildren({match(TokenType::CHAR)});
                break;
            default:
                error({TokenType::INTEGER, TokenType::CHAR});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::structureType() {
    TreeNode* node = createNode("StructureType");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::ARRAY:
                node->setChildren({arrayType()});
                break;
            case TokenType::RECORD:
                node->setChildren({recType()});
                break;
            default:
                error({TokenType::ARRAY, TokenType::RECORD});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::procName() {
    TreeNode* node = createNode("ProcName");
    node->setChildren({match(TokenType::ID)});
    return node;
}

TreeNode* RDParser::paramList() {
    TreeNode* node = createNode("ParamList");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::RPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::INTEGER:
            case TokenType::CHAR:
            case TokenType::ARRAY:
            case TokenType::RECORD:
            case TokenType::ID:
            case TokenType::VAR:
                node->setChildren({paramDecList()});
                break;
            default:
                error({TokenType::RPAREN, TokenType::INTEGER, TokenType::CHAR, 
                      TokenType::ARRAY, TokenType::RECORD, TokenType::ID, TokenType::VAR});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::procBody() {
    TreeNode* node = createNode("ProcBody");
    node->setChildren({programBody()});
    return node;
}

TreeNode* RDParser::arrayType() {
    TreeNode* node = createNode("ArrayType");
    std::vector<TreeNode*> children = {
        match(TokenType::ARRAY),
        match(TokenType::LMIDPAREN),
        low(),
        match(TokenType::UNDERRANGE),
        top(),
        match(TokenType::RMIDPAREN),
        match(TokenType::OF),
        baseType()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::recType() {
    TreeNode* node = createNode("RecType");
    std::vector<TreeNode*> children = {
        match(TokenType::RECORD),
        filedDecList(),
        match(TokenType::END)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::paramDecList() {
    TreeNode* node = createNode("ParamDecList");
    std::vector<TreeNode*> children = {
        param(),
        paramMore()
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::low() {
    TreeNode* node = createNode("Low");
    node->setChildren({match(TokenType::INTC)});
    return node;
}

TreeNode* RDParser::top() {
    TreeNode* node = createNode("Top");
    node->setChildren({match(TokenType::INTC)});
    return node;
}

TreeNode* RDParser::filedDecList() {
    TreeNode* node = createNode("FiledDecList");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::INTEGER:
            case TokenType::CHAR:
                node->setChildren({baseType(), idList(), match(TokenType::SEMI), filedDecMore()});
                break;
            case TokenType::ARRAY:
                node->setChildren({arrayType(), idList(), match(TokenType::SEMI), filedDecMore()});
                break;
            default:
                error({TokenType::INTEGER, TokenType::CHAR, TokenType::ARRAY});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::param() {
    TreeNode* node = createNode("Param");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::INTEGER:
            case TokenType::CHAR:
            case TokenType::ARRAY:
            case TokenType::RECORD:
            case TokenType::ID:
                node->setChildren({typeDef(), formList()});
                break;
            case TokenType::VAR:
                node->setChildren({match(TokenType::VAR), typeDef(), formList()});
                break;
            default:
                error({TokenType::INTEGER, TokenType::CHAR, TokenType::ARRAY, 
                      TokenType::RECORD, TokenType::ID, TokenType::VAR});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::paramMore() {
    TreeNode* node = createNode("ParamMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::RPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::SEMI:
                node->setChildren({match(TokenType::SEMI), paramDecList()});
                break;
            default:
                error({TokenType::RPAREN, TokenType::SEMI});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::idList() {
    TreeNode* node = createNode("IdList");
    node->setChildren({match(TokenType::ID), idMore()});
    return node;
}

TreeNode* RDParser::filedDecMore() {
    TreeNode* node = createNode("FiledDecMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::END:
                node->setChildren({this->createNode()});
                break;
            case TokenType::INTEGER:
            case TokenType::CHAR:
            case TokenType::ARRAY:
                node->setChildren({filedDecList()});
                break;
            default:
                error({TokenType::END, TokenType::INTEGER, TokenType::CHAR, TokenType::ARRAY});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::formList() {
    TreeNode* node = createNode("FormList");
    node->setChildren({match(TokenType::ID), fidMore()});
    return node;
}

TreeNode* RDParser::idMore() {
    TreeNode* node = createNode("IdMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::SEMI:
                node->setChildren({this->createNode()});
                break;
            case TokenType::COMMA:
                node->setChildren({match(TokenType::COMMA), idList()});
                break;
            default:
                error({TokenType::SEMI, TokenType::COMMA});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::fidMore() {
    TreeNode* node = createNode("FidMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::SEMI:
            case TokenType::RPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::COMMA:
                node->setChildren({match(TokenType::COMMA), formList()});
                break;
            default:
                error({TokenType::SEMI, TokenType::RPAREN, TokenType::COMMA});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::stmList() {
    TreeNode* node = createNode("StmList");
    node->setChildren({stm(), stmMore()});
    return node;
}

TreeNode* RDParser::stm() {
    TreeNode* node = createNode("Stm");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::IF:
                node->setChildren({conditionalStm()});
                break;
            case TokenType::WHILE:
                node->setChildren({loopStm()});
                break;
            case TokenType::READ:
                node->setChildren({inputStm()});
                break;
            case TokenType::WRITE:
                node->setChildren({outputStm()});
                break;
            case TokenType::RETURN:
                node->setChildren({returnStm()});
                break;
            case TokenType::ID:
                node->setChildren({match(TokenType::ID), assCall()});
                break;
            default:
                error({TokenType::IF, TokenType::WHILE, TokenType::READ, 
                      TokenType::WRITE, TokenType::RETURN, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::stmMore() {
    TreeNode* node = createNode("StmMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::ELSE:
            case TokenType::FI:
            case TokenType::END:
            case TokenType::ENDWH:
                node->setChildren({this->createNode()});
                break;
            case TokenType::SEMI:
                node->setChildren({match(TokenType::SEMI), stmList()});
                break;
            default:
                error({TokenType::ELSE, TokenType::FI, TokenType::END, 
                      TokenType::ENDWH, TokenType::SEMI});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::conditionalStm() {
    TreeNode* node = createNode("ConditionalStm");
    std::vector<TreeNode*> children = {
        match(TokenType::IF),
        relExp(),
        match(TokenType::THEN),
        stmList(),
        match(TokenType::ELSE),
        stmList(),
        match(TokenType::FI)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::loopStm() {
    TreeNode* node = createNode("LoopStm");
    std::vector<TreeNode*> children = {
        match(TokenType::WHILE),
        relExp(),
        match(TokenType::DO),
        stmList(),
        match(TokenType::ENDWH)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::inputStm() {
    TreeNode* node = createNode("InputStm");
    std::vector<TreeNode*> children = {
        match(TokenType::READ),
        match(TokenType::LPAREN),
        invar(),
        match(TokenType::RPAREN)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::outputStm() {
    TreeNode* node = createNode("OutputStm");
    std::vector<TreeNode*> children = {
        match(TokenType::WRITE),
        match(TokenType::LPAREN),
        exp(),
        match(TokenType::RPAREN)
    };
    node->setChildren(children);
    return node;
}

TreeNode* RDParser::returnStm() {
    TreeNode* node = createNode("ReturnStm");
    node->setChildren({match(TokenType::RETURN)});
    return node;
}

TreeNode* RDParser::assCall() {
    TreeNode* node = createNode("AssCall");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::ASSIGN:
            case TokenType::LMIDPAREN:
            case TokenType::DOT:
                node->setChildren({assignmentRest()});
                break;
            case TokenType::LPAREN:
                node->setChildren({callStmRest()});
                break;
            default:
                error({TokenType::ASSIGN, TokenType::LPAREN});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::relExp() {
    TreeNode* node = createNode("RelExp");
    node->setChildren({exp(), otherRelE()});
    return node;
}

TreeNode* RDParser::invar() {
    TreeNode* node = createNode("Invar");
    node->setChildren({match(TokenType::ID)});
    return node;
}

TreeNode* RDParser::exp() {
    TreeNode* node = createNode("Exp");
    node->setChildren({term(), otherTerm()});
    return node;
}

TreeNode* RDParser::assignmentRest() {
    TreeNode* node = createNode("AssignmentRest");
    node->setChildren({variMore(), match(TokenType::ASSIGN), exp()});
    return node;
}

TreeNode* RDParser::callStmRest() {
    TreeNode* node = createNode("CallStmRest");
    node->setChildren({match(TokenType::LPAREN), actParamList(), match(TokenType::RPAREN)});
    return node;
}

TreeNode* RDParser::otherRelE() {
    TreeNode* node = createNode("OtherRelE");
    node->setChildren({cmpOp(), exp()});
    return node;
}

TreeNode* RDParser::term() {
    TreeNode* node = createNode("Term");
    node->setChildren({factor(), otherFactor()});
    return node;
}

TreeNode* RDParser::otherTerm() {
    TreeNode* node = createNode("OtherTerm");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::LT:
            case TokenType::EQ:
            case TokenType::RMIDPAREN:
            case TokenType::THEN:
            case TokenType::ELSE:
            case TokenType::FI:
            case TokenType::DO:
            case TokenType::ENDWH:
            case TokenType::RPAREN:
            case TokenType::END:
            case TokenType::SEMI:
            case TokenType::COMMA:
                node->setChildren({this->createNode()});
                break;
            case TokenType::PLUS:
            case TokenType::MINUS:
                node->setChildren({addOp(), exp()});
                break;
            default:
                error({TokenType::LT, TokenType::EQ, TokenType::RMIDPAREN, 
                      TokenType::THEN, TokenType::ELSE, TokenType::FI, 
                      TokenType::DO, TokenType::ENDWH, TokenType::RPAREN, 
                      TokenType::END, TokenType::SEMI, TokenType::COMMA, 
                      TokenType::PLUS, TokenType::MINUS});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::variMore() {
    TreeNode* node = createNode("VariMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::ASSIGN:
            case TokenType::TIMES:
            case TokenType::OVER:
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::LT:
            case TokenType::EQ:
            case TokenType::THEN:
            case TokenType::ELSE:
            case TokenType::FI:
            case TokenType::DO:
            case TokenType::ENDWH:
            case TokenType::RPAREN:
            case TokenType::END:
            case TokenType::SEMI:
            case TokenType::COMMA:
            case TokenType::RMIDPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::LMIDPAREN:
                node->setChildren({match(TokenType::LMIDPAREN), exp(), match(TokenType::RMIDPAREN)});
                break;
            case TokenType::DOT:
                node->setChildren({match(TokenType::DOT), filedVar()});
                break;
            default:
                error({TokenType::ASSIGN, TokenType::TIMES, TokenType::OVER, 
                      TokenType::PLUS, TokenType::MINUS, TokenType::LT, TokenType::EQ, 
                      TokenType::THEN, TokenType::ELSE, TokenType::FI, TokenType::DO, 
                      TokenType::ENDWH, TokenType::RPAREN, TokenType::END, 
                      TokenType::SEMI, TokenType::COMMA, TokenType::RMIDPAREN, 
                      TokenType::DOT});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::actParamList() {
    TreeNode* node = createNode("ActParamList");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::RPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::LPAREN:
            case TokenType::INTC:
            case TokenType::ID:
                node->setChildren({exp(), actParamMore()});
                break;
            default:
                error({TokenType::RPAREN, TokenType::LPAREN, TokenType::INTC, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::cmpOp() {
    TreeNode* node = createNode("CmpOp");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::LT:
                node->setChildren({match(TokenType::LT)});
                break;
            case TokenType::EQ:
                node->setChildren({match(TokenType::EQ)});
                break;
            default:
                error({TokenType::LT, TokenType::EQ});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::factor() {
    TreeNode* node = createNode("Factor");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::LPAREN:
                node->setChildren({match(TokenType::LPAREN), exp(), match(TokenType::RPAREN)});
                break;
            case TokenType::INTC:
                node->setChildren({match(TokenType::INTC)});
                break;
            case TokenType::ID:
                node->setChildren({variable()});
                break;
            default:
                error({TokenType::LPAREN, TokenType::INTC, TokenType::ID});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::otherFactor() {
    TreeNode* node = createNode("OtherFactor");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::LT:
            case TokenType::EQ:
            case TokenType::RMIDPAREN:
            case TokenType::THEN:
            case TokenType::ELSE:
            case TokenType::FI:
            case TokenType::DO:
            case TokenType::ENDWH:
            case TokenType::RPAREN:
            case TokenType::END:
            case TokenType::SEMI:
            case TokenType::COMMA:
                node->setChildren({this->createNode()});
                break;
            case TokenType::TIMES:
            case TokenType::OVER:
                node->setChildren({multiOp(), term()});
                break;
            default:
                error({TokenType::PLUS, TokenType::MINUS, TokenType::LT, TokenType::EQ, 
                      TokenType::RMIDPAREN, TokenType::THEN, TokenType::ELSE, TokenType::FI, 
                      TokenType::DO, TokenType::ENDWH, TokenType::RPAREN, TokenType::END, 
                      TokenType::SEMI, TokenType::COMMA, TokenType::TIMES, TokenType::OVER});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::addOp() {
    TreeNode* node = createNode("AddOp");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::PLUS:
                node->setChildren({match(TokenType::PLUS)});
                break;
            case TokenType::MINUS:
                node->setChildren({match(TokenType::MINUS)});
                break;
            default:
                error({TokenType::PLUS, TokenType::MINUS});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::filedVar() {
    TreeNode* node = createNode("FiledVar");
    node->setChildren({match(TokenType::ID), filedVarMore()});
    return node;
}

TreeNode* RDParser::actParamMore() {
    TreeNode* node = createNode("ActParamMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::RPAREN:
                node->setChildren({this->createNode()});
                break;
            case TokenType::COMMA:
                node->setChildren({match(TokenType::COMMA), actParamList()});
                break;
            default:
                error({TokenType::RPAREN, TokenType::COMMA});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::variable() {
    TreeNode* node = createNode("Variable");
    node->setChildren({match(TokenType::ID), variMore()});
    return node;
}

TreeNode* RDParser::multiOp() {
    TreeNode* node = createNode("MultiOp");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::TIMES:
                node->setChildren({match(TokenType::TIMES)});
                break;
            case TokenType::OVER:
                node->setChildren({match(TokenType::OVER)});
                break;
            default:
                error({TokenType::TIMES, TokenType::OVER});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}

TreeNode* RDParser::filedVarMore() {
    TreeNode* node = createNode("FiledVarMore");
    Token* token = peekToken();
    if (token != nullptr) {
        switch (token->type) {
            case TokenType::ASSIGN:
            case TokenType::TIMES:
            case TokenType::OVER:
            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::LT:
            case TokenType::EQ:
            case TokenType::THEN:
            case TokenType::ELSE:
            case TokenType::FI:
            case TokenType::DO:
            case TokenType::ENDWH:
            case TokenType::RPAREN:
            case TokenType::END:
            case TokenType::SEMI:
            case TokenType::COMMA:
                node->setChildren({this->createNode()});
                break;
            case TokenType::LMIDPAREN:
                node->setChildren({match(TokenType::LMIDPAREN), exp(), match(TokenType::RMIDPAREN)});
                break;
            default:
                error({TokenType::ASSIGN, TokenType::TIMES, TokenType::OVER, 
                      TokenType::PLUS, TokenType::MINUS, TokenType::LT, TokenType::EQ, 
                      TokenType::THEN, TokenType::ELSE, TokenType::FI, TokenType::DO, 
                      TokenType::ENDWH, TokenType::RPAREN, TokenType::END, 
                      TokenType::SEMI, TokenType::COMMA, TokenType::LMIDPAREN});
                node->setChildren({this->createNode()});
        }
    }
    return node;
}
