#include "frontend/syntax/SyntaxTree.h"

SyntaxTree::SyntaxTree() : root(nullptr) {
}

SyntaxTree::SyntaxTree(TreeNode* root) : root(root) {
}

SyntaxTree::~SyntaxTree() {
    delete root;
}

TreeNode* SyntaxTree::getRoot() const {
    return root;
}

void SyntaxTree::setRoot(TreeNode* root) {
    this->root = root;
}

void SyntaxTree::print(TreeNode* root, std::ostream& out, const std::string& msg, int offset) {
    if (root == nullptr) return;
    
    for (int i = 0; i <= offset; i++) out << " ";
    out << msg << std::endl;
    
    for (int i = 0; i < offset; i++) out << " ";
    out << " " << root->getValue();
    
    bool isLineHead = false;
    int index = 0;
    std::vector<TreeNode*> list;
    std::stack<TreeNode*> stack;
    
    list.push_back(root);
    pushChild(stack, root);
    
    while (!stack.empty()) {
        TreeNode* node = stack.top();
        stack.pop();
        
        if (!isLineHead) {
            out << "_" << node->getValue();
            node->setPrinted(true);
            
            if (node->hasChild()) {
                insert(list, node, ++index);
                pushChild(stack, node);
                isLineHead = false;
            } else {
                isLineHead = true;
                index = 0;
                out << std::endl;
                for (int i = 0; i < offset; i++) out << " ";
            }
        } else {
            for (int i = 0; i < list.size(); i++) {
                TreeNode* temp = list[i];
                index = i;
                
                if (isChildOf(node, temp)) {
                    for (int j = 0; j < temp->getWidth(); j++) out << " ";
                    out << "|_" << node->getValue();
                    node->setPrinted(true);
                    
                    if (node->hasChild()) {
                        insert(list, node, ++index);
                        pushChild(stack, node);
                        isLineHead = false;
                    } else {
                        out << std::endl;
                        for (int j = 0; j < offset; j++) out << " ";
                        index = 0;
                        isLineHead = true;
                    }
                    break;
                } else {
                    if (temp->hasChildNotPrinted()) {
                        for (int j = 0; j < temp->getWidth(); j++) out << " ";
                        out << "|";
                    } else {
                        for (int j = 0; j <= temp->getWidth(); j++) out << " ";
                    }
                }
            }
        }
    }
}

void SyntaxTree::print(TreeNode* root, std::ostream& out) {
    print(root, out, "", 0);
}

void SyntaxTree::insert(std::vector<TreeNode*>& list, TreeNode* node, int index) {
    if (list.size() <= index) {
        list.push_back(node);
    } else {
        list[index] = node;
    }
}

void SyntaxTree::pushChild(std::stack<TreeNode*>& stack, TreeNode* node) {
    if (node->hasChild()) {
        const std::vector<TreeNode*>& children = node->getChildren();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            if (*it != nullptr) {
                stack.push(*it);
            }
        }
    }
}

bool SyntaxTree::isChildOf(TreeNode* node, TreeNode* parent) {
    if (parent == nullptr) return false;
    
    const std::vector<TreeNode*>& children = parent->getChildren();
    for (TreeNode* n : children) {
        if (n == node) return true;
    }
    return false;
}
