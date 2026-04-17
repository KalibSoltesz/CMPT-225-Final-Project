#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>

#include "BinarySearchTree.hpp"
#include "AVL.hpp"
#include "RedBlack.hpp"
#include "LeftLeaning.hpp"

using namespace std;
using Clock = chrono::high_resolution_clock;

// ================= DATA STRUCTURE FOR RESULTS =================
struct TestResult {
    int size;
    string inputType;
    string treeType;
    double insertTime;
    double searchTime;
    double deleteTime;
    long long insertRotations;
    double avgInsertRotations;
    long long insertColourChanges;
    double avgInsertColourChanges;
    long long deleteRotations;
    double avgDeleteRotations;
    long long deleteColourChanges;
    double avgDeleteColourChanges;
    int treeHeight;
    double heightRatio;
};

// ================= RUN TEST =================
template <typename Tree>
TestResult runTest(const vector<int>& values, const string& name, int size, const string& inputType) {
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

    double avgInsertRot = values.size() > 0 ? double(insertRotations) / values.size() : 0.0;
    double avgInsertCol = values.size() > 0 ? double(insertColourChanges) / values.size() : 0.0;
    double avgDeleteRot = values.size() > 0 ? double(deleteRotations) / values.size() : 0.0;
    double avgDeleteCol = values.size() > 0 ? double(deleteColourChanges) / values.size() : 0.0;

    // Print to console as before
    cout << name << ":\n";
    cout << "  Insert time: " << insertTime << " ms\n";
    cout << "  Search time: " << searchTime << " ms\n";
    cout << "  Delete time: " << deleteTime << " ms\n";
    cout << "  Insert rotations: " << insertRotations
            << " (avg " << avgInsertRot << ")\n";
    cout << "  Insert colour changes: " << insertColourChanges
            << " (avg " << avgInsertCol << ")\n";
    cout << "  Delete rotations: " << deleteRotations
            << " (avg " << avgDeleteRot << ")\n";
    cout << "  Delete colour changes: " << deleteColourChanges
            << " (avg " << avgDeleteCol << ")\n";
    cout << "  Height: " << treeHeight
         << " (height/log2(n)=" << heightRatio << ")\n\n";

    // Return structured result
    return {
        size, inputType, name,
        insertTime, searchTime, deleteTime,
        insertRotations, avgInsertRot,
        insertColourChanges, avgInsertCol,
        deleteRotations, avgDeleteRot,
        deleteColourChanges, avgDeleteCol,
        treeHeight, heightRatio
    };
}

// ================= WRITE CSV =================
void writeCSV(const vector<TestResult>& results, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write header
    file << "Size,InputType,TreeType,InsertTime_ms,SearchTime_ms,DeleteTime_ms,"
         << "InsertRotations,AvgInsertRotations,InsertColourChanges,AvgInsertColourChanges,"
         << "DeleteRotations,AvgDeleteRotations,DeleteColourChanges,AvgDeleteColourChanges,"
         << "TreeHeight,HeightRatio\n";

    // Write data
    file << fixed << setprecision(6);
    for (const auto& result : results) {
        file << result.size << ","
             << result.inputType << ","
             << result.treeType << ","
             << result.insertTime << ","
             << result.searchTime << ","
             << result.deleteTime << ","
             << result.insertRotations << ","
             << result.avgInsertRotations << ","
             << result.insertColourChanges << ","
             << result.avgInsertColourChanges << ","
             << result.deleteRotations << ","
             << result.avgDeleteRotations << ","
             << result.deleteColourChanges << ","
             << result.avgDeleteColourChanges << ","
             << result.treeHeight << ","
             << result.heightRatio << "\n";
    }

    file.close();
    cout << "\nResults exported to " << filename << "\n";
}

// ================= MAIN =================
int main() {
    vector<int> sizes = {100, 1000, 10000, 50000, 100000};
    mt19937_64 rng(42);
    vector<TestResult> allResults;

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

        allResults.push_back(runTest<BST<int, int>>(randomValues, "Standard BST", n, "Random"));
        allResults.push_back(runTest<AVL<int, int>>(randomValues, "AVL Tree", n, "Random"));
        allResults.push_back(runTest<RedBlack<int, int>>(randomValues, "Red-Black Tree", n, "Random"));
        allResults.push_back(runTest<LLRB<int, int>>(randomValues, "Left-Leaning RB Tree", n, "Random"));

        // ================= SORTED INPUT =================
        vector<int> sortedValues(n);
        iota(sortedValues.begin(), sortedValues.end(), 0);

        cout << "----- SORTED INPUT (Worst Case BST) -----\n\n";

        allResults.push_back(runTest<BST<int, int>>(sortedValues, "Standard BST", n, "Sorted"));
        allResults.push_back(runTest<AVL<int, int>>(sortedValues, "AVL Tree", n, "Sorted"));
        allResults.push_back(runTest<RedBlack<int, int>>(sortedValues, "Red-Black Tree", n, "Sorted"));
        allResults.push_back(runTest<LLRB<int, int>>(sortedValues, "Left-Leaning RB Tree", n, "Sorted"));
    }

    // Export results to CSV
    writeCSV(allResults, "test_results.csv");

    return 0;
}