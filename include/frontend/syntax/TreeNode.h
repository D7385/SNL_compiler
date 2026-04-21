#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <string>

class TreeNode {
private:
    std::vector<TreeNode*> children;
    std::string value;
    int width;
    bool printed;

public:
    TreeNode(const std::string& value);
    ~TreeNode();

    bool hasChild() const;
    bool hasChildNotPrinted() const;

    const std::vector<TreeNode*>& getChildren() const;
    void setChildren(const std::vector<TreeNode*>& nodes);

    const std::string& getValue() const;
    void setValue(const std::string& value);

    int getWidth() const;
    void setPrinted(bool printed);

    std::string toString() const;
};

#endif // TREENODE_H
