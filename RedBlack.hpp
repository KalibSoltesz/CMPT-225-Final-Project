#ifndef RB_HPP
#define RB_HPP

#include <iostream>
#include <queue>

template <typename K, typename T>
class RedBlack {
private:
    enum Colour { RED, BLACK };

    struct Node {
        K key;
        T data;
        Node* left;
        Node* right;
        Node* parent;
        Colour colour;

        Node(K k, T x)
            : key(k), data(x), left(nullptr), right(nullptr), parent(nullptr), colour(RED) {}
    };

    Node* root;
    int nodeCount;

    long long rotationCount;
    long long colourChangeCount;

    // ================= ROTATIONS =================
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;

        if (y->left)
            y->left->parent = x;

        y->parent = x->parent;

        if (!x->parent)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        rotationCount++;
    }

    void rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;

        if (x->right)
            x->right->parent = y;

        x->parent = y->parent;

        if (!y->parent)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;

        rotationCount++;
    }

    void setColour(Node* n, Colour c) {
        if (n && n->colour != c) {
            n->colour = c;
            colourChangeCount++;
        }
    }

    // ================= INSERT FIX =================
    void fixInsert(Node* z) {
        while (z->parent && z->parent->colour == RED) {
            Node* gp = z->parent->parent;

            if (z->parent == gp->left) {
                Node* y = gp->right;

                if (y && y->colour == RED) {
                    setColour(z->parent, BLACK);
                    setColour(y, BLACK);
                    setColour(gp, RED);
                    z = gp;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    setColour(z->parent, BLACK);
                    setColour(gp, RED);
                    rotateRight(gp);
                }
            } else {
                Node* y = gp->left;

                if (y && y->colour == RED) {
                    setColour(z->parent, BLACK);
                    setColour(y, BLACK);
                    setColour(gp, RED);
                    z = gp;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    setColour(z->parent, BLACK);
                    setColour(gp, RED);
                    rotateLeft(gp);
                }
            }
        }
        setColour(root, BLACK);
    }

    // ================= SEARCH =================
    Node* findNode(K key) const {
        Node* cur = root;
        while (cur) {
            if (key < cur->key)
                cur = cur->left;
            else if (key > cur->key)
                cur = cur->right;
            else
                return cur;
        }
        return nullptr;
    }

    // ================= MIN =================
    Node* minimum(Node* n) {
        while (n->left)
            n = n->left;
        return n;
    }

    // ================= TRANSPLANT =================
    void transplant(Node* u, Node* v) {
        if (!u->parent)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v)
            v->parent = u->parent;
    }

    // ================= DELETE FIX =================
    void fixDelete(Node* x) {
        while (x != root && (!x || x->colour == BLACK)) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;

                if (w && w->colour == RED) {
                    setColour(w, BLACK);
                    setColour(x->parent, RED);
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if (w && (!w->left || w->left->colour == BLACK) &&
                    (!w->right || w->right->colour == BLACK)) {
                    setColour(w, RED);
                    x = x->parent;
                } else if (w) {
                    if (!w->right || w->right->colour == BLACK) {
                        setColour(w->left, BLACK);
                        setColour(w, RED);
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    setColour(w, x->parent->colour);
                    setColour(x->parent, BLACK);
                    setColour(w->right, BLACK);
                    rotateLeft(x->parent);
                    x = root;
                } else {
                    x = x->parent;
                }
            } else {
                Node* w = x->parent->left;

                if (w && w->colour == RED) {
                    setColour(w, BLACK);
                    setColour(x->parent, RED);
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if (w && (!w->left || w->left->colour == BLACK) &&
                    (!w->right || w->right->colour == BLACK)) {
                    setColour(w, RED);
                    x = x->parent;
                } else if (w) {
                    if (!w->left || w->left->colour == BLACK) {
                        setColour(w->right, BLACK);
                        setColour(w, RED);
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    setColour(w, x->parent->colour);
                    setColour(x->parent, BLACK);
                    setColour(w->left, BLACK);
                    rotateRight(x->parent);
                    x = root;
                } else {
                    x = x->parent;
                }
            }
        }
        if (x)
            setColour(x, BLACK);
    }

    // ================= DEPTH =================
    int calcHeight(Node* n) {
        if (!n) return -1;
        int h = 0;
        // Iterative height calculation using a queue or something, but for simplicity, keep recursive but warn.
        // Actually, to fix, make it iterative.
        // But for now, since n=100k, recursion is too deep.
        // Let's implement iterative height.
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
        return height - 1; // since we start from 0
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
    RedBlack() : root(nullptr), nodeCount(0), rotationCount(0), colourChangeCount(0) {}

    ~RedBlack() {
        destroy(root);
    }

    void put(K key, T data) {
        Node* z = new Node(key, data);
        Node* y = nullptr;
        Node* x = root;

        while (x) {
            y = x;
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else {
                x->data = data;
                delete z;
                return;
            }
        }

        z->parent = y;

        if (!y)
            root = z;
        else if (key < y->key)
            y->left = z;
        else
            y->right = z;

        nodeCount++;
        fixInsert(z);
    }

    T* get(K key) {
        Node* n = findNode(key);
        return n ? &n->data : nullptr;
    }

    bool contains(K key) {
        return findNode(key) != nullptr;
    }

    bool remove(K key) {
        Node* z = findNode(key);
        if (!z) return false;

        Node* y = z;
        Node* x = nullptr;
        Colour yOriginal = y->colour;

        if (!z->left) {
            x = z->right;
            transplant(z, z->right);
        } else if (!z->right) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginal = y->colour;
            x = y->right;

            if (y->parent == z) {
                if (x) x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            setColour(y, z->colour);
        }

        delete z;
        nodeCount--;

        if (yOriginal == BLACK && x)
            fixDelete(x);

        return true;
    }

    int size() {
        return nodeCount;
    }

    int height() {
        return calcHeight(root);
    }

    bool isEmpty() {
        return nodeCount == 0;
    }

    void printInorder() {
        inorder(root);
        std::cout << "\n";
    }

    long long getRotationCount() const {
        return rotationCount;
    }

    long long getColourChangeCount() const {
        return colourChangeCount;
    }
};

#endif