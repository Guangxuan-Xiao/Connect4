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
    void reset(char _player, int _parent) {
        player = _player;
        parent = _parent;
        cnt = value = 0;
        memset(child, -1, sizeof(child));
    }

    double score() {
        if (cnt == 0) return 0;
        return value / cnt;
    }
};

struct NodePool {
    NodePool()
        : pool(new Node[NODEPOOL_SIZE]),
          queue(new int[NODEPOOL_SIZE]),
          size(0),
          head(0),
          tail(0) {}
    int newNode(char player, int parent);
    void recycle(int node);
    Node& operator[](int idx) { return pool[idx]; }
    int* queue;
    Node* pool;
    int size;
    int head, tail;
    void push(int node);
    int pop();
    void printNodes() {
        for (int i = 0; i < size; ++i) {
            cout << "Node " << i << endl;
            cout << "Parent: " << pool[i].parent << endl;
            cout << "Child: ";
            for (int j = 0; j < MAX_N; ++j) {
                if (pool[i].child[j] != -1) cout << pool[i].child[j] << " ";
            }
            cout << endl;
        }
    }
};

class MCTree {
   public:
    MCTree() : lastMove(-1) {}
    void setPhase(const Phase& phase, int move);
    int search(int timeLimit = 2000000);

   private:
    Phase initPhase, curPhase;
    int bestMove(int node);
    void backUp(int node, double value);
    int randomPolicy();
    int smartPolicy(int player);
    double rollout(int node);
    int expand(int node);
    int select();
    int finalDecision();
    void moveRoot(int move);
    NodePool nodes;
    int root;
    int lastMove;
};

#endif