#include <iostream>

#include "Phase.hpp"
using namespace std;
void testPhase() {
    Phase::setBoard(8, 8);
    Phase phase;
    cout << sizeof(phase) << endl;
    phase.printBoard();
    Phase newPhase = phase.play(3, 1);
    for (int i = 2; i >= 0; --i) {
        for (int j = 0; j < 3 - i; ++j) newPhase = newPhase.play(i, 2);
        newPhase = newPhase.play(i, 1);
    }
    newPhase.printBoard();
    cout << newPhase.userWin() << endl;
    cout << newPhase.machineWin() << endl;
    phase.printBoard();
}

int main() {
    cout << "Hello World!" << endl;
    testPhase();
    return 0;
}