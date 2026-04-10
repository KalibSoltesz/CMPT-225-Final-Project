#include "BinarySearchTree.hpp"

using namespace std;

int main() {

    srand(time(0));

    BST<int> tree;

    for(int i = 0; i < 100; i++) {
        tree.insertNode(rand() % 1000);
    }

    while (true) {
        int input;
        int key;
        cout << "\n(1) search\n(2) delete\n(3) insert\n(4) print inorder\n(5) exit\n\nInput: ";
        cin >> input;

        if(input != 4) {
            cout << "Key: ";
            cin >> key;
        }

        Node<int>* node;

        switch (input) {
            case 1:
                node = tree.searchNode(key);
                if(!node) cout << "not found\n";
                else cout << "found\n";
                break;
            
            case 2:
                tree.deleteNode(key);
                break;

            case 3:
                tree.insertNode(key);
                break;

            case 4:
                cout << '\n';
                tree.inorderTraversal();
                cout << '\n';
                break;
            
            case 5:
                return 0;
        }
    }

    return -1;
}