#ifndef REDBLACK_HPP
#define REDBLACK_HPP

struct Node {
    bool isRed;
    int data;
    Node* left;
    Node* right;
    Node(int x, bool red) : data(x), isRed(red) {}
};


class RedBlackTree {
    private:
        Node* root;

        Node* traverseTree(int data) {
            if(!root) return nullptr;

            Node* curr = root;

            
        }

    public:
        RedBlackTree() : root(nullptr) {}

        // insert a new node
        bool insertNode(int data) {
            if(!root) {
                root = new Node(data, false);
                return true;
            }

            

            return false;
        }

        Node* getRoot() {
            return root;
        }

        int getHeight() {
            return -1;
        }
};

#endif