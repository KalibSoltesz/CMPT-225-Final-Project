#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <cmath>

#include "BinarySearchTree.hpp"
#include "AVL.hpp"
#include "RedBlack.hpp"
#include "LeftLeaning.hpp"

using namespace std;
using Clock = chrono::high_resolution_clock;

// ================= RUN TEST =================
template <typename Tree>
void runTest(const vector<int>& values, const string& name) {
    Tree tree;

    // ---------- INSERT ----------
    auto start = Clock::now();
    for (int x : values) tree.put(x, x);
    auto end = Clock::now();

    double insertTime = chrono::duration<double, milli>(end - start).count();

    // ---------- SEARCH ----------
    start = Clock::now();
    for (int x : values) tree.get(x);
    end = Clock::now();

    double searchTime = chrono::duration<double, milli>(end - start).count();

    // ---------- METRICS (Before Delete) ----------
    int treeHeight = tree.height();
    long long insertRotations = tree.getRotationCount();
    long long insertColourChanges = tree.getColourChangeCount();
    
    double heightRatio = values.size() > 0 && treeHeight >= 0
        ? double(treeHeight) / log2(values.size())
        : 0.0;

    // ---------- DELETE ----------
    start = Clock::now();
    for (int x : values) tree.remove(x);
    end = Clock::now();

    double deleteTime = chrono::duration<double, milli>(end - start).count();

    long long totalRotations = tree.getRotationCount();
    long long deleteRotations = totalRotations - insertRotations;
    
    long long totalColourChanges = tree.getColourChangeCount();
    long long deleteColourChanges = totalColourChanges - insertColourChanges;

    cout << name << ":\n";
    cout << "  Insert time: " << insertTime << " ms\n";
    cout << "  Search time: " << searchTime << " ms\n";
    cout << "  Delete time: " << deleteTime << " ms\n";

    double avgInsertRot = values.size() > 0 ? double(insertRotations) / values.size() : 0.0;
    cout << "  Insert rotations: " << insertRotations
            << " (avg " << avgInsertRot << ")\n";

    double avgDeleteRot = values.size() > 0 ? double(deleteRotations) / values.size() : 0.0;
    cout << "  Delete rotations: " << deleteRotations
            << " (avg " << avgDeleteRot << ")\n";

    double avgInsertCol = values.size() > 0 ? double(insertColourChanges) / values.size() : 0.0;
    cout << "  Insert colour changes: " << insertColourChanges
            << " (avg " << avgInsertCol << ")\n";

    double avgDeleteCol = values.size() > 0 ? double(deleteColourChanges) / values.size() : 0.0;
    cout << "  Delete colour changes: " << deleteColourChanges
            << " (avg " << avgDeleteCol << ")\n";

    cout << "  Height: " << treeHeight
         << " (height/log2(n)=" << heightRatio << ")\n\n";
}

// ================= MAIN =================
int main() {
    vector<int> sizes = {10000, 50000, 100000};
    mt19937_64 rng(42);

    cout << fixed << setprecision(3);

    for (int n : sizes) {
        cout << "========================================\n";
        cout << "SIZE: " << n << "\n";
        cout << "========================================\n\n";

        // ================= RANDOM INPUT =================
        vector<int> randomValues(n);
        iota(randomValues.begin(), randomValues.end(), 0);
        shuffle(randomValues.begin(), randomValues.end(), rng);

        cout << "----- RANDOM INPUT -----\n\n";

        runTest<BST<int, int>>(randomValues, "Standard BST");
        runTest<AVL<int, int>>(randomValues, "AVL Tree");
        runTest<RedBlack<int, int>>(randomValues, "Red-Black Tree");
        runTest<LLRB<int, int>>(randomValues, "Left-Leaning RB Tree");

        // ================= SORTED INPUT =================
        vector<int> sortedValues(n);
        iota(sortedValues.begin(), sortedValues.end(), 0);

        cout << "----- SORTED INPUT (Worst Case BST) -----\n\n";

        runTest<BST<int, int>>(sortedValues, "Standard BST");
        runTest<AVL<int, int>>(sortedValues, "AVL Tree");
        runTest<RedBlack<int, int>>(sortedValues, "Red-Black Tree");
        runTest<LLRB<int, int>>(sortedValues, "Left-Leaning RB Tree");
    }

    return 0;
}