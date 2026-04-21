#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <iostream>
#include <vector>
#include <stack>
#include "TreeNode.h"

class SyntaxTree {
private:
    TreeNode* root;

public:
    SyntaxTree();
    SyntaxTree(TreeNode* root);
    ~SyntaxTree();

    TreeNode* getRoot() const;
    void setRoot(TreeNode* root);

    static void print(TreeNode* root, std::ostream& out, const std::string& msg, int offset);
    static void print(TreeNode* root, std::ostream& out);

private:
    static void insert(std::vector<TreeNode*>& list, TreeNode* node, int index);
    static void pushChild(std::stack<TreeNode*>& stack, TreeNode* node);
    static bool isChildOf(TreeNode* node, TreeNode* parent);
};

#endif // SYNTAXTREE_H
