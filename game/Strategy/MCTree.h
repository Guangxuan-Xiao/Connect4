#ifndef MCTREE_H_
#define MCTREE_H_
#include <iostream>

#include "Phase.hpp"
#include "Point.h"
#include "constant.h"
struct Node {
    int cnt;
    double value;
    char player;
    int parent;
    int child[MAX_N];
    bool isLeaf();
    double score() { return value / cnt; }
};

struct NodePool {
    int newNode(char player, int parent);
    Node& operator[](int idx) { return pool[idx]; }
    Node pool[NODEPOOL_SIZE];
    int size;
    void printNodes() {
        for (int i = 0; i < size; ++i) {
            cout << "Node " << i << endl;
            cout << "Parent: " << pool[i].parent << endl;
            cout << "Child: ";
            for (int j = 0; j < MAX_N;++j) {
                if (pool[i].child[j] !=-1) 
                    cout << pool[i].child[j] << " ";
            }
            cout << endl;
        }
    }
};

class MCTree {
   public:
    void setPhase(const Phase& phase);
    int search(int timeLimit = 2000000);

   private:
    Phase initPhase, curPhase;
    int bestMove(int node);
    void backUp(int node, double value);
    int randomPolicy();
    int smartPolicy(int player);
    double rollout(int player);
    int expand(int node);
    int select();
    int finalDecision();
    NodePool nodes;
    int root;
    int M, N;
};

#endif