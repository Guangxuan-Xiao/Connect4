#include "MCTree.h"

#include <sys/time.h>

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

void MCTree::backUp(int node, double value) {
    while (node != -1) {
        ++nodes[node].cnt;
        nodes[node].value += value;
        value = -value;
        node = nodes[node].parent;
    }
}

int MCTree::bestMove(int node) {
    int ret = -1;
    double maxScore = -INFINITY;
    for (int i = 0; i < initPhase.N; ++i) {
        int child = nodes[node].child[i];
        if (child == -1) continue;
        if (nodes[child].cnt == 0) return i;
        double score = nodes[child].score() +
                       UCB_C * sqrt(log(nodes[node].cnt) / (nodes[child].cnt));
        if (score > maxScore) {
            maxScore = score;
            ret = i;
        }
    }
    return ret;
}

int NodePool::newNode(char player, int parent) {
    pool[size].player = player;
    pool[size].parent = parent;
    pool[size].cnt = pool[size].value = pool[size].end = 0;
    memset(pool[size].child, -1, sizeof(pool[size].child));
    return size++;
}

void MCTree::setPhase(const Phase& phase) {
    initPhase = phase;
    nodes.size = 0;
    root = nodes.newNode(2, -1);
    expand(root);
}

int MCTree::select() {
    int node = root;
    int player = 2;
    curPhase = initPhase;
    while (!curPhase.terminal() && !curPhase.winner() &&
           !nodes[node].isLeaf()) {
        int move = bestMove(node);
        node = nodes[node].child[move];
        curPhase.play(move, player);
        player = 3 - player;
    }
    return node;
}

int MCTree::expand(int node) {
    int player = nodes[node].player;
    for (int i = 0; i < initPhase.N; ++i) {
        if (curPhase.canPlay(i))
            nodes[node].child[i] = nodes.newNode(3 - player, node);
    }
    for (int i = 0; i < initPhase.N; ++i)
        if (nodes[node].child[i] != -1) return nodes[node].child[i];
}

int MCTree::randomPolicy() {
    int nextMove[MAX_N];
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) nextMove[moveNum++] = i;
    if (moveNum == 0) {
        curPhase.printBoard();
    }
    return nextMove[rand() % moveNum];
}

int MCTree::smartPolicy(int player) {
    int nextMove[MAX_N];
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) {
            if (curPhase.isWinningMove(i, player)) return i;
            nextMove[moveNum++] = i;
        }
    return nextMove[rand() % moveNum];
}

double MCTree::rollout(int node) {
    int player = nodes[node].player;
    int sgn = ((player == 2) ? 1 : -1);
    while (!curPhase.terminal()) {
        if (curPhase.userWin()) return curPhase.score() * sgn;
        if (curPhase.machineWin()) return -curPhase.score() * sgn;
        int move = randomPolicy();
        curPhase.play(move, player);
        player = 3 - player;
    }
    return 0;
}

int MCTree::finalDecision() {
    int move = -1;
    double maxScore = INT_MIN;
    for (int i = 0; i < initPhase.N; ++i) {
        int child = nodes[root].child[i];
        if (child == -1) continue;
        double score = nodes[child].score();
#ifdef OUTPUT
        cout << "Move: " << i << " Score: " << score
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
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int times = 0; true; ++times) {
        gettimeofday(&end, NULL);
        int usedTime =
            1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        if (usedTime >= timeLimit) break;
        int cur = select();
        if (nodes[cur].cnt != 0) cur = expand(cur);
        backUp(cur, rollout(cur));
    }
    return finalDecision();
}