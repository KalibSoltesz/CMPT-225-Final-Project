#include "BinarySearchTree.hpp"

using namespace std;

int main() {

    AVL<int, int> tree;

    for(int i = 0; i < 100; i++) {
        int temp = rand() % 1000;
        tree.put(temp, temp);
    }

    tree.printInorder();

    // while (true) {
    //     int input;
    //     int key;
    //     cout << "\n(1) search\n(2) delete\n(3) insert\n(4) print inorder\n(5) exit\n\nInput: ";
    //     cin >> input;

    //     if(input != 4) {
    //         cout << "Key: ";
    //         cin >> key;
    //     }

    //     switch (input) {
    //         // insert
    //         case 1:
    //             break;
            
    //         // search
    //         case 2:
    //             break;

    //         // delete
    //         case 3:
    //             break;

    //         // print inorder
    //         case 4:
    //             break;
            
    //         // exit
    //         case 5:
    //             return 0;
    //     }
    // }

    return 0;
}