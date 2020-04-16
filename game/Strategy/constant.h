#ifndef CONSTANT_H_
#define CONSTANT_H_
const int NODEPOOL_SIZE = 1.1e7;
#ifdef WINDOWS
const int TIME_LIMIT = 2900;
#else
const int TIME_LIMIT = 2900000;
#endif
const int MAX_N = 12;
const int MAX_M = 12;
const float UCB_C = 1.0;
const float K = 1000;
const int SEED = 42;
#endif
