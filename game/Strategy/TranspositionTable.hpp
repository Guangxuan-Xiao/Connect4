#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include <cassert>
#include <cstring>
#include <vector>
typedef unsigned long long ull;
class TranspositionTable {
   private:
    static const size_t size = (1 << 23) + 9;
    typedef unsigned int key_t;
    typedef unsigned char value_t;

    key_t *K;
    value_t *V;

    size_t index(ull key) const { return key % size; }
    public:
};

#endif