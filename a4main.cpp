// Basic test program for AVL Tree
// Author: John Edgar
// Date: July 2022

#include <vector>
#include <iostream>
#include "AVLTree.h"
using std::cout;
using std::endl;
using std::vector;

void a4marking();
template <class K, class V>
bool checkAVLTree(const AVLTree<K, V>& tree);

void bar(int *p) {
    int i = 2;
    *p = i;
}

int foo() {
    int x = 10;
    bar(&x);
    a4marking();
    cout << x;

    return 0;
}

int main()
{
    //a4marking();
    foo();

    cout << endl;
}

void a4marking()
{
    AVLTree<int, int> aspen;

    // Method return values
    bool success = aspen.insert(1, 2);
    int result = aspen.search(1);
    checkAVLTree<int, int>(aspen);
    success = aspen.remove(1);
    vector<int> v2 = aspen.values();
    vector<int> k1 = aspen.keys();
    AVLTree<int, int> larch(aspen);
    aspen = larch;

    cout << endl << "end a4 test" << endl;
}

template <class K, class V>
bool checkAVLTree(const AVLTree<K, V>& tree)
{
    AVLTreeNode<K, V>* nd = (AVLTreeNode<K, V>*)tree.getRoot();
    cout << "height = " << nd->height << endl;

    return false;
}