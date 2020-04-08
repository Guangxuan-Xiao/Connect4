g++ -Wall -std=c++11 -O2 -fpic -shared -DDEBUG -D OUTPUT -g -rdynamic ./Strategy/Judge.cpp ./Strategy/Strategy.cpp ./Strategy/MCTree.cpp ./Strategy/Phase.hpp -o ./so/Strategy.so.d
g++ -std=c++11 ./Compete/*.cpp -o ./Compete/Compete -ldl -lpthread -g -fnon-call-exceptions -Wall -DDEBUG -rdynamic
./Compete/Compete  so/Strategy.so.d so/Strategy.so result_debug.txt 5