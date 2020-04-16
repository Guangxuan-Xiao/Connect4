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

// void MCTree::backUp(int node, int value) {
//     while (node != -1) {
//         ++nodes[node].cnt;
//         nodes[node].value += value;
//         value = -value;
//         node = nodes[node].parent;
//     }
// }

void MCTree::backUp(int node, int value) {
    while (node != -1) {
        ++nodes[node].cnt;
        ++moveCnt[nodes[node].x][nodes[node].y];
        nodes[node].value += value;
        moveValue[nodes[node].x][nodes[node].y] += value;
        value = -value;
        node = nodes[node].parent;
    }
}

float MCTree::moveScore(int node) {
    char x = nodes[node].x, y = nodes[node].y;
    return (float)moveValue[x][y] / moveCnt[x][y];
}

int MCTree::bestMove(int node) {
    int ret = -1;
    float maxScore = -INFINITY;
    float log_N = logf(nodes[node].cnt + 0.001);
    float beta = sqrtf(K / (3 * nodes[node].cnt + K));
    for (int i = 0; i < initPhase.N; ++i) {
        int child = nodes[node].child[i];
        if (child == -1) continue;
        if (nodes[child].cnt == 0) return i;
        // float score =
        //     nodes[child].score() + UCB_C * sqrtf(log_N / (nodes[child].cnt));
        float score = (1 - beta) * nodes[child].score() +
                      beta * moveScore(node) +
                      UCB_C * sqrtf(log_N / (nodes[child].cnt));
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

int NodePool::newNode(char player, int parent, char x, char y) {
    pool[size].reset(player, parent);
    pool[size].setPoint(x, y);
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
    memset(moveValue, 0, sizeof(moveValue));
    memset(moveCnt, 0, sizeof(moveCnt));
    if (lastMove == -1 || (NODEPOOL_SIZE - nodes.size) <= 2e6) {
        nodes.size = 0;
        root = nodes.newNode(2, -1);
    } else {
        moveRoot(lastMove);
        moveRoot(move);
    }
    nodes[root].setPoint(curPhase.getX(move), move);
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

// int MCTree::expand(int node) {
//     int player = nodes[node].player;
//     if (curPhase.terminal() || curPhase.winner()) return node;
//     int urgent = -1;
//     for (int i = 0; i < initPhase.N; ++i) {
//         if (curPhase.canPlay(i)) {
//             if (curPhase.isWinningMove(i, player)) {
//                 nodes[node].child[i] = nodes.newNode(3 - player, node);
//                 for (int j = 0; j < i; ++j) nodes[node].child[j] = -1;
//                 return nodes[node].child[i];
//             }
//             if (!curPhase.isLosingMove(i, player))
//                 nodes[node].child[i] = nodes.newNode(3 - player, node);
//             if (curPhase.isWinningMove(i, 3 - player)) urgent = i;
//         }
//     }
//     if (urgent != -1) {
//         for (int i = 0; i < urgent; ++i) nodes[node].child[i] = -1;
//         return nodes[node].child[urgent] = nodes.newNode(3 - player, node);
//     }
//     for (int i = 0; i < initPhase.N; ++i)
//         if (nodes[node].child[i] != -1) return nodes[node].child[i];
//     for (int i = 0; i < initPhase.N; ++i)
//         if (curPhase.canPlay(i))
//             return nodes[node].child[i] = nodes.newNode(3 - player, node);
// }

int MCTree::expand(int node) {
    int player = nodes[node].player;
    if (curPhase.terminal() || curPhase.winner()) return node;
    int urgent = -1;
    for (int i = 0; i < initPhase.N; ++i) {
        char x = curPhase.getX(i);
        if (curPhase.canPlay(i)) {
            if (curPhase.isWinningMove(i, player)) {
                nodes[node].child[i] = nodes.newNode(3 - player, node, x, i);
                for (int j = 0; j < i; ++j) nodes[node].child[j] = -1;
                return nodes[node].child[i];
            }
            if (!curPhase.isLosingMove(i, player))
                nodes[node].child[i] = nodes.newNode(3 - player, node, x, i);
            if (curPhase.isWinningMove(i, 3 - player)) urgent = i;
        }
    }
    if (urgent != -1) {
        for (int i = 0; i < urgent; ++i) nodes[node].child[i] = -1;
        char x = curPhase.getX(urgent);
        return nodes[node].child[urgent] =
                   nodes.newNode(3 - player, node, x, urgent);
    }
    for (int i = 0; i < initPhase.N; ++i)
        if (nodes[node].child[i] != -1) return nodes[node].child[i];
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) {
            char x = curPhase.getX(i);
            return nodes[node].child[i] = nodes.newNode(3 - player, node, x, i);
        }
}

int MCTree::centerSample(int moveNum) {
    int totalScore = 0, mid = (moveNum >> 1);
    for (int i = 0; i < moveNum; ++i) {
        score[i] = i < mid ? i / 2 + 1 : (moveNum - 1 - i) / 2 + 1;
        totalScore += score[i];
    }
    if (totalScore == 0) return 0;
    int randNum = rand() % totalScore;
    int move = -1;
    do {
        ++move;
        randNum -= score[move];
    } while (randNum > 0);
    return move;
}

int MCTree::scoreSample(int moveNum, int player) {
    int totalScore = 0;
    for (int i = 0; i < moveNum; ++i) {
        score[i] = curPhase.moveScore(nextMove[i], player);
        totalScore += score[i];
    }
    if (totalScore == 0) return 0;
    int randNum = rand() % totalScore;
    int move = -1;
    do {
        ++move;
        randNum -= score[move];
    } while (randNum > 0);
    return move;
}

int MCTree::randomPolicy() {
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) nextMove[moveNum++] = i;
    return nextMove[centerSample(moveNum)];
}

int MCTree::smartPolicy(int player) {
    int moveNum = 0;
    for (int i = 0; i < initPhase.N; ++i)
        if (curPhase.canPlay(i)) nextMove[moveNum++] = i;
    return nextMove[scoreSample(moveNum, player)];
}

int MCTree::rollout(int node) {
    int player = nodes[node].player;
    int sgn = ((player == 2) ? 1 : -1);
    while (!curPhase.terminal()) {
        if (curPhase.userWin()) return sgn;
        if (curPhase.machineWin()) return -sgn;
        // int move = randomPolicy();
        int move = smartPolicy(player);
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
        cout << "Move: " << i << " Score: " << score
             << " Cnt: " << nodes[child].cnt << " Value: " << nodes[child].value
             << endl;
#endif
        if (score > maxScore) {
            maxScore = score;
            move = i;
        }
    }
#ifdef OUTPUT
    cout << "Confidence: " << maxScore << endl;
#endif
    return move;
}

int MCTree::search(int timeLimit) {
    time_t start = clock();
    while (true) {
        int usedTime = clock() - start;
        if (usedTime >= timeLimit) break;
        int cur = select();
        if (nodes[cur].cnt != 0) cur = expand(cur);
        backUp(cur, rollout(cur));
    }
    return lastMove = finalDecision();
}
