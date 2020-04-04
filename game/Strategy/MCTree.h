#ifndef MCTREE_H_
#define MCTREE_H_
#include <iostream>

#include "Point.h"
const int MAX_N = 12;
class MCTree {
   public:
    void init(const int M, const int N, const int *top, const int *board);
    Point search(int timeLimit = 2500000);

   private:
    class MCNode {
        int cnt, value;
        short player;
        int parent;
        int child[MAX_N];
        void backProp(int value);

       private:
        bool isLeaf();
    };
    class NodePool {};
};

#endif