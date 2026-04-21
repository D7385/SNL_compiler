#include "frontend/syntax/TreeNode.h"

TreeNode::TreeNode(const std::string& value) : value(value), width(value.length()), printed(false) {
}

TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
}

bool TreeNode::hasChild() const {
    for (TreeNode* child : children) {
        if (child != nullptr) {
            return true;
        }
    }
    return false;
}

bool TreeNode::hasChildNotPrinted() const {
    for (TreeNode* child : children) {
        if (child != nullptr && !child->printed) {
            return true;
        }
    }
    return false;
}

const std::vector<TreeNode*>& TreeNode::getChildren() const {
    return children;
}

void TreeNode::setChildren(const std::vector<TreeNode*>& nodes) {
    children = nodes;
}

const std::string& TreeNode::getValue() const {
    return value;
}

void TreeNode::setValue(const std::string& value) {
    this->value = value;
    width = value.length();
}

int TreeNode::getWidth() const {
    return width;
}

void TreeNode::setPrinted(bool printed) {
    this->printed = printed;
}

std::string TreeNode::toString() const {
    return "[TreeNode value=" + value + "]";
}
