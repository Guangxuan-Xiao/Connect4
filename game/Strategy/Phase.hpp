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

    inline bool canPlay(int col) const { return top[col] > 0; }

    void play(int col, int player) {
        if (player == 1)
            user[col] |= (1 << (M - top[col]));
        else if (player == 2)
            machine[col] |= (1 << (M - top[col]));
        top[col] -= 1;
        if (noY == col && top[col] == noX + 1) top[col] -= 1;
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
        unsigned short pos[MAX_N] = {0};
        if (player == 1)
            memmove(pos, user, sizeof(user));
        else
            memmove(pos, machine, sizeof(machine));
        pos[col] |= (1 << (M - top[col]));
        return centralAlign(pos, col);
    }

    bool isLosingMove(int col, int player) const {
        unsigned short pos[MAX_N] = {0};
        int newtop = top[col] - 1;
        if (noY == col && newtop == noX + 1) newtop -= 1;
        if (player == 2)
            memmove(pos, user, sizeof(user));
        else
            memmove(pos, machine, sizeof(machine));
        pos[col] |= (1 << (M - newtop));
        return centralAlign(pos, col);
    }

    void printBoard() const {
        cout << "Board:" << endl;
        for (int i = M - 1; i >= 0; --i) {
            for (int j = 0; j < N; ++j) {
                if (M - 1 - i == noX && j == noY) {
                    cout << "X"
                         << " ";
                    continue;
                }
                if ((user[j] >> i) & 1)
                    cout << "U"
                         << " ";
                else if ((machine[j] >> i) & 1)
                    cout << "M"
                         << " ";
                else
                    cout << "."
                         << " ";
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

    int moveScore(int col, char player) const {
        int mid = (N >> 1), score = 1;
        score += align3(col, player);
        if (score > 1000) return score;
        score += align3(col, 3 - player);
        if (score > 1000) return score;
        score += (col < mid) ? col / 3 : (N - 1 - col) / 3;
        // if (isLosingMove(col, player)) return 0;
        return score;
    }

    float score() const { return 1 - (float)moves / (M * N); }

    unsigned char top[MAX_N];
    unsigned short user[MAX_N], machine[MAX_N];
    int moves;
    static int M, N;
    static int noX, noY;

   private:
    bool alignment(const unsigned short* pos) const {
        // Horizontal
        unsigned short m[MAX_N] = {0};
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

    bool centralAlign(const unsigned short* pos, int col) const {
        unsigned short m[MAX_N] = {0};
        int left = (col - 3) < 0 ? 0 : col - 3;
        int right = (col + 3) < N ? col + 3 : N - 1;
        for (int i = left; i < right; ++i) m[i] = pos[i] & pos[i + 1];
        for (int i = left; i < right - 2; ++i)
            if (m[i] & m[i + 2]) return true;

        // Vertical
        m[col] = pos[col] & (pos[col] >> 1);
        if (m[col] & (m[col] >> 2)) return true;
        // Diagnal 1: Bottom-left to upper-right
        for (int i = left; i < right; ++i) m[i] = pos[i] & (pos[i + 1] >> 1);
        for (int i = left; i < right - 2; ++i)
            if (m[i] & (m[i + 2] >> 2)) return true;

        // Diagnal 2: Upper-left to bottom-right
        for (int i = left; i < right; ++i) m[i] = pos[i] & (pos[i + 1] << 1);
        for (int i = left; i < right - 2; ++i)
            if (m[i] & (m[i + 2] << 2)) return true;
        return false;
    }

    int align3(int col, char player) const {
        unsigned short pos[MAX_N] = {0};
        if (player == 1)
            memmove(pos, user, sizeof(user));
        else
            memmove(pos, machine, sizeof(machine));
        pos[col] |= (1 << (M - top[col]));
        int score = 0;
        unsigned short m[MAX_N] = {0};
        // Horizontal
        int left = (col - 3) < 0 ? 0 : col - 3;
        int right = (col + 3) < N ? col + 3 : N - 1;
        for (int i = left; i < right; ++i) m[i] = pos[i] & pos[i + 1];
        for (int i = left; i < right - 2; ++i)
            if (m[i] & m[i + 2]) return 1000000;
        for (int i = left + 1; i < col - 1; ++i)
            if (m[i] & (1 << (M - top[col]))) ++score;
        for (int i = col + 1; i < right - 1; ++i)
            if (m[i] & (1 << (M - top[col]))) ++score;

        // Vertical
        m[col] = pos[col] & (pos[col] >> 1);
        if (m[col] & (m[col] >> 2)) return 1000000;
        if ((m[col] >> (M - top[col] - 2)) & 1) score++;

        // Diagnal 1: Bottom-left to upper-right
        for (int i = left; i < right; ++i) m[i] = pos[i] & (pos[i + 1] >> 1);
        for (int i = left; i < right - 2; ++i)
            if (m[i] & (m[i + 2] >> 2)) return 1000000;
        for (int i = left + 1; i < col - 1; ++i)
            if (m[i] & (1 << (M - top[col] - col + i))) ++score;
        for (int i = col + 1; i < right - 1; ++i)
            if (m[i] & (1 << (M - top[col] - col + i))) ++score;

        // Diagnal 2: Upper-left to bottom-right
        for (int i = left; i < right; ++i) m[i] = pos[i] & (pos[i + 1] << 1);
        for (int i = left; i < right - 2; ++i)
            if (m[i] & (m[i + 2] << 2)) return 1000000;
        for (int i = left + 1; i < col - 1; ++i)
            if (m[i] & (1 << (M - top[col] + col - i))) ++score;
        for (int i = col + 1; i < right - 1; ++i)
            if (m[i] & (1 << (M - top[col] + col - i))) ++score;

        return score;
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
