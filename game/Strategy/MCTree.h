#ifndef MCTREE_H_
#define MCTREE_H_
#include <iostream>

#include "Phase.hpp"
#include "Point.h"
#include "constant.h"
struct Node {
    int cnt;
    int value;
    char player;
    int parent;
    int child[MAX_N];
    char x, y;
    bool isLeaf();
    void reset(char _player, int _parent) {
        player = _player;
        parent = _parent;
        cnt = value = 0;
        memset(child, -1, sizeof(child));
    }

    void setPoint(char _x, char _y) {
        x = _x;
        y = _y;
    }

    float score() {
        if (cnt == 0) return 0;
        return (float)value / cnt;
    }
};

struct NodePool {
    NodePool() : pool(new Node[NODEPOOL_SIZE]), size(0) {}
    ~NodePool() { delete[] pool; }
    int newNode(char player, int parent);
    int newNode(char player, int parent, char x, char y);
    Node& operator[](int idx) { return pool[idx]; }
    Node* pool;
    int size;
    void printNodes() {
        for (int i = 0; i < size; ++i) {
            cerr << "Node " << i << endl;
            cerr << "Parent: " << pool[i].parent << endl;
            cerr << "Child: ";
            for (int j = 0; j < MAX_N; ++j) {
                if (pool[i].child[j] != -1) cerr << pool[i].child[j] << " ";
            }
            cerr << endl;
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
    void backUp(int node, int value);
    int randomPolicy();
    int smartPolicy(int player);
    int rollout(int node);
    int expand(int node);
    int select();
    int finalDecision();
    int centerSample(int moveNum);
    int scoreSample(int moveNum, int player);
    void moveRoot(int move);
    float moveScore(int node);
    NodePool nodes;
    int root;
    int lastMove;
    int nextMove[MAX_N];
    int score[MAX_N];
    int moveValue[MAX_M][MAX_N];
    int moveCnt[MAX_M][MAX_N];
};

#endif
