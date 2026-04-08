#ifndef REDBLACK_HPP
#define REDBLACK_HPP

template<typename T>
struct Node {
    bool isRed;
    int key;
    T data;
    Node<T>* left;
    Node<T>* right;
    Node(int k, T x, bool red) : data(x), key(k), isRed(red) {}
};


template<typename T>
class RedBlackTree {
    private:
        Node<T>* root;

        Node<T>* traverseTree(int Key) {
            if(!root) return nullptr;

            Node<T>* curr = root;

            
        }

    public:
        RedBlackTree() : root(nullptr) {}

        // insert a new node, returns false if an error occured
        bool insertNode(int key, T data) {
            if(!root) {
                root = new Node(key, data, false);
                return true;
            }

            

            return false;
        }

        Node<T>* getRoot() {
            return root;
        }

        int getHeight() {
            return -1;
        }
};

#endif