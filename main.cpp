#include "avltree.h"
#include <iostream>

int main()
{
    AVLTree<int> tree;
    tree.insert(133);
    tree.insert(134);
    tree.insert(135);
    tree.insert(136);
    tree.insert(196);
    tree.insert(197);
    tree.insert(470);
    tree.insert(471);
    tree.insert(700);
    std::cout << tree.serialize() << "\n";
    tree.remove(133);
    std::cout << tree.serialize() << "\n";
}
