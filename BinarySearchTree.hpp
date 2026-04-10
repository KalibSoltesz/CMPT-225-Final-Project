#ifndef BST_HPP
#define BST_HPP

#include <iostream>

// K must be comparable
template<typename K, typename T>
class BST {
    private:

        struct Node {
            K key;
            T data;
            Node* left;
            Node* right;
            Node(K k, T x) : key(k), data(x), left(nullptr), right(nullptr) {}
        };

        Node* root;
        int treeSize;

        // destroy subtree starting at p
        void destroy(Node* p) {
            if(!p) return
            destroy(p->left);
            destroy(p->right);
            delete p;
        }

        Node* put(Node* p, K key, T data) {
            if(!p) return new Node(key, data);

            if(key < p->key) p->left = put(p->left, key, data);
            else if(key > p->key) p->right = put(p->right, key, data);

            return p;
        }

        // find node with key
        Node* find(Node* p, K key) {
            if(!p || key == p->key) return p;
            if(key < p->key) return find(p->left, key);
            if(key > p->key) return find(p->right, key);
        }

        // delete node with key
        Node* deleteNode(Node* p, K key) {
            if(!p) return nullptr;
            
            if(key < p->key) p->left = deleteNode(p->left, key);
            else if(key > p->key) p->right = deleteNode(p->right, key);
            else {
                treeSize--;
                Node* temp;
                if(!p->left) {
                    temp = p->right;
                    delete p;
                    return temp;
                }

                if(!p->right) {
                    temp = p->left;
                    delete p;
                    return temp;
                }

                temp = p->right;
                while(temp && temp->left) {
                    temp = temp->left;
                }

                p->key = temp->key;
                p->data = temp->data;
                p->right = deleteNode(p->right, temp->key);
            }

            return p;
        }

        int height(Node* p) {
            if(!p) return -1;
            return std::max(height(p->left), height(p->right)) + 1;
        }

    public:
        BST() : root(nullptr), size(0) {}
        ~BST() { destroy(root); root = nullptr; treeSize = 0; };

        void put(K key, T data) { root = put(root, key, data); }

        T* get(K key) { 
            Node* node = find(root, key);
            if(node) return node->data;
            return nullptr;
        }

        bool contains(K key) { return find(root, key); }

        bool remove(K key) {
            if(!contains(key)) return false;
            deleteNode(key);
            return true;
        }

        int size() { return treeSize; }

        int height() { return height(root); }

        bool isEmpty() { return treeSize == 0; }
};

#endif