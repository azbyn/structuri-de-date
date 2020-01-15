#include "avl_tree.h"

#include <iostream>

int main() {
    AVLTree<int, int> t = { 3, 6, 9, 5, 10, 4, 1, 0, 2 };
    printf("before:\n");
    t.printIndented();
    t.remove(6);
    t.remove(4);
    t.remove(0);
    printf("\nafter:\n");
    t.printIndented();
    return 0;
}
