#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>

// K must be comparable
template<typename K, typename T>
class AVL {
    private:

        // node structure
        struct Node {
            K key;
            T data;
            Node* left;
            Node* right;
            Node* parent;
            int height;
            
            Node(K k, T x) : key(k), data(x), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
        };

        Node* root;
        int treeSize;

        // destroy subtree starting at p recursively
        void destroy(Node* p) {
            if(!p) return;
            destroy(p->left);
            destroy(p->right);
            delete p;
        }

        bool unbalanced(Node* p) {
            if(!p) return false;
            if(std::abs(p->left->height - p->right->height) > 1) return true;
            return false;
        }

        void updateHeight(Node* p) {
            if(p) p->height = std::max(p->left->height, p->right->height) + 1;
        }

        // recursive insertion function
        Node* put(Node* p, K key, T data) {
            if(!p) {
                treeSize++;
                return new Node(key, data);
            }

            if(key < p->key) {
                p->left = put(p->left, key, data);
                p->left->parent = p;
            }
            else if(key > p->key) {
                p->right = put(p->right, key, data);
                p->right->parent = p;
            }
            else return p;

            updateHeight(p);

            return p;
        }

        void rebalance(Node* p) {
            Node* z = p;
            while(!unbalanced(z)) z = z->parent;
            Node* y = (z->left->height > z->right->height ? z->left : z->right);
            Node* x = (y->left->height > y->right->height ? y->left : y->right);

            trinode(z, y, x);
        }

        void trinode(Node* z, Node* y, Node* x) {
            Node* a = z;
            Node* b = y;
            Node* c = x;
            Node* temp;

            while (a->key < b->key || b->key < c->key) {
                if(a->key < b->key) {
                    temp = a;
                    a = b;
                    b = temp;
                }

                if(b->key < c->key) {
                    temp = b;
                    b = c;
                    c = temp;
                }
            }

            b->parent = z->parent;


        }

        // recursive find function
        Node* find(Node* p, K key) {
            if(!p || key == p->key) return p;
            if(key < p->key) return find(p->left, key);
            if(key > p->key) return find(p->right, key);
        }

        // recursive deletion function
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

        // recursive printing functions
        void inorder(Node* p) {
            if(!p) return;
            inorder(p->left);
            std::cout << p->data << ' ';
            inorder(p->right);
        }

        void preorder(Node* p) {
            if(!p) return;
            std::cout << p->data << ' ';
            preorder(p->left);
            preorder(p->right);
        }

        void postorder(Node* p) {
            if(!p) return;
            std::coiut << p->data << ' ';
            postorder(p->left);
            postorder(p->right);
        }

    public:
        // constructor & destructor
        AVL() : root(nullptr), treeSize(0) {}
        ~AVL() { destroy(root); root = nullptr; treeSize = 0; };

        // insert new node
        void put(K key, T data) {
            root = put(root, key, data);
            rebalance(find(key));
        }

        // get data from node with key
        T* get(K key) { 
            Node* node = find(root, key);
            if(node) return node->data;
            return nullptr;
        }
        
        // remove a node with key
        bool remove(K key) {
            if(!contains(key)) return false;
            deleteNode(key);
            return true;
        }

        // find the smallest key
        K* findMin() {
            if(!root) return nullptr;
            Node* curr = root;
            while(curr->left) curr = curr->left;
            return &(curr->key);
        }

        // find the largest key
        K* findMax() {
            if(!root) return nullptr;
            Node* curr = root;
            while(curr->right) curr = curr->right;
            return &(curr->key);
        }

        int height() {
            if(!root) return -1;
            return root->height;
        }

        bool contains(K key) { return find(root, key); }

        int size() { return treeSize; }

        bool isEmpty() { return treeSize == 0; }

        void printInorder() { inorder(root); std::cout << '\n'; }

        void printPreorder() { preorder(root); std::cout << '\n'; }

        void printPostorder() { postorder(root); std::cout << '\n'; }
};

#endif