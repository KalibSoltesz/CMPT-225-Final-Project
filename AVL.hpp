#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>

template<typename K, typename T>
class AVL {
private:
    struct Node {
        K key;
        T data;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        Node(K k, T x)
            : key(k), data(x), left(nullptr), right(nullptr),
              parent(nullptr), height(0) {}
    };

    Node* root;
    int nodeCount;
    long long rotationCount;

    // ================= HELPERS =================

    int calcHeight(Node* n) const {
        return n ? n->height : -1;
    }

    void updateHeight(Node* n) {
        if (n)
            n->height = 1 + std::max(calcHeight(n->left), calcHeight(n->right));
    }

    int getBalance(Node* n) const {
        return n ? calcHeight(n->left) - calcHeight(n->right) : 0;
    }

    Node* rotateLeft(Node* x) {
        rotationCount++;

        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        if (T2) T2->parent = x;

        y->parent = x->parent;
        x->parent = y;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* rotateRight(Node* y) {
        rotationCount++;

        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        if (T2) T2->parent = y;

        x->parent = y->parent;
        y->parent = x;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* balance(Node* n) {
        updateHeight(n);

        int bf = getBalance(n);

        if (bf > 1) {
            if (getBalance(n->left) < 0)
                n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        if (bf < -1) {
            if (getBalance(n->right) > 0)
                n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        return n;
    }

    Node* insert(Node* n, K key, T data) {
        if (!n) {
            nodeCount++;
            return new Node(key, data);
        }

        if (key < n->key) {
            n->left = insert(n->left, key, data);
            n->left->parent = n;
        } else if (key > n->key) {
            n->right = insert(n->right, key, data);
            n->right->parent = n;
        } else {
            n->data = data;
            return n;
        }

        return balance(n);
    }

    Node* minimum(Node* n) {
        while (n && n->left)
            n = n->left;
        return n;
    }

    Node* removeNode(Node* n, K key) {
        if (!n) return nullptr;

        if (key < n->key)
            n->left = removeNode(n->left, key);
        else if (key > n->key)
            n->right = removeNode(n->right, key);
        else {
            if (!n->left || !n->right) {
                nodeCount--;
                Node* temp = n->left ? n->left : n->right;
                delete n;
                return temp;
            }

            Node* y = minimum(n->right);
            n->key = y->key;
            n->data = y->data;
            n->right = removeNode(n->right, y->key);
        }

        return balance(n);
    }

    Node* findNode(K key) const {
        Node* cur = root;
        while (cur) {
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
            else return cur;
        }
        return nullptr;
    }

    void inorder(Node* n) {
        if (!n) return;
        inorder(n->left);
        std::cout << n->key << " ";
        inorder(n->right);
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    AVL() : root(nullptr), nodeCount(0), rotationCount(0) {}
    ~AVL() { destroy(root); }

    // ================= INSERT =================

    void put(K key, T data) {
        root = insert(root, key, data);
        if (root) root->parent = nullptr;
    }

    // ================= SEARCH =================

    T* get(K key) {
        Node* n = findNode(key);
        return n ? &n->data : nullptr;
    }

    bool contains(K key) {
        return findNode(key) != nullptr;
    }

    // ================= DELETE =================

    bool remove(K key) {
        if (!contains(key)) return false;
        root = removeNode(root, key);
        return true;
    }

    // ================= STATS =================

    int size() const { return nodeCount; }

    int height() const { return calcHeight(root); }

    bool isEmpty() const { return nodeCount == 0; }

    void printInorder() {
        inorder(root);
        std::cout << "\n";
    }

    long long getRotationCount() const { return rotationCount; }

    long long getColourChangeCount() const { return 0; }
};

#endif