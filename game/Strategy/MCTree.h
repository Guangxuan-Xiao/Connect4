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
		size(0) {
	}
	~NodePool() {
		delete[] pool;
	}
	int newNode(char player, int parent);
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
	void backUp(int node, double value);
	int randomPolicy();
	int smartPolicy(int player);
	double rollout(int node);
	int expand(int node);
	int select();
	int finalDecision();
	// int weightedSample(int moveNum);
	void moveRoot(int move);
	NodePool nodes;
	int root;
	int lastMove;
	int nextMove[MAX_N];
	int adjustedMove[MAX_N << 2];
};

#endif