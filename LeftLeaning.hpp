#ifndef LLRB_HPP
#define LLRB_HPP

#include <iostream>
#include <queue>
#include <utility>

template <typename K, typename T>
class LLRB {
private:
    enum Colour { RED, BLACK };

    struct Node {
        K key;
        T data;
        Node* left;
        Node* right;
        Colour colour;

        Node(K k, T x) : key(k), data(x), left(nullptr), right(nullptr), colour(RED) {}
    };

    Node* root;
    int nodeCount;
    long long rotationCount;
    long long colourChangeCount;

    // ================= UTIL =================
    bool isRed(Node* n) { return n && n->colour == RED; }

    void setColour(Node* n, Colour c) {
        if (n && n->colour != c) {
            n->colour = c;
            colourChangeCount++;
        }
    }

    // ================= ROTATIONS =================
    Node* rotateLeft(Node* h) {
        Node* x = h->right;
        h->right = x->left;
        x->left = h;

        x->colour = h->colour;
        h->colour = RED;

        rotationCount++;
        colourChangeCount += 2;

        return x;
    }

    Node* rotateRight(Node* h) {
        Node* x = h->left;
        h->left = x->right;
        x->right = h;

        x->colour = h->colour;
        h->colour = RED;

        rotationCount++;
        colourChangeCount += 2;

        return x;
    }

    void flipColours(Node* h) {
        setColour(h, (h->colour == RED ? BLACK : RED));
        setColour(h->left, (h->left->colour == RED ? BLACK : RED));
        setColour(h->right, (h->right->colour == RED ? BLACK : RED));
    }

    // ================= INSERT =================
    Node* insert(Node* h, K key, T data) {
        if (!h) {
            nodeCount++;
            return new Node(key, data);
        }

        if (key < h->key) h->left = insert(h->left, key, data);
        else if (key > h->key) h->right = insert(h->right, key, data);
        else h->data = data;

        if (isRed(h->right) && !isRed(h->left)) h = rotateLeft(h);

        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        
        if (isRed(h->left) && isRed(h->right)) flipColours(h);

        return h;
    }

    // ================= SEARCH =================
    Node* find(Node* n, K key) {
        while (n) {
            if (key < n->key) n = n->left;
            else if (key > n->key) n = n->right;
            else return n;
        }
        return nullptr;
    }

    // ================= METRICS =================
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
    LLRB() : root(nullptr), nodeCount(0), rotationCount(0), colourChangeCount(0) {}

    ~LLRB() { destroy(root); }

    // ================= PUBLIC API =================
    void put(K key, T data) {
        root = insert(root, key, data);
        setColour(root, BLACK);
    }

    T* get(K key) {
        Node* n = find(root, key);
        return n ? &n->data : nullptr;
    }

    bool contains(K key) { return find(root, key) != nullptr; }

    int size() { return nodeCount; }

    int height() { return calcHeight(root); }

    bool isEmpty() { return nodeCount == 0; }

    void printInorder() {
        inorder(root);
        std::cout << "\n";
    }

    long long getRotationCount() const { return rotationCount; }

    long long getColourChangeCount() const { return colourChangeCount; }



    // ==================== DELETION ====================

    bool remove(K key) {
        if (!contains(key)) return false;

        if (!isRed(root->left) && !isRed(root->right)) setColour(root, RED);

        root = remove(root, key);

        if (root) setColour(root, BLACK);

        return true;
    }

private:
    Node* moveRedLeft(Node* h) {
        flipColours(h);

        if (isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            flipColours(h);
        }
        return h;
    }

    Node* moveRedRight(Node* h) {
        flipColours(h);

        if (isRed(h->left->left)) {
            h = rotateRight(h);
            flipColours(h);
        }
        return h;
    }

    Node* fixUp(Node* h) {
        if (isRed(h->right)) h = rotateLeft(h);

        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);

        if (isRed(h->left) && isRed(h->right)) flipColours(h);

        return h;
    }

    Node* deleteMin(Node* h) {
        if (!h->left) {
            delete h;
            nodeCount--;
            return nullptr;
        }

        if (!isRed(h->left) && !isRed(h->left->left)) h = moveRedLeft(h);

        h->left = deleteMin(h->left);
        return fixUp(h);
    }

    Node* remove(Node* h, K key) {
        if (key < h->key) {
            if (h->left && !isRed(h->left) && !isRed(h->left->left)) h = moveRedLeft(h);
            h->left = remove(h->left, key);
            return fixUp(h);
        }

        if (isRed(h->left)) h = rotateRight(h);

        if (h->right && !isRed(h->right) && !isRed(h->right->left)) h = moveRedRight(h);

        if (key == h->key) {
            if (!h->right) {
                delete h;
                nodeCount--;
                return nullptr;
            }
            Node* x = h->right;
            while (x->left) x = x->left;
            h->key = x->key;
            h->data = x->data;
            h->right = deleteMin(h->right);
        } 
        else {
            h->right = remove(h->right, key);
        }

        return fixUp(h);
    }
};

#endif