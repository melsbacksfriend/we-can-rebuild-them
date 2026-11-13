#ifndef AVLTREE_H
#define AVLTREE_H

#include <algorithm>
#include <memory>
#include <vector>
#include <string>

template <class T>
class AVLTree {
public:

    typedef struct Node {
        int bf;
        T value;
        int height;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
    } Node;

    AVLTree();
    int height();
    int size();
    bool empty();
    bool contains(T value);
    bool insert(T value);
    bool remove(T value);
    std::string serialize();
    std::shared_ptr<Node> root;

private:
    int nodeCount;
    bool contains(std::shared_ptr<Node> node, T value);
    std::shared_ptr<Node> insert(std::shared_ptr<Node> node, T value);
    void update(std::shared_ptr<Node> node);
    std::shared_ptr<Node> balance(std::shared_ptr<Node> node);
    std::shared_ptr<Node> leftLeftCase(std::shared_ptr<Node> node);
    std::shared_ptr<Node> leftRightCase(std::shared_ptr<Node> node);
    std::shared_ptr<Node> rightRightCase(std::shared_ptr<Node> node);
    std::shared_ptr<Node> rightLeftCase(std::shared_ptr<Node> node);
    std::shared_ptr<Node> leftRotation(std::shared_ptr<Node> node);
    std::shared_ptr<Node> rightRotation(std::shared_ptr<Node> node);
    std::shared_ptr<Node> remove(std::shared_ptr<Node> node, T value);
    T findMin(std::shared_ptr<Node> node);
    T findMax(std::shared_ptr<Node> node);
    void serializeHelper(std::shared_ptr<Node> root, int level, std::vector<std::vector<std::string>>& levels);
};

template <class T>
std::string AVLTree<T>::serialize()
{
    std::vector<std::vector<std::string>> levels(0);
    std::string res;
    serializeHelper(root, 0, levels);
    if (levels.empty()) return "";
    for (auto& level: levels)
    {
        for (std::string& val: level)
        {
            res += val + ", ";
        }
    }
    res.pop_back();
    res.pop_back();
    return res;
}

template <class T>
void AVLTree<T>::serializeHelper(std::shared_ptr<Node> root, int level, std::vector<std::vector<std::string>>& levels)
{
    if (levels.size() <= level)
    {
        levels.push_back({});
    }
    if (root == nullptr)
    {
        levels[level].push_back("null");
        return;
    }
    levels[level].push_back(std::to_string(root->value));
    serializeHelper(root->left, level + 1, levels);
    serializeHelper(root->right, level + 1, levels);
}

template <class T>
AVLTree<T>::AVLTree()
{
    root = nullptr;
    nodeCount = 0;
}

template <class T>
int AVLTree<T>::height()
{
    if (root == nullptr) return 0;
    return root->height;
}

template <class T>
int AVLTree<T>::size()
{
    return nodeCount;
}

template <class T>
bool AVLTree<T>::empty()
{
    return nodeCount == 0;
}

template <class T>
bool AVLTree<T>::contains(T value)
{
    return contains(root, value);
}

template <class T>
bool AVLTree<T>::contains(std::shared_ptr<Node> node, T value)
{
    if (node == nullptr) return false;
    int cmp = value - node->value;
    if (cmp < 0) return contains(node->left, value);
    if (cmp > 0) return contains(node->right, value);
    return true;
}

template <class T>
bool AVLTree<T>::insert(T value)
{
    if (!contains(root, value))
    {
        root = insert(root, value);
        nodeCount++;
        return true;
    }
    return false;
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::insert(std::shared_ptr<Node> node, T value)
{
    if (node == nullptr)
    {
        std::shared_ptr<Node> result = std::make_shared<Node>();
        result->value = value;
        result->bf = 0;
        result->height = 0;
        result->left = nullptr;
        result->right = nullptr;
        return result;
    }
    int cmp = value - node->value;
    if (cmp < 0) node->left = insert(node->left, value);
    else node->right = insert(node->right, value);
    update(node);
    return balance(node);
}

template <class T>
void AVLTree<T>::update(std::shared_ptr<Node> node)
{
    int leftNodeHeight = (node->left == nullptr) ? -1 : node->left->height;
    int rightNodeHeight = (node->right == nullptr) ? -1 : node->right->height;
    node->height = 1 + std::max(leftNodeHeight, rightNodeHeight);
    node->bf = rightNodeHeight - leftNodeHeight;
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::balance(std::shared_ptr<Node> node)
{
    if (node->bf == -2)
    {
        if (node->left->bf <= 0)
        {
            return leftLeftCase(node);
        }
        else
        {
            return leftRightCase(node);
        }
    }
    else if (node->bf == 2)
    {
        if (node->right->bf >= 0)
        {
            return rightRightCase(node);
        }
        else
        {
            return rightLeftCase(node);
        }
    }
    return node;
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::leftLeftCase(std::shared_ptr<Node> node)
{
    return rightRotation(node);
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::leftRightCase(std::shared_ptr<Node> node)
{
    node->left = leftRotation(node->left);
    return leftLeftCase(node);
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::rightRightCase(std::shared_ptr<Node> node)
{
    return leftRotation(node);
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::rightLeftCase(std::shared_ptr<Node> node)
{
    node->right = rightRotation(node->right);
    return rightRightCase(node);
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::leftRotation(std::shared_ptr<Node> node)
{
    std::shared_ptr<Node> newParent = node->right;
    node->right = newParent->left;
    newParent->left = node;
    update(node);
    update(newParent);
    return newParent;
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::rightRotation(std::shared_ptr<Node> node)
{
    std::shared_ptr<Node> newParent = node->left;
    node->left = newParent->right;
    newParent->right = node;
    update(node);
    update(newParent);
    return newParent;
}

template <class T>
bool AVLTree<T>::remove(T value)
{
    if (contains(root, value))
    {
        root = remove(root, value);
        nodeCount--;
        return true;
    }
    return false;
}

template <class T>
std::shared_ptr<typename AVLTree<T>::Node> AVLTree<T>::remove(std::shared_ptr<Node> node, T value)
{
    if (node == nullptr) return nullptr;
    int cmp = value - node->value;
    if (cmp < 0)
    {
        node->left = remove(node->left, value);
    }
    else if (cmp > 0)
    {
        node->right = remove(node->right, value);
    }
    else
    {
        if (node->left == nullptr) return node->right;
        else if (node->right == nullptr) return node->left;
        else
        {
            if (node->left->height > node->right->height)
            {
                T successorValue = findMax(node->left);
                node->value = successorValue;
                node->left = remove(node->left, successorValue);
            }
            else
            {
                T successorValue = findMin(node->right);
                node->value = successorValue;
                node->right = remove(node->right, successorValue);
            }
        }
    }
    update(node);
    return balance(node);
}

template <class T>
T AVLTree<T>::findMin(std::shared_ptr<Node> node)
{
    while (node->left != nullptr) node = node->left;
    return node->value;
}

template <class T>
T AVLTree<T>::findMax(std::shared_ptr<Node> node)
{
    while (node->right != nullptr) node = node->right;
    return node->value;
}

#endif // AVLTREE_H
