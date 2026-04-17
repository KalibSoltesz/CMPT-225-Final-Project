#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <queue>
#include <utility>

template <typename K, typename T>
class BST {
private:

    struct Node {
        K key;
        T data;
        Node* left;
        Node* right;
        Node* parent;

        Node(K k, T x) : key(k), data(x), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    int nodeCount;

    // ================= HELPERS =================

    Node* findNode(K key) const {
        Node* cur = root;
        while (cur) {
            if (key < cur->key) cur = cur->left;
            else if (key > cur->key) cur = cur->right;
            else return cur;
        }
        return nullptr;
    }

    Node* minimum(Node* n) {
        while (n && n->left) n = n->left;
        return n;
    }

    void inorder(Node* n) {
        if (!n) return;
        inorder(n->left);
        std::cout << n->key << " ";
        inorder(n->right);
    }

    int calcHeight(Node* n) {
        if (!n) return -1;
        std::queue<Node*> q;
        q.push(n);
        int height = 0;
        while (!q.empty()) {
            int size = q.size();
            height++;
            for (int i = 0; i < size; i++) {
                Node* curr = q.front(); q.pop();
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
        }
        return height - 1;
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    BST() : root(nullptr), nodeCount(0) {}

    ~BST() { destroy(root); }

    // ================= INSERT =================

    void put(K key, T data) {
        Node* z = new Node(key, data);

        Node* y = nullptr;
        Node* x = root;

        while (x) {
            y = x;
            if (key < x->key) x = x->left;
            else if (key > x->key) x = x->right;
            else {
                // overwrite existing
                x->data = data;
                delete z;
                return;
            }
        }

        z->parent = y;

        if (!y) root = z;
        else if (key < y->key) y->left = z;
        else y->right = z;

        nodeCount++;
    }

    // ================= SEARCH =================

    T* get(K key) {
        Node* n = findNode(key);
        return n ? &n->data : nullptr;
    }

    bool contains(K key) { return findNode(key) != nullptr; }

    // ================= STATS =================

    int size() { return nodeCount; }

    int height() { return calcHeight(root); }

    bool isEmpty() { return nodeCount == 0; }

    void printInorder() {
        inorder(root);
        std::cout << "\n";
    }

    long long getRotationCount() const { return 0; }

    long long getColourChangeCount() const { return 0; }


    
    // ==================== DELETION ====================

    bool remove(K key) {
        Node* z = findNode(key);
        if (!z) return false;

        if (!z->left) transplant(z, z->right);
        else if (!z->right) transplant(z, z->left);
        else {
            Node* y = minimum(z->right);

            if (y->parent != z) {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
        }

        delete z;
        nodeCount--;
        return true;
    }

private:
    void transplant(Node* u, Node* v) {
        if (!u->parent) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;

        if (v) v->parent = u->parent;
    }
};

#endif