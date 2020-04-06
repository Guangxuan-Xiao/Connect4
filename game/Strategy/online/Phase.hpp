#ifndef PHASE_HPP_
#define PHASE_HPP_
#include <assert.h>
#include <string.h>

#include <iostream>

#include "constant.h"
using namespace std;

class Phase {
   public:
    Phase() : moves(0) {
        memset(user, 0, sizeof(user));
        memset(machine, 0, sizeof(machine));
        memset(top, 0, sizeof(top));
    }

    Phase(int** board, const int* _top) : moves(0) {
        memset(user, 0, sizeof(user));
        memset(machine, 0, sizeof(machine));
        memset(top, 0, sizeof(top));
        for (int i = 0; i < N; ++i) top[i] = _top[i];
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                setPiece(i, j, board[i][j]);
                if (board[i][j]) ++moves;
            }
        }
    }

    Phase(const Phase& phase) : moves(phase.moves) {
        memmove(user, phase.user, sizeof(user));
        memmove(machine, phase.machine, sizeof(machine));
        memmove(top, phase.top, sizeof(top));
    }

    Phase& operator=(const Phase& phase) {
        moves = phase.moves;
        memmove(user, phase.user, sizeof(user));
        memmove(machine, phase.machine, sizeof(machine));
        memmove(top, phase.top, sizeof(top));
        return *this;
    }

    bool canPlay(int col) const {
        assert(col < N && col >= 0);
        return top[col] > 0;
    }

    void play(int col, int player) {
        if (player == 1)
            user[col] |= (1 << (M - top[col]));
        else if (player == 2)
            machine[col] |= (1 << (M - top[col]));
        else {
            cout << "Invalid Player " << player << endl;
            exit(1);
        }
        top[col] -= 1;
        if (noY == col && top[col] == noX) top[col] -= 1;
        moves = moves + 1;
    }

    bool userWin() const { return alignment(user); }

    bool machineWin() const { return alignment(machine); }

    int winner() const {
        if (alignment(user)) return 1;
        if (alignment(machine)) return 2;
        return 0;
    }

    bool terminal() const {
        for (int i = 0; i < N; ++i)
            if (top[i] > 0) return false;
        return true;
    }

    bool isWinningMove(int col, int player) const {
        unsigned int pos[MAX_N] = {0};
        if (player == 1)
            memmove(pos, user, sizeof(user));
        else
            memmove(pos, machine, sizeof(machine));
        pos[col] |= (1 << (M - top[col]));
        return alignment(pos);
    }

    bool isLosingMove(int col, int player) const {
        unsigned int pos[MAX_N] = {0};
        int newtop = top[col] - 1;
        if (noY == col && newtop == noX) newtop -= 1;
        if (player == 2)
            memmove(pos, user, sizeof(user));
        else
            memmove(pos, machine, sizeof(machine));
        pos[col] |= (1 << (M - newtop));
        return alignment(pos);
    }

    void printBoard() const {
        cout << "Board:" << endl;
        for (int i = M - 1; i >= 0; --i) {
            for (int j = 0; j < N; ++j) {
                if ((user[j] >> i) & 1)
                    cout << 1 << " ";
                else if ((machine[j] >> i) & 1)
                    cout << 2 << " ";
                else
                    cout << 0 << " ";
            }
            cout << endl;
        }
    }

    static void set(int m, int n, int _noX, int _noY) {
        M = m;
        N = n;
        noX = _noX;
        noY = _noY;
    }

    double score() const { return 1 - (double)moves / (M * N); }

    unsigned char top[MAX_N];
    unsigned int user[MAX_N], machine[MAX_N];
    int moves;
    static int M, N;
    static int noX, noY;

   private:
    bool alignment(const unsigned int* pos) const {
        // Horizontal
        unsigned int m[MAX_N] = {0};
        for (int i = 0; i < N - 1; ++i) m[i] = pos[i] & pos[i + 1];
        for (int i = 0; i < N - 3; ++i)
            if (m[i] & m[i + 2]) return true;

        // Vertical
        for (int i = 0; i < N; ++i) {
            m[i] = pos[i] & (pos[i] >> 1);
            if (m[i] & (m[i] >> 2)) return true;
        }

        // Diagnal 1: Bottom-left to upper-right
        for (int i = 0; i < N - 1; ++i) m[i] = pos[i] & (pos[i + 1] >> 1);
        for (int i = 0; i < N - 3; ++i)
            if (m[i] & (m[i + 2] >> 2)) return true;

        // Diagnal 2: Upper-left to bottom-right
        for (int i = 0; i < N - 1; ++i) m[i] = pos[i] & (pos[i + 1] << 1);
        for (int i = 0; i < N - 3; ++i)
            if (m[i] & (m[i + 2] << 2)) return true;

        return false;
    }

    void setPiece(int x, int y, int player) {
        if (player == 1) {
            user[y] |= 1 << (M - x - 1);
        } else if (player == 2) {
            machine[y] |= 1 << (M - x - 1);
        } else
            return;
    }
};
#endif