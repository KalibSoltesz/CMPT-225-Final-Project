#ifndef BST_HPP
#define BST_HPP

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int x) : data(x) {}
};

class BinarySearchTree {
    private:
        Node* root;

        Node* recursiveInsert(Node* p, int key) {

            if (!p) return new Node(key);

            if (key < p->data) p->left = recursiveInsert(p->left, key);
            
            else if (key > p->data) p->right = recursiveInsert(p->right, key);

            return p;
        }

        Node* recursiveSearch(Node* p, int key) {
            if (p->data == key) return p;
            if (key < p->data) return recursiveInsert(p->left, key);
            if (key > p->data) return recursiveInsert(p->right, key);
        }

        Node* recursiveDelete(Node* p, int key) {
            if(p->data == key) {
                
            }
        }

    public:
        BinarySearchTree() : root(nullptr) {}

        // insert a new node
        void insertNode(int data) { recursiveInsert(root, data); }

        Node* searchNode(int data) { recursiveSearch(root, data); }

        void deleteNode(int data) { recursiveDelete(root, data); }
};

#endif