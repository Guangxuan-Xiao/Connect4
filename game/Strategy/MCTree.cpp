#include "MCTree.h"

#include <sys/time.h>
void MCTree::init(const int M, const int N, const int *top, const int *board) {}
Point MCTree::search(int timeLimit) {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    Point nextMove(0, 0);
    for (int times = 0; true; ++times) {
        gettimeofday(&end, NULL);
        int usedTime =
            1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        if (usedTime >= timeLimit) break;
    }
}