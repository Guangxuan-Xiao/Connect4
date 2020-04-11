#include "MCTree.h"

#include <time.h>

#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;
bool Node::isLeaf() {
    for (int i = 0; i < MAX_N; ++i) {
        if (child[i] != -1) return false;
    }
    return true;
}

void MCTree::backUp(int node, float value) {
    while (node != -1) {
        ++nodes[node].cnt;
        nodes[node].value += value;
        value = -value;
        node = nodes[node].parent;
    }
}

int MCTree::bestMove(int node) {
    int ret = -1;
    float maxScore = -INFINITY;
    float log_N = log(nodes[node].cnt + 0.001);
    for (int i = 0; i < initPhase.N; ++i) {
        int child = nodes[node].child[i];
        if (child == -1) continue;
        if (nodes[child].cnt == 0) return i;
        float score =
            nodes[child].score() + UCB_C * sqrt(log_N / (nodes[child].cnt));
        if (score > maxScore) {
            maxScore = score;
            ret = i;
        }
    }
    return ret;
}

int NodePool::newNode(char player, int parent) {
    pool[size].reset(player, parent);
    return size++;
}

void MCTree::moveRoot(int move) {
    if (nodes[root].child[move] == -1)
        nodes[root].reset(2, -1);
    else {
        root = nodes[root].child[move];
        nodes[root].parent = -1;
    }
}

void MCTree::setPhase(const Phase& phase, int move) {
    initPhase = phase;
    curPhase = phase;
    if (lastMove == -1 || (NODEPOOL_SIZE - nodes.size) <= 2e6) {
        nodes.size = 0;
        root = nodes.newNode(2, -1);
    } else {
        moveRoot(lastMove);
        moveRoot(move);
    }
    if (nodes[root].isLeaf()) expand(root);
}

int MCTree::select() {
    int node = root;
    int player = 2;
    curPhase = initPhase;
    while (!nodes[node].isLeaf()) {
        int move = bestMove(node);
        node = nodes[node].child[move];
        curPhase.play(move, player);
        player = 3 - player;
    }
    return node;
}

int MCTree::expand(int node) {
    int player = nodes[node].player;
    if (curPhase.terminal() || curPhase.winner()) return node;
    int urgent = -1;
    for (int i = 0; i < initPhase.N; ++i) {
        if (curPhase.canPlay(i)) {
            if (curPhase.isWinningMove(i, player)) {
                nodes[node].child[i] = nodes.newNode(3 - player, node);
                for (int j = 0; j < i; ++j) nodes[node].child[j] = -1;
                return nodes[node].child[i];
            }
            if (!curPhase.isLosingMove(i, player))
                nodes[node].child[i] = nodes.newNode(3 - player, node);
            if (curPhase.isWinningMove(i, 3 - player)) urgent = i;
        }
    }
    if (urgent != -1) {
        for (int i = 0; i < urgent; ++i) nodes[node].child[i] = -1;
        return nodes[node].child[urgent] = nodes.newNode(3 - player, node);
    }
    for (int i = 0; i < initPhase.N; ++i)
        if (nodes[node].child[i] != -1) return nodes[node].child[i];
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i))
            return nodes[node].child[i] = nodes.newNode(3 - player, node);
}

int MCTree::weightedSample(int moveNum) {
    int i = 0;
    for (int j = 0; j < moveNum; ++j) {
        int weight = 0, mid = (moveNum >> 1);
        if (j < mid)
            weight = j / 2 + 1;
        else
            weight = (moveNum - 1 - j) / 2 + 1;
        for (int k = i; k < i + weight; ++k) adjustedMove[k] = j;
        i += weight;
    }
    return adjustedMove[rand() % i];
}

int MCTree::randomPolicy() {
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) nextMove[moveNum++] = i;
    return nextMove[weightedSample(moveNum)];
}

int MCTree::smartPolicy(int player) {
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) {
            // if (curPhase.isWinningMove(i, player)) return i;
            if (curPhase.isWinningMove(i, 3 - player)) return i;
            // if (!curPhase.isLosingMove(i, player))
            nextMove[moveNum++] = i;
        }
    if (moveNum == 0) {
        for (int i = 0; i < initPhase.N; ++i)
            if (curPhase.canPlay(i)) return i;
    }
    return nextMove[weightedSample(moveNum)];
}

float MCTree::rollout(int node) {
    int player = nodes[node].player;
    int sgn = ((player == 2) ? 1 : -1);
    while (!curPhase.terminal()) {
        if (curPhase.userWin()) return sgn;
        if (curPhase.machineWin()) return -sgn;
        // int move = smartPolicy(player);
        int move = randomPolicy();
        curPhase.play(move, player);
        player = 3 - player;
    }
    return 0;
}

int MCTree::finalDecision() {
    int move = -1;
    float maxScore = INT_MIN;
    for (int i = 0; i < initPhase.N; ++i) {
        int child = nodes[root].child[i];
        if (child == -1) continue;
        float score = nodes[child].score();
#ifdef OUTPUT
        cerr << "Move: " << i << " Score: " << score
             << " Cnt: " << nodes[child].cnt << " Value: " << nodes[child].value
             << endl;
#endif
        if (score > maxScore) {
            maxScore = score;
            move = i;
        }
    }
    return move;
}

int MCTree::search(int timeLimit) {
    time_t start = clock();
    for (int times = 0; true; ++times) {
        int usedTime = clock() - start;
        if (usedTime >= timeLimit) break;
        int cur = select();
        if (nodes[cur].cnt != 0) cur = expand(cur);
        backUp(cur, rollout(cur));
    }
    return lastMove = finalDecision();
}
