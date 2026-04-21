#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

#include "frontend/lexer/Lexer.h"
#include "frontend/syntax/recursivedescent/RDParser.h"

void help() {
    std::cout << "Usage: SNLc [-r] [-l] [-h] [-v] <SourceFile>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -r, --RD     (default) use recursive descent to parse." << std::endl;
    std::cout << "  -l, --LL1    use LL1 to parse." << std::endl;
    std::cout << "  -h, --help   show this help text and exit." << std::endl;
    std::cout << "  -v, --version show version text and exit." << std::endl;
    std::cout << "  -e, --encoding specify default encoding to open file." << std::endl;
}

void version() {
    std::cout << "SNLc   : SNL(Small Nested Language) Compiler." << std::endl;
    std::cout << "Version: 1.0." << std::endl;
    std::cout << "Author : Youth．霖" << std::endl;
    std::cout << "Contact: http://youthlin.com/" << std::endl;
}

int main(int argc, char* argv[]) {
    bool useLL1 = false;
    std::string encoding = "UTF-8";
    std::string sourceFile;

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            version();
            return 0;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--LL1") == 0) {
            useLL1 = true;
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encoding") == 0) {
            if (i + 1 < argc) {
                encoding = argv[++i];
            } else {
                std::cerr << "Error: encoding argument missing." << std::endl;
                help();
                return 1;
            }
        } else {
            sourceFile = argv[i];
        }
    }

    if (sourceFile.empty()) {
        std::cerr << "Error: source file not specified." << std::endl;
        help();
        return 1;
    }

    // 打开源文件
    std::ifstream inFile(sourceFile);
    if (!inFile) {
        std::cerr << "File Not Found.找不到源文件。" << std::endl;
        help();
        return 1;
    }

    // 词法分析
    std::cout << "开始词法分析..." << std::endl;
    Lexer lexer;
    LexerResult lexerResult = lexer.getResult(inFile);
    inFile.close();

    const std::vector<std::string>& lexerErrors = lexerResult.getErrors();
    if (!lexerErrors.empty()) {
        std::cerr << "词法分析错误:" << std::endl;
        for (const std::string& error : lexerErrors) {
            std::cerr << error << std::endl;
        }
        return 1;
    }

    // 输出词法分析结果
    std::string tokenListFile = sourceFile + ".token.list.txt";
    std::cout << "生成词法分析结果到: " << tokenListFile << std::endl;
    std::ofstream tokenOut(tokenListFile);
    const std::vector<Token>& tokens = lexerResult.getTokenList();
    std::cout << "词法分析结果包含 " << tokens.size() << " 个token" << std::endl;
    for (const Token& token : tokens) {
        tokenOut << token.toString() << std::endl;
    }
    tokenOut.close();
    std::cout << "词法分析结果生成完成" << std::endl;

    // 语法分析
    SyntaxParser* parser;
    std::string parserName;
    if (useLL1) {
        // 暂时只实现了递归下降分析器
        std::cerr << "LL1 parser not implemented yet. Using recursive descent parser." << std::endl;
        parser = new RDParser();
        parserName = "Recursive Descent";
    } else {
        parser = new RDParser();
        parserName = "Recursive Descent";
    }

    ParseResult* parseResult = parser->parse(tokens);

    if (parseResult->isSuccess()) {
        std::string treeFile = sourceFile + ".tree.txt";
        std::ofstream treeOut(treeFile);
        SyntaxTree::print(parseResult->getTree()->getRoot(), treeOut, 
                         "Syntax Tree for source code: " + sourceFile + "(by " + parserName + ")", 0);
        treeOut.close();
        std::cout << "语法分析成功! 结果已输出到 " << treeFile << std::endl;
    } else {
        std::cerr << parserName << " Parser: parse Error. Error List:" << std::endl;
        const std::vector<std::string>& parseErrors = parseResult->getErrors();
        for (const std::string& error : parseErrors) {
            std::cerr << error << std::endl;
        }
    }

    // 清理资源
    delete parser;
    delete parseResult;

    return 0;
}
