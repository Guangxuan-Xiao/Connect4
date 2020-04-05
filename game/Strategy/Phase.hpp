#ifndef PHASE_H_
#define PHASE_H_
#include <assert.h>
#include <string.h>

#include <iostream>
using namespace std;
const int MAX_N = 12;
const int MAX_M = 12;
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
        memmove(top, _top, N * sizeof(int));
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                setPiece(i, j, board[i][j]);
                if (board[i][j]) ++moves;
            }
        }
    }

    bool canPlay(int col) const {
        assert(col < N && col >= 0);
        return top[col] < M;
    }

    Phase play(int col, int player) const {
        Phase newPhase;
        memmove(newPhase.user, user, sizeof(user));
        memmove(newPhase.machine, machine, sizeof(machine));
        memmove(newPhase.top, top, sizeof(top));
        if (player == 1)
            newPhase.user[col] |= (1 << newPhase.top[col]);
        else if (player == 2)
            newPhase.machine[col] |= (1 << newPhase.top[col]);
        else {
            cout << "Invalid Player!" << endl;
            exit(1);
        }
        newPhase.top[col] += 1;
        newPhase.moves = moves + 1;
        return newPhase;
    }

    bool userWin() const { return alignment(user); }

    bool machineWin() const { return alignment(machine); }

    bool terminal() const {
        for (int i = 0; i < N; ++i)
            if (top[i] < M) return false;
        return true;
    }

    bool isWinningMove(int col, int player) const {
        unsigned int pos[MAX_N];
        if (player == 1)
            memmove(pos, user, sizeof(user));
        else if (player == 2)
            memmove(pos, machine, sizeof(machine));
        else {
            cout << "Invalid Player!" << endl;
            exit(1);
        }
        return alignment(pos);
    }

    void printBoard() const {
        cout << "User Positions" << endl;
        for (int i = M - 1; i >= 0; --i) {
            for (int j = 0; j < N; ++j) {
                cout << ((user[j] >> i) & 1) << " ";
            }
            cout << endl;
        }
        cout << "Machine Positions" << endl;
        for (int i = M - 1; i >= 0; --i) {
            for (int j = 0; j < N; ++j) {
                cout << ((machine[j] >> i) & 1) << " ";
            }
            cout << endl;
        }
    }

    static void setBoard(int m, int n) {
        M = m;
        N = n;
    }

    int score() const { return (M * N - moves) / 2; }

    unsigned char top[MAX_N];
    unsigned int user[MAX_N], machine[MAX_N];
    int moves;
    static int M;
    static int N;

   private:
    bool alignment(const unsigned int* pos) const {
        // Horizontal
        unsigned int m[MAX_N];
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
int Phase::M = 12;
int Phase::N = 12;
#endif